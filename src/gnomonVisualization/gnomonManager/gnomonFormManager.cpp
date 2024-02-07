#include "gnomonFormManager.h"

#include <QtGlobal>

#if (defined (Q_OS_WIN))
#include <windows.h>
#include <psapi.h>
#elif (defined (Q_OS_LINUX))
#include "sys/types.h"
#include "sys/sysinfo.h"
#include "stdlib.h"
#include "stdio.h"
#include <iostream>
#include <fstream>
#include <string>
#elif (defined (Q_OS_MAC))
#include <sys/types.h>
#include <sys/sysctl.h>
#include <mach/vm_statistics.h>
#include <mach/mach_types.h>
#include <mach/mach_init.h>
#include <mach/mach_host.h>
#include <mach/task.h>
#endif

#include <memory>
#include <utility>

#include "gnomonVisualizations/gnomonAbstractVtkVisualization.h"
#include "gnomonVisualizations/gnomonAbstractMplVisualization.h"
#include "gnomonVisualizations/gnomonAbstractQmlVisualization.h"


#include "gnomonView/gnomonVtkView.h"
#include "gnomonView/gnomonQmlView.h"
#include "gnomonView/gnomonMplView.h"

#include <gnomonPipeline/gnomonPipelineManager.h>
#include <gnomonPipeline/gnomonPipelineNodeReader.h>

#include <gnomonCore/gnomonCommand/gnomonBinaryImage/gnomonBinaryImageWriterCommand>
#include <gnomonCore/gnomonCommand/gnomonCellComplex/gnomonCellComplexWriterCommand>
#include <gnomonCore/gnomonCommand/gnomonCellImage/gnomonCellImageWriterCommand>
#include <gnomonCore/gnomonCommand/gnomonDataDict/gnomonDataDictWriterCommand>
#include <gnomonCore/gnomonCommand/gnomonDataFrame/gnomonDataFrameWriterCommand>
#include <gnomonCore/gnomonCommand/gnomonImage/gnomonImageWriterCommand>
#include <gnomonCore/gnomonCommand/gnomonLString/gnomonLStringWriterCommand>
#include <gnomonCore/gnomonCommand/gnomonMesh/gnomonMeshWriterCommand>
#include <gnomonCore/gnomonCommand/gnomonPointCloud/gnomonPointCloudWriterCommand>
#include <gnomonCore/gnomonCommand/gnomonTree/gnomonTreeWriterCommand>

#include <gnomonCore/gnomonCommand/gnomonBinaryImage/gnomonBinaryImageReaderCommand>
#include <gnomonCore/gnomonCommand/gnomonCellComplex/gnomonCellComplexReaderCommand>
#include <gnomonCore/gnomonCommand/gnomonCellImage/gnomonCellImageReaderCommand>
#include <gnomonCore/gnomonCommand/gnomonDataDict/gnomonDataDictReaderCommand>
#include <gnomonCore/gnomonCommand/gnomonDataFrame/gnomonDataFrameReaderCommand>
#include <gnomonCore/gnomonCommand/gnomonImage/gnomonImageReaderCommand>
#include <gnomonCore/gnomonCommand/gnomonLString/gnomonLStringReaderCommand>
#include <gnomonCore/gnomonCommand/gnomonMesh/gnomonMeshReaderCommand>
#include <gnomonCore/gnomonCommand/gnomonPointCloud/gnomonPointCloudReaderCommand>
#include <gnomonCore/gnomonCommand/gnomonTree/gnomonTreeReaderCommand>

#include <gnomonProject>

#include <vtkCamera.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkRenderWindowInteractor.h>

class gnomonFormManagerPrivate : public QObject
{
    Q_OBJECT

public:
     gnomonFormManagerPrivate(gnomonFormManager *q_ptr, QObject *parent = nullptr);
    ~gnomonFormManagerPrivate(void);

public:
    void insertForm(int item, const QString& form_uuid, const QImage& image);
    void addFormWriter(const QString& form_name, int item);

public:
    QHash<int, QString> forms;
    QHash<int, std::shared_ptr<gnomonAbstractVisualization> > formVisualizations;
    QHash<int, gnomonAbstractWriterCommand *> formWriterCommand;
    QHash<int, gnomonAbstractReaderCommand *> formReaderCommand;
    QHash<int, QImage> formThumbnail;
    QHash<int, vtkCamera *> formCameras;
    QHash<QString, gnomonAbstractWriterCommand *> commands;
    QHash<QString, int> formCounter;
    QHash<int, bool> formDropped;

public:
    gnomonAbstractView *view = nullptr;
    QTemporaryDir *tmpDir = nullptr;

public:
    static int item_counter;
    QMap<int, QString> cache_forms;
    QMap<int, std::pair<QString, gnomonPipelineNodeReader *> > cache_pipeline_nodes;
    QMap<int, QJsonObject > cache_metadatas;

