#include <QtConcurrent>
#include "gnomonWorkspaceMorphonet.h"

#include "gnomonCommand/gnomonCellImage/gnomonCellImageReaderCommand"
#include "gnomonCommand/gnomonCellImage/gnomonCellImageWriterCommand"
#include <gnomonCore/gnomonForm/gnomonCellImage/gnomonCellImage.h>
#include <gnomonCore/gnomonMorphonetHelper.h>
#include <gnomonCore/simpleCrypt.h>
#include "gnomonManager/gnomonFormManager"
#include <gnomonPipeline/gnomonPipelineManager.h>
#include <gnomonVisualization/gnomonView/gnomonVtkView.h>

#include <dtkLog>
#include <QtCore>

#pragma push_macro("slots")
#undef slots
#include <Python.h>
#pragma pop_macro("slots")

#include <dtkScript>
#include <csignal>

class gnomonWorkspaceMorphonetPrivate{

public:
    gnomonWorkspaceMorphonetPrivate() = default;
    ~gnomonWorkspaceMorphonetPrivate();

public:
    QString encodePassword(const QString& password);    
    QString decodePassword(const QString& encoded_password);

    bool selectDataset(int id);
    void loadMNDataAtTime(int time, double voxelsize, bool load_infos);

    enum Status {
        Morphonet_NotLoaded,
        Morphonet_disconnected,
        Morphonet_connected
    };

public: 
    void clearWatcherAndForms(void);

public:
    QSettings settings = QSettings(QSettings::IniFormat, QSettings::UserScope, "inria", "gnomon");
    Status morphonet_status = Morphonet_NotLoaded;
   
    int current_id = -1, dataset_start_time=-1, dataset_end_time=-1;
    double voxelsize;
    bool upload_mode = false;
    gnomonPipelineManager *pipeline_manager;

    gnomonVtkView *view = nullptr;
    std::shared_ptr<gnomonCellImageSeries> img_series = nullptr;

    QFutureWatcher<void> *watcher = nullptr;
    QProcess *morphoplot_process =  nullptr;
    QTemporaryDir *morphoplot_tmp_dir = nullptr;

private: 
    SimpleCrypt crypto = SimpleCrypt(Q_UINT64_C(0x0c2ad6a4adb3f073));
};


QString gnomonWorkspaceMorphonetPrivate::encodePassword(const QString& password) 
{
    return crypto.encryptToString(password);
}


QString gnomonWorkspaceMorphonetPrivate::decodePassword(const QString& encoded_password) 
{
    return crypto.decryptToString(encoded_password);
}

bool gnomonWorkspaceMorphonetPrivate::selectDataset(int id)
{
    if(id != -1) {
        qInfo() << Q_FUNC_INFO << "setting morphonet dataset id to" << id;
        current_id = id;
    } else {
        return false;
    }

    //clear current data 
    this->clearWatcherAndForms();

    if(gnomonMorphonetHelper::instance()->selectDataset(id)) {
        this->dataset_start_time = gnomonMorphonetHelper::instance()->startTime();
        this->dataset_end_time = gnomonMorphonetHelper::instance()->endTime();
    } else {
        qWarning() << Q_FUNC_INFO << "Problem selecting dataset id:" << this->current_id;
        return false;   
    }


    // dtkScriptInterpreterPython::instance()->childAcquireLock();
    // PyObject *pFunc = PyObject_GetAttrString(this->mn_net, "select_dataset_by_id");
    // PyObject *pArgs = Py_BuildValue("(i)", this->current_id);
    // PyObject_CallObject(pFunc, pArgs);
    // Py_XDECREF(pFunc);
    // Py_XDECREF(pArgs); 

    // //  set dataset_start_time and dataset_end_time
    // PyObject *pMinTime = PyObject_GetAttrString(this->mn_net, "minTime");
    // this->dataset_start_time = PyLong_AsLong(pMinTime);
    // Py_XDECREF(pMinTime);

    // PyObject *pMaxTime = PyObject_GetAttrString(this->mn_net, "maxTime");
    // this->dataset_end_time = PyLong_AsLong(pMaxTime);
    // Py_XDECREF(pMaxTime);
    // dtkScriptInterpreterPython::instance()->childReleaseLock();


    return true;
}

