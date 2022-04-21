#include "gnomonWorkspaceMorphonet.h"
#include "gnomonCommand/gnomonCellImage/gnomonCellImageWriterCommand"
#include "gnomonCommand/gnomonCellImage/gnomonCellImageReaderCommand"
#include "gnomonManager/gnomonFormManager"

#include <gnomonCore/simpleCrypt.h>
#include <gnomonCore/gnomonMorphonetHelper.h>

#include <gnomonCore/gnomonForm/gnomonCellImage/gnomonCellImage.h>
#include <gnomonPipeline/gnomonPipelineManager.h>
#include <gnomonVisualization/gnomonView/gnomonViewForm.h>

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
    void clear(void);

public:
    QSettings settings = QSettings(QSettings::IniFormat, QSettings::UserScope, "inria", "gnomon");
    Status morphonet_status = Morphonet_NotLoaded;
   
   
    //PyObject *mn_module = nullptr, *mn_net = nullptr, *gnm_mn_helper = nullptr;
    int current_id = -1, start_time=-1, end_time=-1;
    bool upload_mode = false;
    gnomonPipelineManager *pipeline_manager;

    gnomonViewForm *view = nullptr;
    gnomonCellImageSeries *img_series = nullptr;

    QProcess *morphoplot_process =  nullptr;
    QTemporaryDir *morphoplot_tmp_dir = nullptr;
    //gnomonAbstractDynamicForm *current_form; // time_series ? 

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
    this->clear();

    if(gnomonMorphonetHelper::instance()->selectDataset(id)) {
        this->start_time = gnomonMorphonetHelper::instance()->startTime();
        this->end_time = gnomonMorphonetHelper::instance()->endTime();
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

    // //  set start_time and end_time
    // PyObject *pMinTime = PyObject_GetAttrString(this->mn_net, "minTime");
    // this->start_time = PyLong_AsLong(pMinTime);
    // Py_XDECREF(pMinTime);

    // PyObject *pMaxTime = PyObject_GetAttrString(this->mn_net, "maxTime");
    // this->end_time = PyLong_AsLong(pMaxTime);
    // Py_XDECREF(pMaxTime);
    // dtkScriptInterpreterPython::instance()->childReleaseLock();


    return true;
}

void gnomonWorkspaceMorphonetPrivate::loadMNDataAtTime(int time, double voxelsize, bool load_infos)
{
    gnomonCellImage *cell_img = gnomonMorphonetHelper::instance()->loadMnDataAtTime(time, voxelsize, load_infos);

    if(cell_img) {
        this->img_series->insert(double(time), cell_img);
    } else {
        qWarning() << Q_FUNC_INFO << "load_mn_data_at_time returned PyNone or nullptr";
    }
}


void gnomonWorkspaceMorphonetPrivate::clear(void) {
    this->view->clear();

    for(auto time : this->img_series->times()) {\
        auto *img = this->img_series->at(time);
        this->img_series->drop(time);
        delete img;
    } 

}

gnomonWorkspaceMorphonetPrivate::~gnomonWorkspaceMorphonetPrivate() {
    delete morphoplot_process;
    delete morphoplot_tmp_dir;
    delete view;
    delete img_series;
}