    QSet<QString> hibernating_workspaces;
    QList<QString> active_workspaces;
    QTimer *memoryManagementTimer = nullptr;


public:
    QMetaObject::Connection connection;

public:
    gnomonFormManager* q = nullptr;

public:
    bool deleteFormFromMemory(int id);
    void loadFormToMemory(int id);

private slots:
    void startMemoryTimer();

public:
    gnomonAbstractView *createView(const QString& className, const QString& form_type, int figure_number=-1);
};

// ///////////////////////////////////////////////////////////////////
// gnomonFormManagerPrivate
// ///////////////////////////////////////////////////////////////////

int gnomonFormManagerPrivate::item_counter = 0;

gnomonFormManagerPrivate::gnomonFormManagerPrivate(gnomonFormManager *q_ptr, QObject *parent) : QObject(parent), q(q_ptr)
{
    memoryManagementTimer = new QTimer(this);
    QMetaObject::invokeMethod(this, &gnomonFormManagerPrivate::startMemoryTimer, Qt::QueuedConnection);
}

gnomonFormManagerPrivate::~gnomonFormManagerPrivate(void)
{
    delete memoryManagementTimer;
    memoryManagementTimer = nullptr;
    for (auto command: this->commands)
        delete command;
    this->commands.clear();

    if(tmpDir)
        tmpDir->remove();
    delete tmpDir;

    this->cache_pipeline_nodes.clear();
    q = nullptr;
}

void gnomonFormManagerPrivate::startMemoryTimer() {
    memoryManagementTimer->setInterval(1000);
    connect(memoryManagementTimer, &QTimer::timeout, q, &gnomonFormManager::memoryManagement);
    memoryManagementTimer->start();
}

void gnomonFormManagerPrivate::insertForm(int item, const QString& form_uuid, const QImage& image)
{
    GNOMON_SESSION->getForm(form_uuid)->setThumbnailId(item);
    this->forms.insert(item, form_uuid);
    this->formThumbnail.insert(item, image);
    this->formDropped.insert(item, false);

    gnomonPipelineManager::instance()->setFormIndex(form_uuid, item);

    QString form_name = GNOMON_SESSION->getForm(form_uuid)->formName();
    this->addFormWriter(form_name, item);
    if (!this->formCounter.contains(form_name)) {
        this->formCounter.insert(form_name, 0);
    }
    this->formCounter[form_name]++;
}

bool gnomonFormManagerPrivate::deleteFormFromMemory(int id)
{
    if(this->cache_forms.contains(id)) {
        this->forms[id] = nullptr;
        // TOCHECK
        //Do we decide to clear the visu here, which means that
        // the visu won't work in the workspace that created this form ?
        // If so, we need to add the mecanismto re-create the visu when needed
        // from it's parameters.
        // ------

        //if (this->formVisualizations.contains(id))
        //{
        //    this->formVisualizations[id]->disconnect();
        //    this->formVisualizations[id]->clearConnections();
        //    this->formVisualizations[id]->clear();
        //    this->formVisualizations[id] = nullptr;
        //}
        //else if (this->formMplVisualizations.contains(id))
        //{
        //    this->formMplVisualizations[id] = nullptr;
        //}
        //this->formThumbnail[id] = QImage(); TODO "griser"
        return true;
    }

    return false;
}