void gnomonWorkspaceMorphonetPrivate::loadMNDataAtTime(int time, double voxelsize, bool load_infos)
{
    std::shared_ptr<gnomonCellImage> cell_img = gnomonMorphonetHelper::instance()->loadMnDataAtTime(time, voxelsize, load_infos);

    if(cell_img) {
        this->img_series->insert(double(time), cell_img);
    } else {
        qWarning() << Q_FUNC_INFO << "load_mn_data_at_time returned PyNone or nullptr";
    }
}


void gnomonWorkspaceMorphonetPrivate::clearWatcherAndForms(void) {

    if(watcher){
        watcher->disconnect();
        if(watcher->isRunning()) {
            watcher->cancel();
            watcher->waitForFinished();
        }
    }
    delete watcher;
    watcher = nullptr;

    for(auto time : this->img_series->times()) {
        this->img_series->drop(time);
    } 

}

gnomonWorkspaceMorphonetPrivate::~gnomonWorkspaceMorphonetPrivate() {
    this->clearWatcherAndForms();
    if(morphoplot_process && morphoplot_process->state()!=QProcess::NotRunning){
        morphoplot_process->kill();
    }
    delete morphoplot_process;
    delete morphoplot_tmp_dir;
    delete view;
}


gnomonWorkspaceMorphonet::gnomonWorkspaceMorphonet(QObject *parent) : gnomonAbstractWorkspace(parent)
{
    d = new gnomonWorkspaceMorphonetPrivate();

    d->pipeline_manager = gnomonPipelineManager::instance();
    d->view = new gnomonVtkView(this);
    d->view->setNodePortNames({});
    d->img_series = std::make_shared<gnomonCellImageSeries>();
    d->img_series->metadata()->set("source", "MorphoNet");

    d->view->setAcceptForm("gnomonCellImage",true);

    connect(d->view, &gnomonVtkView::exportedForm, [=] (std::shared_ptr<gnomonAbstractDynamicForm> f) {
        //TODO what to do in pipeline manager if data coming from morphonet?
        d->pipeline_manager->addForm(f);
    });

    int stat;
    dtkScriptInterpreterPython::instance()->interpret("import gnomon.utils.morphonetHelper", &stat);


    if(!gnomonMorphonetHelper::instance() ) {
        //timer to wait until qml component is created. Otherwise, it won't catch the message
        QTimer::singleShot(100, this, [this](){
            emit message("Cannot load morphonet, please install it (pip install morphonet)");
        });
    } else {
        d->morphonet_status = gnomonWorkspaceMorphonetPrivate::Morphonet_disconnected;

        //automatically try to connect to Morphonet using saved login and password
        if(d->settings.contains("morphonet/login") 
            && d->settings.contains("morphonet/password")  
            && this->login(d->settings.value("morphonet/login").toString(), d->decodePassword(d->settings.value("morphonet/password").toString()))) {
            d->morphonet_status = gnomonWorkspaceMorphonetPrivate::Morphonet_connected;

            //timer to wait until qml component is created. Otherwise, it won't catch the message
            QTimer::singleShot(100, this, [this](){
                emit connectionStatusChanged();
            });
        }
    }
}


gnomonWorkspaceMorphonet::~gnomonWorkspaceMorphonet()
{
    delete d;
}

bool gnomonWorkspaceMorphonet::login(const QString& login, const QString& passwd)
{
    if(d->morphonet_status == gnomonWorkspaceMorphonetPrivate::Morphonet_connected) {
        this->disconnect();
    }

    if(d->morphonet_status != gnomonWorkspaceMorphonetPrivate::Morphonet_disconnected) {
        qWarning() << "Morphonet not loaded, nothing done";
        emit message("Morphonet not loaded, nothing done");
        return false;
    }

    if(login.isEmpty() || passwd.isEmpty()) {
        qWarning() << Q_FUNC_INFO << "Empty login or password";
        emit message("Empty login or password");
        return false;
    }

    if(gnomonMorphonetHelper::instance()->connect(login, passwd)) {
        qInfo() << "Connected to morphonet";
    } else {
        emit message("cannot connect to Morphonet with login: " + login);
        return false;
    }

    // save to settings
    d->settings.setValue("morphonet/login", login);
    d->settings.setValue("morphonet/password", d->encodePassword(passwd));

    d->morphonet_status = gnomonWorkspaceMorphonetPrivate::Morphonet_connected;
    emit connectionStatusChanged();
    return true;
}