gnomonWorkspaceMorphonet::gnomonWorkspaceMorphonet(QObject *parent) : gnomonAbstractWorkspace(parent)
{
    d = new gnomonWorkspaceMorphonetPrivate();

    d->pipeline_manager = gnomonPipelineManager::instance();
    d->view = new gnomonViewForm(this);
    d->img_series = new gnomonCellImageSeries();
    d->view->setAcceptForm("gnomonCellImage",true);

    connect(d->view, &gnomonViewForm::exportedForm, [=] (gnomonAbstractDynamicForm *f) {
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
    return d->start_time;
}

int gnomonWorkspaceMorphonet::timeEnd(void) const
{
    return d->end_time;
}

bool gnomonWorkspaceMorphonet::uploadMode(void) const
{
    return d->upload_mode;
}

void gnomonWorkspaceMorphonet::setTimeStart(int new_time)
{
    d->start_time = new_time;
    emit timeStartChanged();
}

void gnomonWorkspaceMorphonet::setTimeEnd(int new_time)
{
    d->end_time = new_time;
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

QString gnomonWorkspaceMorphonet::datasetsInfo(const QString& search)
{
    QString res = "";
    if(d->morphonet_status != gnomonWorkspaceMorphonetPrivate::Morphonet_connected) {
        qWarning() << Q_FUNC_INFO << "Morphonet status is not connected. nothing is done";
        return res;
    }

    return gnomonMorphonetHelper::instance()->datasetsInfo(search);
}

void gnomonWorkspaceMorphonet::importDatasetPreview(int id, double voxelsize)
{
    if(d->morphonet_status != gnomonWorkspaceMorphonetPrivate::Morphonet_connected) {
        qWarning() << Q_FUNC_INFO << "Morphonet status is not connected. nothing is done";
        return;
    }

    d->clear();

    bool ok = d->selectDataset(id);
    if(!ok) {
        message(QString("cannot select dataset: %1").arg(id));
        return;
    }

    //import first time of selected dataset and set it to the view
    d->loadMNDataAtTime(d->start_time, voxelsize, false);

    if(!d->img_series->times().isEmpty())
        d->view->setForm("CellImage", d->img_series); 

}


void gnomonWorkspaceMorphonet::importDataset(int time_start, int time_end, int id, double voxelsize)
{
    if(d->morphonet_status != gnomonWorkspaceMorphonetPrivate::Morphonet_connected) {
        qWarning() << Q_FUNC_INFO << "Morphonet status is not connected. nothing is done";
        return;
    }

    if(id != -1) {
        qInfo() << Q_FUNC_INFO << "setting morphonet dataset id to" << id;
        d->current_id = id;
    }

    //1 select dataset
    bool ok = d->selectDataset(d->current_id);
    if(!ok) {
        message("cannot select dataset:" + QString::number(d->current_id));
        return;
    }

    d->clear();

    for(int time = time_start; time <= time_end; time++) {
      d->loadMNDataAtTime(time, voxelsize, true);
    }

    if(!d->img_series->times().isEmpty())
        d->view->setForm("CellImage", d->img_series); 
}

int gnomonWorkspaceMorphonet::importDatasetInfos(void)
{
    int res = -1;
    if(d->morphonet_status != gnomonWorkspaceMorphonetPrivate::Morphonet_connected) {
        qWarning() << Q_FUNC_INFO << "Morphonet status is not connected. nothing is done";
        return res;
    }

    auto *serie = dynamic_cast<gnomonCellImageSeries *>(d->view->form("gnomonCellImage"));

    if (serie) {
        gnomonMorphonetHelper::instance()->loadMnInfos(serie);
        d->view->setCellImage(serie);
        res = 1;
    } else {
        message("No cellImageSeries! Impossible to update infos");
    }

    return res;
}

int gnomonWorkspaceMorphonet::exportDataset(QString name, int id_NCBI, int id_type, QString description)
{
    int res = -1;
    if(d->morphonet_status != gnomonWorkspaceMorphonetPrivate::Morphonet_connected) {
        qWarning() << Q_FUNC_INFO << "Morphonet status is not connected. nothing is done";
        return res;
    }

    auto *serie = dynamic_cast<gnomonCellImageSeries *>(d->view->form("gnomonCellImage"));
    
    if(serie)
        res = gnomonMorphonetHelper::instance()->createDataset(name, serie, id_NCBI, id_type, description);
    else {
        message("Set a cellImageSeries before creating a dataset");
    }

    return res;
}

void gnomonWorkspaceMorphonet::morphoPlot(void)
{
    auto image = this->view()->cellImage();
    if(!image) {
        return;
    }

    if(d->morphoplot_process) {
        // cleaning up
        kill((pid_t)d->morphoplot_process->processId(), SIGINT);
        d->morphoplot_process->waitForFinished(3000);
        d->morphoplot_process->kill();
        d->morphoplot_process->waitForFinished(10000);
        delete d->morphoplot_process;
    }
    delete d->morphoplot_tmp_dir;
    d->morphoplot_tmp_dir = new QTemporaryDir();
    auto filepath = d->morphoplot_tmp_dir->filePath("edited_seg_img.inr.gz");



    auto writer = gnomonCellImageWriterCommand();
    writer.setCellImage(image);
    writer.setAlgorithmName("cellImageWriterTissueImage");
    writer.setPath(filepath);
    writer.setNoAsync();

    writer.predo();
    writer.redo();
    writer.postdo();

    d->morphoplot_process = new QProcess();
    /*
    d->morphoplot_process->setProgram("_run_morphoplot");
    d->morphoplot_process->setArguments({filepath});
    d->morphoplot_process->start();
     */
    d->morphoplot_process->startCommand(QString("_run_morphoplot %1").arg(filepath));


    //gnomonMorphonetHelper::instance()->morphoPlot(form);
    qDebug() << Q_FUNC_INFO << "end mn plot " << d->morphoplot_process->state();
}

void gnomonWorkspaceMorphonet::morphoPlotCollect(void)
{
    if(d->morphoplot_process) {
        // terminate morphoplot server
        kill((pid_t)d->morphoplot_process->processId(), SIGINT);

        // collecting file
        auto reader = gnomonCellImageReaderCommand();
        reader.setAlgorithmName("cellImageReaderTimagetk");
        reader.setPath(d->morphoplot_tmp_dir->filePath("edited_seg_img.inr.gz"));
        reader.setNoAsync();

        reader.predo();
        reader.redo();
        reader.postdo();

        auto cellImage_series = dynamic_cast<gnomonCellImageSeries *>(reader.cellImage()->clone());

        int form_count = gnomonFormManager::instance()->formCount(cellImage_series->formName());
        cellImage_series->metadata()->set("name", cellImage_series->formName().remove("gnomon") + QString::number(form_count+1));
        cellImage_series->metadata()->set("source", "Morphoplot");
        d->view->setCellImage(cellImage_series, {});


        // cleaning up
        d->morphoplot_process->waitForFinished(3000);
        d->morphoplot_process->kill();
        d->morphoplot_process->waitForFinished(10000);
        delete d->morphoplot_process;
        d->morphoplot_process = nullptr;
        delete d->morphoplot_tmp_dir;
        d->morphoplot_tmp_dir = nullptr;
    }
}

gnomonViewForm *gnomonWorkspaceMorphonet::view(void)
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