void gnomonFormManagerPrivate::loadFormToMemory(int id)
{
    QString reader_plugin;

    //get reader command name
    QString writer_command_name = this->formWriterCommand[id]->metaObject()->className();
    QString reader_command_name = writer_command_name.replace("WriterCommand", "ReaderCommand");

    //create reader command from name using metatype
    auto reader_metatype = QMetaType::fromName(reader_command_name.toUtf8());
    if(reader_metatype.isValid()) {
        this->formReaderCommand[id] = static_cast<gnomonAbstractReaderCommand *>(reader_metatype.create());
        reader_plugin = this->formReaderCommand[id]->availablePlugins()[0];
    } else {
        qWarning() << "No reader Command found for form " << reader_command_name;
        qWarning() << "cannot create reader to read file from cache!";
        qWarning() << "file " << this->cache_forms[id];
        return;
    }

    gnomonAbstractReaderCommand *readerCommand = this->formReaderCommand[id];
    QString path = this->cache_forms.take(id);
    readerCommand->setAlgorithmName(reader_plugin);
    readerCommand->setPath(path);
    QString source = QUrl(path).fileName();
    readerCommand->setSource(source);

    connect(readerCommand, &gnomonAbstractCommand::finished, [=]() {
            this->forms[id] = readerCommand->outputs().first()->uuid();
            GNOMON_SESSION->getForm(this->forms[id])->setThumbnailId(id);
            GNOMON_SESSION->getForm(this->forms[id])->metadata()->deserialize(this->cache_metadatas.take(id));
            QString form_name = GNOMON_SESSION->getForm(this->forms[id])->formName();
            gnomonPipelineManager::instance()->decacheNode(GNOMON_SESSION->getForm(this->forms[id])->uuid(), this->cache_pipeline_nodes.take(id));
    });
    readerCommand->redo();
}

void gnomonFormManagerPrivate::addFormWriter(const QString& form_name, int item)
{
    gnomonAbstractWriterCommand *command = nullptr;
    QString writer_plugin;

    QString writer_command_name = form_name + "WriterCommand";
    if(!this->commands.contains(form_name)) {
        auto writer_metatype = QMetaType::fromName(writer_command_name.toUtf8());
        if(writer_metatype.isValid()) {
            command = static_cast<gnomonAbstractWriterCommand *>(writer_metatype.create());
            this->commands.insert(form_name, command);
            writer_plugin = command->availablePlugins()[0];
        } else {
            qWarning() << Q_FUNC_INFO << "No writer Command found for form " << form_name;
            return;
        }
    }

    this->formWriterCommand[item] = this->commands[form_name];
    this->formWriterCommand[item]->setAlgorithmName(writer_plugin);
}

gnomonAbstractView *gnomonFormManagerPrivate::createView(const QString& className, const QString& form_type, int figure_number)
{
    gnomonAbstractView* view = nullptr;
    if(className == "gnomonVtkView") {
        view = new gnomonVtkView(this);
        view->setAcceptForm(form_type, true);
        vtkNew<vtkRenderWindow> temp_render_window;
        temp_render_window->SetOffScreenRendering(true);
        vtkNew<vtkRenderWindowInteractor> temp_render_window_interactor;
        temp_render_window_interactor->SetRenderWindow(temp_render_window);
        dynamic_cast<gnomonVtkView*>(view)->associate(temp_render_window);
    } else if (className == "gnomonQmlView") {
        view = new gnomonQmlView(this);
        view->setAcceptForm(form_type, true);
    } else if (className == "gnomonMplView") {
        view = new gnomonMplView(this);
        view->setAcceptForm(form_type, true);
        dynamic_cast<gnomonMplView*>(view)->setFigureNumber(figure_number);
    }
    return view;
}

// ///////////////////////////////////////////////////////////////////
// gnomonFormManager
// ///////////////////////////////////////////////////////////////////

gnomonFormManager::gnomonFormManager(QObject *parent) : QObject(parent)
{
    d = new gnomonFormManagerPrivate(this, this);

}

gnomonFormManager::~gnomonFormManager(void)
{
    delete d;
}

bool gnomonFormManager::deleteForm(int id, bool force)
{
    if(force) {
        d->formDropped[id] = false; // usefull when we do a reset!
    }
    if (!d->forms.contains(id) || d->formDropped[id]) {
        dtkWarn() << "Unknown forms id or form already dropped in other workspace" << id << "can't delete it ";
        return false;
    }
    if(gnomonPipelineManager::instance()->removeForm(GNOMON_SESSION->getForm(d->forms[id])->uuid())) {
        d->forms.remove(id);
        if (d->formCameras.contains(id)) {
            d->formCameras.remove(id);
        }
        d->formVisualizations.remove(id);
        d->formThumbnail.remove(id);
        d->formWriterCommand.remove(id);
        d->formDropped.remove(id);
        d->item_counter--;
        emit removed(id);
        return true;
    }
    return false;
}