bool gnomonWorkspaceMorphonet::disconnect(void)
{
    if(d->morphonet_status != gnomonWorkspaceMorphonetPrivate::Morphonet_connected) {
        return false;
    }

    qDebug() << Q_FUNC_INFO << "TODO  not implemented";    

    d->morphonet_status = gnomonWorkspaceMorphonetPrivate::Morphonet_disconnected;
    emit connectionStatusChanged();
    return true;
}

bool gnomonWorkspaceMorphonet::connected(void)
{
    return d->morphonet_status == gnomonWorkspaceMorphonetPrivate::Morphonet_connected;
}


int gnomonWorkspaceMorphonet::currentId(void) const
{
    return d->current_id;
}

int gnomonWorkspaceMorphonet::timeStart(void) const
{
    return d->dataset_start_time;
}

int gnomonWorkspaceMorphonet::timeEnd(void) const
{
    return d->dataset_end_time;
}

bool gnomonWorkspaceMorphonet::uploadMode(void) const
{
    return d->upload_mode;
}

void gnomonWorkspaceMorphonet::setTimeStart(int new_time)
{
    d->dataset_start_time = new_time;
    emit timeStartChanged();
}

void gnomonWorkspaceMorphonet::setTimeEnd(int new_time)
{
    d->dataset_end_time = new_time;
    emit timeEndChanged();
}

void gnomonWorkspaceMorphonet::setCurrentId(int new_id)
{
    d->current_id = new_id;
    emit currentIdChanged();
}

void gnomonWorkspaceMorphonet::setUploadMode(bool upload) 
{
    d->upload_mode = upload;
    emit uploadModeChanged();
}

bool gnomonWorkspaceMorphonet::deleteDataset(int id)
{
    if(id==-1)
        id = d->current_id;

    return gnomonMorphonetHelper::instance()->deleteDataset(id);
}

QString gnomonWorkspaceMorphonet::importDatasetInfos(const QString& search)
{
    QString res = "";
    if(d->morphonet_status != gnomonWorkspaceMorphonetPrivate::Morphonet_connected) {
        qWarning() << Q_FUNC_INFO << "Morphonet status is not connected. nothing is done";
        return res;
    }
 
    return gnomonMorphonetHelper::instance()->datasetsInfo(search);
}

void gnomonWorkspaceMorphonet::importDataset(int id, double voxelsize, int time_start, int time_end)
{
    emit started();
    if(d->morphonet_status != gnomonWorkspaceMorphonetPrivate::Morphonet_connected) {
        qWarning() << Q_FUNC_INFO << "Morphonet status is not connected. nothing is done";
        return;
    }

    if(id != -1) {
        qInfo() << Q_FUNC_INFO << "setting morphonet dataset id to" << id;
        d->current_id = id;
        d->voxelsize = voxelsize;
    }

    //1 select dataset
    bool ok = d->selectDataset(d->current_id);
    if(!ok) {
        message("cannot select dataset:" + QString::number(d->current_id));
        return;
    }


    // load only the first frame if not specified otherwise
    int t0 = time_start;
    int t_end = time_end;
    if(time_start == -1 || time_end == -1) {
        t0 = d->dataset_start_time;
        t_end = d->dataset_start_time;
    }

    d->clearWatcherAndForms();
    d->watcher = new QFutureWatcher<void>();
    connect(d->watcher, &QFutureWatcher<void>::finished, [this, t0, t_end]() {
        this->onDataLoaded(t0, t_end);
        emit finished();
    });
    
    auto future = QtConcurrent::run([=](){
        for(int time = t0; time <= t_end; time++) {
            std::shared_ptr<gnomonCellImage> cell_img = gnomonMorphonetHelper::instance()->loadMnDataAtTime(time, voxelsize);
            if(cell_img) {
                d->img_series->insert(double(time), cell_img);
            } else {
                qWarning() << Q_FUNC_INFO << "load_mn_data_at_time " << time << " returned PyNone or nullptr";
            }
        }
    });
    d->watcher->setFuture(future);
}

void gnomonWorkspaceMorphonet::onDataLoaded(int startTime, int endTime)
{
    if(!d->img_series->times().isEmpty()) {
        d->view->clear();
        int form_count = gnomonFormManager::instance()->formCount(d->img_series->formName());
        d->img_series->metadata()->set("name", d->img_series->formName().remove("gnomon") + QString::number(form_count+1));
        d->view->setForm("gnomonCellImage", d->img_series, {});
        d->pipeline_manager->addMorphoForm(d->img_series, d->current_id, d->voxelsize, startTime, endTime);

        emit timeEndChanged();
    }
}