void gnomonFormManager::compose(int first, int second) {
    auto output = GNOMON_SESSION->getForm(d->forms[first])->clone();
    auto form2 = GNOMON_SESSION->getForm(d->forms[second]);
    output->compose(form2);

    QMap<QString, QString> inputs = {
            {"first", GNOMON_SESSION->getForm(d->forms[first])->uuid()},
            {"second", GNOMON_SESSION->getForm(d->forms[second])->uuid()},
    };
    QMap<QString, QString> outputs = {
            {"output", output->uuid()},
    };

    GNOMON_SESSION->addForm(output);

    this->addForm(output->uuid(), d->formThumbnail[first]);

    gnomonPipelineManager::instance()->addTask("compose", inputs, outputs);
    gnomonPipelineManager::instance()->addForm(output->uuid());
}

void gnomonFormManager::saveAs(int id, const QString& f, bool add_to_pipeline) const
{
    QString file_name = f;
    QUrl url(file_name);
    if (url.isLocalFile()){
        file_name = url.toLocalFile();

        //check if there is an extension, if not add one
        QFileInfo f_info(file_name);
        if(f_info.completeSuffix().isEmpty()) {
            dtkWarn() << "No extension provided on : " << file_name << " I will add " << d->formWriterCommand[id]->extensions()[0];
            file_name += "." + d->formWriterCommand[id]->extensions()[0];
        }
    }

    if(!file_name.isEmpty()) {
        auto command = d->formWriterCommand[id];
        command->setPath(file_name);
        command->setForm(GNOMON_SESSION->getForm(d->forms[id]));
        command->redo();
        if (add_to_pipeline)
        {
            gnomonPipelineManager::instance()->addWriter(command);
        }
    }
}

void gnomonFormManager::addToCache(int id) const
{
    gnomonAbstractWriterCommand *writer_command = d->formWriterCommand[id];
    QStringList extensions = writer_command->extensions();
    QString f = QString::number(id) + "." + extensions[0];
    if(!d->tmpDir)
        d->tmpDir = new QTemporaryDir(".GNOMON_TEMP");

    auto filepath = d->tmpDir->filePath(f);
    this->saveAs(id, filepath, false);

    d->cache_forms[id] = filepath;
    d->cache_pipeline_nodes[id] = gnomonPipelineManager::instance()->cacheNode(GNOMON_SESSION->getForm(d->forms[id])->uuid());
    d->cache_metadatas[id] = GNOMON_SESSION->getForm(d->forms[id])->metadata()->serialize();
    d->deleteFormFromMemory(id);
}

void gnomonFormManager::loadFromCache(int id) const
{
    d->loadFormToMemory(id);
}
// ///////////////////////////////////////////////////////////////////
// gnomonFormManager
// ///////////////////////////////////////////////////////////////////

gnomonFormManager *gnomonFormManager::instance(void)
{
    static gnomonFormManager s_instance;
    return &s_instance;
}

void gnomonFormManager::addForm(const QString& form_uuid, const QImage& image, std::shared_ptr<gnomonAbstractVisualization> visualization)
{
    if (!d->forms.values().contains(form_uuid)) {
        int item = d->item_counter++;
        d->insertForm(item, form_uuid, image);
        d->formVisualizations.insert(item, visualization);
        emit added(item, GNOMON_SESSION->getForm(form_uuid)->formName());
    } else {
        emit alreadyAdded();
    }
}

QString gnomonFormManager::get(int index)
{
    return d->forms.value(index);
}

int gnomonFormManager::formIndex(const QString& form_uuid)
{
    if (d->forms.values().contains(form_uuid)) {
        return d->forms.values().indexOf(form_uuid);
    } else {
        return -1;
    }
}

std::shared_ptr<gnomonAbstractVisualization> gnomonFormManager::getVisualization(int index)
{
    return d->formVisualizations.value(index, nullptr);
}

void gnomonFormManager::setCamera(int index, vtkCamera *cam)
{
    d->formCameras.insert(index, cam);
}

vtkCamera *gnomonFormManager::getCamera(int index)
{
    return d->formCameras.value(index, nullptr);
}

QImage gnomonFormManager::thumbnail(int index)
{
    return d->formThumbnail.value(index, QImage());
}