int gnomonWorkspaceMorphonet::exportDataset(QString name, int id_NCBI, int id_type, QString description, double voxelsize)
{
    int res = -1;
    if(d->morphonet_status != gnomonWorkspaceMorphonetPrivate::Morphonet_connected) {
        qWarning() << Q_FUNC_INFO << "Morphonet status is not connected. nothing is done";
        return res;
    }

    auto serie = std::dynamic_pointer_cast<gnomonCellImageSeries>(d->view->form("gnomonCellImage"));
    
    if(serie)
        res = gnomonMorphonetHelper::instance()->createDataset(name, serie, id_NCBI, id_type, description, voxelsize);
    else {
        message("Set a cellImageSeries before creating a dataset");
    }

    return res;
}

int gnomonWorkspaceMorphonet::morphoPlot(double voxelsize)
{
    auto image = this->view()->cellImage();
    if(!image) {
        return 1;
    }

    if(d->morphoplot_process) {
        // cleaning up
        kill((pid_t)d->morphoplot_process->processId(), SIGINT);
        d->morphoplot_process->waitForFinished(3000);
        d->morphoplot_process->kill();
        d->morphoplot_process->waitForFinished(10000);
        delete d->morphoplot_process;
    }

    d->morphoplot_process = new QProcess();
    d->morphoplot_process->setProcessChannelMode(QProcess::ForwardedChannels);
    QObject::connect(d->morphoplot_process, &QProcess::readyReadStandardOutput, [&]() {
        QStringList data = QString(d->morphoplot_process->readAllStandardOutput()).split("\n");
        for(auto d : data) {
            //dtkInfo() << "morphoplot dtk: " << d;
            qDebug() << "morphoplot: " << d;
        }
    });

    QObject::connect(d->morphoplot_process, &QProcess::readyReadStandardError, [&]() {
        QStringList data = QString(d->morphoplot_process->readAllStandardError()).split("\n");
        for(auto d: data) {
            //dtkWarn() << "morphoplot error dtk: " << d;
            qWarning() << "morphoplot error: " << d;
        }
    });

    d->morphoplot_process->start(QString("_morphoplot_server"));
    if(!d->morphoplot_process->waitForStarted(3000) ) {
        dtkWarn() << "Cannot start process _morphoplot_server";
        message("error starting morphonet Plot");
        return 1;
    }

    bool server_found = gnomonMorphonetHelper::instance()->startCuration("name", image, 1, 3, "description", voxelsize);

    if(!server_found) {
        message("Morphonet Server not found!");
        return 1;
    }
    dtkInfo() << "MorphoNet plot launched: " << d->morphoplot_process->state();
    return 0;
}

void gnomonWorkspaceMorphonet::morphoPlotCollect(void)
{
    if(d->morphoplot_process) {
        //auto image = this->view()->cellImage();
        qDebug() << "launch collect for image ";
        auto image = gnomonMorphonetHelper::instance()->collectCuration();
        image->metadata()->set("source", "MorphoPlot");
        qDebug() << "collect done";

        auto input_image = this->view()->cellImage();
        QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs;
        inputs["cellImage"] = input_image;

        QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs;
        outputs["curatedCellImage"] = image;

        this->view()->setForm("gnomonCellImage", image);
        d->pipeline_manager->addTask("morphoPlotCuration", inputs, outputs);

        //this->view()->setForm("gnomonCellImage", imageSerie, {}); //same image, not needed?
        // just beed to refresh the view
    
        // cleaning up
        d->morphoplot_process->kill();
        d->morphoplot_process->waitForFinished(5000);
        delete d->morphoplot_process;
        d->morphoplot_process = nullptr;
    }
}

gnomonVtkView *gnomonWorkspaceMorphonet::view(void)
{
    return d->view;
}

void gnomonWorkspaceMorphonet::saveState(void) 
{
    //TODO
}

void gnomonWorkspaceMorphonet::restoreState(void) 
{
    //TODO
}

void gnomonWorkspaceMorphonet::export_outputs(void) {
    d->view->transmit();
}