gnomonDynamicFormMetadata *gnomonFormManager::getDynamicFormMetadata(int id) {
    if(contains(id)) {
        gnomonDynamicFormMetadata *metadata = GNOMON_SESSION->getForm(d->forms[id])->metadata();
        metadata->set("uuid", d->forms[id]);
        QQmlEngine::setObjectOwnership(metadata, QQmlEngine::CppOwnership);
        return metadata;
    } else {
        qWarning() << Q_FUNC_INFO << "doesnt contain id: " << id << " ids: " << d->forms.keys();
        return nullptr;
    }
}

bool gnomonFormManager::contains(int id) {
    return d->forms.contains(id);
}

QVariantList gnomonFormManager::timeKeys(int id) {
    if(contains(id)) {
        const auto& times = GNOMON_SESSION->getForm(d->forms[id])->times();
        QVariantList out;
        for(double time : times) {
            out.append(time);
        }
        return out;
    } else {
        return {};
    }
}

QStringList gnomonFormManager::formMetadataKeysAtT(int id, double t) {
    if(contains(id) && GNOMON_SESSION->getForm(d->forms[id])->times().contains(t)) {
        return GNOMON_SESSION->getForm(d->forms[id])->metadataAtT(t).keys();
    } else {
        qWarning() << Q_FUNC_INFO << "Cannot get metadata for form " << id << " at time " << t;
        if(contains(id))
            qWarning() << Q_FUNC_INFO << "available times: " << GNOMON_SESSION->getForm(d->forms[id])->times();
        return {};
    }
}

QString gnomonFormManager::formMetadataValueAtT(int id, double t, const QString& key) {
    if(contains(id)){
        auto metadata = GNOMON_SESSION->getForm(d->forms[id])->metadataAtT(t);
        if(metadata.contains(key)) {
            return metadata[key];
        }
    }
    return {};
}

QString gnomonFormManager::formWriterNameFilter(int id)
{
    QString filter;
    if (this->contains(id)) {
        std::shared_ptr<gnomonAbstractDynamicForm> form = GNOMON_SESSION->getForm(d->forms[id]);
        gnomonAbstractWriterCommand* writer_command = d->formWriterCommand[id];
        QStringList extensions = writer_command->extensions();
        filter += form->formName().remove("gnomon");
        filter += " files (";
        for (const auto& ext : extensions) {
            filter += "*." + ext + " ";
        }
        filter += ")";
    }
    return filter;
}

int gnomonFormManager::formCount(const QString& form_name)
{
    if (!d->formCounter.contains(form_name)) {
        return 0;
    } else {
        return  d->formCounter[form_name];
    }
}

void gnomonFormManager::setFormDropped(const QString& form_uuid)
{
    //once dropped, a form cannot be deleted anymore. Otherwise, it will cause
    // onconsistency in the pipeline
    int index = d->forms.key(form_uuid);
    d->formDropped[index] = true;
}

QList<int> gnomonFormManager::systemStat(void) const
{
    QList<int> stat(3); //total_mem, used_mem, this_mem
#if (defined (Q_OS_WIN))
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof (statex);
    GlobalMemoryStatusEx (&statex);

    stat[0] = statex.ullTotalPhys / 1024);
    stat[1] = (statex.ullTotalPhys - statex.ullAvailPhy) / (1024 );
    
    PROCESS_MEMORY_COUNTERS_EX pmc;
    GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
    stat[2] = pmc.WorkingSetSize/ (1024 );
    // virtualMemUsedByMe = pmc.PrivateUsage;
#elif (defined (Q_OS_LINUX))
    struct sysinfo memInfo;
    sysinfo (&memInfo);
    stat[0] = memInfo.totalram * memInfo.mem_unit / (1000*1000);
    std::ifstream memfile("/proc/meminfo");
    if(memfile.is_open()) {
        std::string line;
        while(std::getline(memfile, line)) {
            if(line.find("MemAvailable:") != std::string::npos) {
                auto start = line.find_first_of("123456789");
                auto stop = line.find_last_of("1234567890");
                auto available_mem = std::stol(line.substr(start, stop-start+1));
                stat[1] = (memInfo.totalram/1000 - available_mem)/1000;
                break;
            }
        }
    }
    std::ifstream procfile("/proc/self/smaps_rollup");
    if(procfile.is_open()) {
        std::string line;
        while(std::getline(procfile, line)) {
            if(line.find("Rss:") != std::string::npos) {
                auto start = line.find_first_of("123456789");
                auto stop = line.find_last_of("1234567890");
                stat[2] = std::stol(line.substr(start, stop-start+1)) / 1000;
                break;
            }
        }
    }
#elif (defined (Q_OS_MAC))
    int mib[2]; 
    int64_t total_memory;
    mib[0] = CTL_HW;  mib[1] = HW_MEMSIZE;
    size_t length = sizeof(int64_t);
    sysctl(mib, 2, &total_memory, &length, NULL, 0);
    stat[0] = total_memory / (1024*1024);

    //free mem and used mem
    vm_size_t page_size;
    mach_port_t mach_port;
    mach_msg_type_number_t count;
    vm_statistics64_data_t vm_stats;

    mach_port = mach_host_self();
    count = sizeof(vm_stats) / sizeof(natural_t);
    if (KERN_SUCCESS == host_page_size(mach_port, &page_size) &&
        KERN_SUCCESS == host_statistics64(mach_port, HOST_VM_INFO,
                                        (host_info64_t)&vm_stats, &count))
    {
        //long long free_memory = (int64_t)vm_stats.free_count * (int64_t)page_size;

        stat[1] = (((int64_t)vm_stats.active_count +
                    (int64_t)vm_stats.inactive_count +
                    (int64_t)vm_stats.wire_count) *  (int64_t)page_size ) / (1024*1024);
    }

    struct task_basic_info t_info;
    mach_msg_type_number_t t_info_count = TASK_BASIC_INFO_COUNT;

    if (KERN_SUCCESS != task_info(mach_task_self(),
                                  TASK_BASIC_INFO, (task_info_t)&t_info,
                                  &t_info_count))
    {
        qDebug() << "macos cannot get this_used_mem";
        stat[2] = 0; 
    } else {
        stat[2] = t_info.resident_size / (1024*1024);
    }
#endif
    return stat;
}

QJsonObject gnomonFormManager::serialize(void)
{
    QJsonObject state;
    QJsonObject forms;
    for( auto [id, form]: d->forms.asKeyValueRange()) {
        forms[QString::number(id)] = form;
    }
    state["forms"] = forms;

    QJsonObject form_visualizations;
    for( auto [id, visualization]: d->formVisualizations.asKeyValueRange()) {
        QVariantMap visu_info;
        visu_info["form_type"] = GNOMON_SESSION->getForm(d->forms[id])->formName();
        QString visu_type;
        QString figure_number;
        if(auto vtk_visu = std::dynamic_pointer_cast<gnomonAbstractVtkVisualization>(visualization)) {
            visu_type = "gnomonVtkView";
        } else if (auto qml_visu = std::dynamic_pointer_cast<gnomonAbstractQmlVisualization>(visualization)) {
            visu_type = "gnomonQmlView";
        } else if (auto mpl_visu = std::dynamic_pointer_cast<gnomonAbstractMplVisualization>(visualization)) {
            visu_type = "gnomonMplView";
            auto mpl_view = dynamic_cast<gnomonMplView *>(mpl_visu->view());
            if (mpl_view) {
                figure_number = QString::number(mpl_view->figureNumber());
            }
        }
        visu_info["visu_type"] = visu_type;
        visu_info["figure_number"] = figure_number;
        visu_info["visu_name"] = visualization->pluginName();
        QVariantMap parameters;
        for(auto [k, v]: visualization->parameters().asKeyValueRange()) {
            parameters.insert(k, v->toVariantHash());
        }
        visu_info["parameters"] = parameters;

        form_visualizations.insert(QString::number(id), QJsonObject::fromVariantMap(visu_info));
    }
    state["form_visualizations"] = form_visualizations;

    QJsonObject form_cameras;
    for( auto [id, camera]: d->formCameras.asKeyValueRange()) {
        QVariantMap camera_info;
        QStringList position;
        QStringList focal_point;
        QStringList view_up;
        for(int i = 0; i < 3; i++) {
            position << QString::number(camera->GetPosition()[i]);
            focal_point << QString::number(camera->GetFocalPoint()[i]);
            view_up << QString::number(camera->GetViewUp()[i]);
        }

        camera_info["position"] = QVariant::fromValue(position);
        camera_info["focal_point"] = QVariant::fromValue(focal_point);
        camera_info["view_up"] = QVariant::fromValue(view_up);

        form_cameras[QString::number(id)] = QJsonObject::fromVariantMap(camera_info);
    }
    state["form_cameras"] = form_cameras;

    return state;
}

void gnomonFormManager::deserialize(const QJsonObject& state)
{
    auto forms = state["forms"].toObject().toVariantHash();
    auto form_visualizations = state["form_visualizations"].toObject().toVariantHash();

    d->forms.clear();
    d->formVisualizations.clear();
    d->formThumbnail.clear();

    QList<QString> ids = forms.keys();
    ids.sort();

    for(auto id : ids) {
        int index = id.toInt();
        QString uuid = forms[id].toString();
        auto visualization = form_visualizations[id];
        QString form_type = visualization.toMap()["form_type"].toString();
        QString visu_type = visualization.toMap()["visu_type"].toString();
        int figure_number = visualization.toMap()["figure_number"].toString().toInt();
        QString visu_name = visualization.toMap()["visu_name"].toString();
        auto parameters = visualization.toMap()["parameters"].toMap();
        gnomonAbstractView *view = d->createView(visu_type, form_type, figure_number);
        std::shared_ptr<gnomonAbstractVisualization> visu = nullptr;
        QImage image(1500, 1500, QImage::Format_RGB32);
        image.fill(Qt::GlobalColor::black);
        if (view) {
            view->setForm(uuid, form_type, visu_name, parameters);
            visu = view->getVisualization(form_type);
            image = view->getVisualization(form_type)->imageRendering();

            view->clear();
            delete view;
        }

        d->insertForm(index, uuid, image);
        d->formVisualizations.insert(index, visu);
        emit added(index, form_type);
        d->item_counter++;
    }

    d->formCameras.clear();
    auto form_cameras = state["form_cameras"].toObject().toVariantMap();
    for(auto [id, camera]: form_cameras.asKeyValueRange()) {
        int index = id.toInt();
        auto position = camera.toMap()["position"].toStringList();
        auto focal_point = camera.toMap()["focal_point"].toStringList();
        auto view_up = camera.toMap()["view_up"].toStringList();
        if(position.size()==3 && focal_point.size()==3 && view_up.size()==3) {
            double position_val[3], focal_point_val[3], view_up_val[3];

            for(int i = 0; i < 3; i++) {
                position_val[i] = position[i].toDouble();
                focal_point_val[i] = focal_point[i].toDouble();
                view_up_val[i] = view_up[i].toDouble();
            }

            vtkSmartPointer<vtkCamera> cam = vtkCamera::New();
            cam->SetPosition(position_val);
            cam->SetFocalPoint(focal_point_val);
            cam->SetViewUp(view_up_val);
            d->formCameras.insert(index, cam);
        }
    }
}

void gnomonFormManager::memoryManagement() {
    auto stats = systemStat();
    int total_mem = stats[0];
    int used_mem = stats[1];
    //qDebug() << "$$ Memory usage: " << used_mem << " | " << total_mem << " | " << stats[2];
    while((used_mem > 3000 || (float)used_mem/(float)total_mem>0.8) && d->active_workspaces.size()>1) {
        break;
        auto uuid =  d->active_workspaces.first();
        d->active_workspaces.pop_front();
        qInfo() << "Memory used threshold reached: hibernating workspaces (" << uuid << ")";
        emit requestHibernation(uuid);
        d->hibernating_workspaces.insert(uuid);
        for(auto [index, uuid]: d->forms.asKeyValueRange()) {
            auto form = GNOMON_SESSION->getForm(uuid);
            qDebug() << "Form " << uuid << " :: use count: " << form.use_count();
            if(form.use_count() <= 2) {
                //form->unload();
            }
        }
    }
}

void gnomonFormManager::testDeactivate(void) {
    if(d->active_workspaces.size()>1) {
        auto uuid =  d->active_workspaces.first();
        d->active_workspaces.pop_front();
        emit requestHibernation(uuid);
        d->hibernating_workspaces.insert(uuid);
    } else {
        qDebug() << "$$ cannot deactivate more workspaces";
    }
}

void gnomonFormManager::registerNewWorkspace(const QString &uuid) {
    d->active_workspaces.append(uuid);
}

void gnomonFormManager::registerWorkspaceWakeup(const QString &uuid) {
    if(d->hibernating_workspaces.contains(uuid)) {
        d->hibernating_workspaces.remove(uuid);
    }
    if(d->active_workspaces.contains(uuid)) {
        d->active_workspaces.removeAll(uuid);
    }
    d->active_workspaces.append(uuid);
}


#include "gnomonFormManager.moc"
//
// gnomonFormManager.cpp ends here
