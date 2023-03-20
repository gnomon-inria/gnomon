#include "gnomonFormManager.h"

#include <memory>
#include <utility>

#include "gnomonVisualizations/gnomonAbstractVtkVisualization.h"
#include "gnomonVisualizations/gnomonAbstractMplVisualization.h"


#include "gnomonView/gnomonVtkView.h"

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


#include <vtkCamera.h>
#include <vtkGenericOpenGLRenderWindow.h>


class gnomonFormManagerPrivate : public QObject
{
    Q_OBJECT

public:
     gnomonFormManagerPrivate(QObject *parent = nullptr);
    ~gnomonFormManagerPrivate(void);

public:
    void insertForm(int item, std::shared_ptr<gnomonAbstractDynamicForm> form, const QImage& image);
    void addFormWriter(const QString& form_name, int item);

public:
    QHash<int, std::shared_ptr<gnomonAbstractDynamicForm> > forms;
    QHash<int, std::shared_ptr<gnomonAbstractVtkVisualization> > formVisualizations;
    QHash<int, std::shared_ptr<gnomonAbstractMplVisualization> > formMplVisualizations;
    QHash<int, gnomonAbstractWriterCommand *> formWriterCommand;
    QHash<int, gnomonAbstractReaderCommand *> formReaderCommand;
    QHash<int, QImage> formThumbnail;
    QHash<int, vtkCamera *> formCameras;
    QHash<QString, gnomonAbstractWriterCommand *> commands;
    QHash<QString, int> formCounter;
    QHash<int, bool> formDropped;

public:
    gnomonVtkView *view = nullptr;
    QTemporaryDir *tmpDir = nullptr;

public:
    static int item_counter;
    QMap<int, QString> cache_forms;
    QMap<int, std::pair<QString, gnomonPipelineNodeReader *> > cache_pipeline_nodes;
    QMap<int, QJsonObject > cache_metadatas;


public:
    QMetaObject::Connection connection;

public:
    bool deleteFormFromMemory(int id);
    void loadFormToMemory(int id);
};

// ///////////////////////////////////////////////////////////////////
// gnomonFormManagerPrivate
// ///////////////////////////////////////////////////////////////////

int gnomonFormManagerPrivate::item_counter = 0;

gnomonFormManagerPrivate::gnomonFormManagerPrivate(QObject *parent) : QObject(parent)
{
    gnomonAbstractCommand::gui_thread = this->thread();
}

gnomonFormManagerPrivate::~gnomonFormManagerPrivate(void)
{
    for (auto command: this->commands)
        delete command;
    this->commands.clear();

    if(tmpDir)
        tmpDir->remove();
    delete tmpDir;

    this->cache_pipeline_nodes.clear();
}

void gnomonFormManagerPrivate::insertForm(int item, std::shared_ptr<gnomonAbstractDynamicForm> form, const QImage& image)
{
    form->setThumbnailId(item);
    this->forms.insert(item, form);
    this->formThumbnail.insert(item, image);
    this->formDropped.insert(item, false);

    gnomonPipelineManager::instance()->setFormIndex(form, item);

    QString form_name = form->formName();
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
    qDebug() << Q_FUNC_INFO << id;

    QString reader_plugin;

    if(dynamic_cast<gnomonBinaryImageWriterCommand *>(this->formWriterCommand[id]))
    {
        this->formReaderCommand[id] = new gnomonBinaryImageReaderCommand();
        reader_plugin = dynamic_cast<gnomonBinaryImageReaderCommand *>(this->formReaderCommand[id])->availablePlugins()[0];
    } else if(dynamic_cast<gnomonCellComplexWriterCommand *>(this->formWriterCommand[id]))
    {
        this->formReaderCommand[id] = new gnomonCellComplexReaderCommand();
        reader_plugin = dynamic_cast<gnomonCellComplexReaderCommand *>(this->formReaderCommand[id])->availablePlugins()[0];
    } else if(dynamic_cast<gnomonCellImageWriterCommand *>(this->formWriterCommand[id]))
    {
        this->formReaderCommand[id] = new gnomonCellImageReaderCommand();
        reader_plugin = dynamic_cast<gnomonCellImageReaderCommand *>(this->formReaderCommand[id])->availablePlugins()[0];
    } else if(dynamic_cast<gnomonDataDictWriterCommand *>(this->formWriterCommand[id]))
    {
        this->formReaderCommand[id] = new gnomonDataDictReaderCommand();
        reader_plugin = dynamic_cast<gnomonDataDictReaderCommand *>(this->formReaderCommand[id])->availablePlugins()[0];
    } else if(dynamic_cast<gnomonDataFrameWriterCommand *>(this->formWriterCommand[id]))
    {
        this->formReaderCommand[id] = new gnomonDataFrameReaderCommand();
        reader_plugin = dynamic_cast<gnomonDataFrameReaderCommand *>(this->formReaderCommand[id])->availablePlugins()[0];
    } else if(dynamic_cast<gnomonImageWriterCommand *>(this->formWriterCommand[id]))
    {
        this->formReaderCommand[id] = new gnomonImageReaderCommand();
        reader_plugin = dynamic_cast<gnomonImageReaderCommand *>(this->formReaderCommand[id])->availablePlugins()[0];
    } else if(dynamic_cast<gnomonLStringWriterCommand *>(this->formWriterCommand[id]))
    {
        this->formReaderCommand[id] = new gnomonLStringReaderCommand();
        reader_plugin = dynamic_cast<gnomonLStringReaderCommand *>(this->formReaderCommand[id])->availablePlugins()[0];
    } else if(dynamic_cast<gnomonMeshWriterCommand *>(this->formWriterCommand[id]))
    {
        this->formReaderCommand[id] = new gnomonMeshReaderCommand();
        reader_plugin = dynamic_cast<gnomonMeshReaderCommand *>(this->formReaderCommand[id])->availablePlugins()[0];
    } else if(dynamic_cast<gnomonPointCloudWriterCommand *>(this->formWriterCommand[id]))
    {
        this->formReaderCommand[id] = new gnomonPointCloudReaderCommand();
        reader_plugin = dynamic_cast<gnomonPointCloudReaderCommand *>(this->formReaderCommand[id])->availablePlugins()[0];
    } else if(dynamic_cast<gnomonTreeWriterCommand *>(this->formWriterCommand[id]))
    {
        this->formReaderCommand[id] = new gnomonTreeReaderCommand();
        reader_plugin = dynamic_cast<gnomonTreeReaderCommand *>(this->formReaderCommand[id])->availablePlugins()[0];
    }

    gnomonAbstractReaderCommand *readerCommand = this->formReaderCommand[id];
    if(!readerCommand) {
        qWarning() << "cannot create reader to read file from cache!";
        qWarning() << "file " << this->cache_forms[id];
        return;
    }
    QString path = this->cache_forms.take(id);
    readerCommand->setAlgorithmName(reader_plugin);
    readerCommand->setPath(path);
    QString source = QUrl(path).fileName();
    readerCommand->setSource(source);

    connect(readerCommand, &gnomonAbstractCommand::finished, [=]() {
            this->forms[id] = readerCommand->outputs().first();
            this->forms[id]->setThumbnailId(id);
            this->forms[id]->metadata()->deserialize(this->cache_metadatas.take(id));
            QString form_name = this->forms[id]->formName();
            gnomonPipelineManager::instance()->decachNode(this->forms[id], this->cache_pipeline_nodes.take(id));
    });
    readerCommand->redo();
}


void gnomonFormManagerPrivate::addFormWriter(const QString& form_name, int item)
{
    gnomonAbstractWriterCommand *command = nullptr;
    QString writer_plugin;

    if(form_name == "gnomonBinaryImage") {
        if (!this->commands.contains(form_name)) {
            this->commands.insert(form_name, new gnomonBinaryImageWriterCommand);
        }
        writer_plugin = dynamic_cast<gnomonBinaryImageWriterCommand *>(this->commands[form_name])->availablePlugins()[0];
    } else if(form_name == "gnomonCellComplex"){
        if (!this->commands.contains(form_name)) {
            this->commands.insert(form_name, new gnomonCellComplexWriterCommand);
        }
        writer_plugin = dynamic_cast<gnomonCellComplexWriterCommand *>(this->commands[form_name])->availablePlugins()[0];
    } else if (form_name == "gnomonCellImage") {
        if (!this->commands.contains(form_name)) {
            this->commands.insert(form_name, new gnomonCellImageWriterCommand);
        }
        writer_plugin = dynamic_cast<gnomonCellImageWriterCommand *>(this->commands[form_name])->availablePlugins()[0];
    } else if (form_name == "gnomonImage") {
        if (!this->commands.contains(form_name)) {
            this->commands.insert(form_name, new gnomonImageWriterCommand);
        }
        writer_plugin = dynamic_cast<gnomonImageWriterCommand *>(this->commands[form_name])->availablePlugins()[0];
    } else if (form_name == "gnomonLString") {
        if (!this->commands.contains(form_name)) {
            this->commands.insert(form_name, new gnomonLStringWriterCommand);
        }
        writer_plugin = dynamic_cast<gnomonLStringWriterCommand *>(this->commands[form_name])->availablePlugins()[0];
    } else if (form_name == "gnomonMesh") {
        if (!this->commands.contains(form_name)) {
            this->commands.insert(form_name, new gnomonMeshWriterCommand);
        }
        writer_plugin = dynamic_cast<gnomonMeshWriterCommand *>(this->commands[form_name])->availablePlugins()[0];
    } else if (form_name == "gnomonPointCloud") {
        if (!this->commands.contains(form_name)) {
            this->commands.insert(form_name, new gnomonPointCloudWriterCommand);
        }
        writer_plugin = dynamic_cast<gnomonPointCloudWriterCommand *>(this->commands[form_name])->availablePlugins()[0];
    } else if (form_name == "gnomonDataFrame") {
        if (!this->commands.contains(form_name)) {
            this->commands.insert(form_name, new gnomonDataFrameWriterCommand);
        }
        writer_plugin = dynamic_cast<gnomonDataFrameWriterCommand *>(this->commands[form_name])->availablePlugins()[0];
    } else if (form_name == "gnomonTree") {
        if (!this->commands.contains(form_name)) {
            this->commands.insert(form_name, new gnomonTreeWriterCommand);
        }
        writer_plugin = dynamic_cast<gnomonTreeWriterCommand *>(this->commands[form_name])->availablePlugins()[0];
    } else if (form_name == "gnomonDataDict") {
        if(!this->commands.contains(form_name)) {
            this->commands.insert(form_name, new gnomonDataDictWriterCommand);
        }
        writer_plugin = dynamic_cast<gnomonDataDictWriterCommand *>(this->commands[form_name])->availablePlugins()[0];
    } else {
        qWarning() << Q_FUNC_INFO << "No writer found for form " << form_name;
        return;
    }

    this->formWriterCommand[item] = this->commands[form_name];
    this->formWriterCommand[item]->setAlgorithmName(writer_plugin);
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
    if(gnomonPipelineManager::instance()->removeForm(d->forms[id])) {
        d->forms.remove(id);
        if (d->formCameras.contains(id)) {
            d->formCameras.remove(id);
        }
        if (d->formVisualizations.contains(id)) {
            d->formVisualizations.remove(id);
        } else if (d->formMplVisualizations.contains(id)) {
            d->formMplVisualizations.remove(id);
        }
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
    auto output = d->forms[first]->clone();
    auto form2 = d->forms[second];
    output->compose(form2);

    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm>> inputs = {
            {"first", d->forms[first]},
            {"second", d->forms[second]},
    };
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm>> outputs = {
            {"output",output},
    };
    gnomonPipelineManager::instance()->addTask("compose", inputs, outputs);
    gnomonPipelineManager::instance()->addForm(output);

    this->addForm(output, {}, d->formThumbnail[first]);
}

void gnomonFormManager::saveAs(int id, const QString& f, bool add_to_pipeline) const
{
    QString file_name = f;
    QUrl url(file_name);
    if (url.isLocalFile()){
        file_name = url.toLocalFile();
    }

    if(!file_name.isEmpty()) {
        auto command = d->formWriterCommand[id];
        command->setPath(file_name);
        command->setForm(d->forms[id]);
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
    d->cache_pipeline_nodes[id] = gnomonPipelineManager::instance()->cacheNode(d->forms[id]);
    d->cache_metadatas[id] = d->forms[id]->metadata()->serialize();
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

void gnomonFormManager::addForm(std::shared_ptr<gnomonAbstractDynamicForm> form,  std::shared_ptr<gnomonAbstractVtkVisualization> visualization, const QImage& image,  vtkCamera *cam)
{
    if (!d->forms.values().contains(form)) {
        int item = d->item_counter++;
        d->insertForm(item, form, image);
        d->formVisualizations.insert(item, visualization);
        d->formCameras.insert(item, cam);
        emit added(item, form->formName());
    } else {
        emit alreadyAdded();
    }
}

void gnomonFormManager::addForm(std::shared_ptr<gnomonAbstractDynamicForm> form, std::shared_ptr<gnomonAbstractMplVisualization> visualization)
{
    if (!d->forms.values().contains(form)) {
        int item = d->item_counter++;
        QImage image = visualization->imageRendering();
        form->metadata()->moveToThread(QThread::currentThread());
        d->insertForm(item, form, image);
        d->formMplVisualizations.insert(item, visualization);
        emit added(item, form->formName());
    } else {
        emit alreadyAdded();
    }
}


void gnomonFormManager::addForm(std::shared_ptr<gnomonAbstractDynamicForm> form, const QImage& image)
{
    if (!d->forms.values().contains(form)) {
        int item = d->item_counter++;
        form->metadata()->moveToThread(QThread::currentThread());
        d->insertForm(item, form, image);
        d->formMplVisualizations.insert(item, nullptr);
        emit added(item, form->formName());
    } else {
        emit alreadyAdded();
    }
}

std::shared_ptr<gnomonAbstractDynamicForm> gnomonFormManager::get(int index)
{
    return d->forms.value(index, nullptr);
}

std::shared_ptr<gnomonAbstractVtkVisualization> gnomonFormManager::getVisualization(int index)
{
    return d->formVisualizations.value(index, nullptr);
}

vtkCamera *gnomonFormManager::getCamera(int index)
{
    return d->formCameras.value(index, nullptr);
}

QImage gnomonFormManager::thumbnail(int index)
{
    return d->formThumbnail.value(index, QImage());
}

gnomonFormManager::gnomonFormManager(QObject *parent) : QObject(parent)
{
    d = new gnomonFormManagerPrivate;
}

gnomonFormManager::~gnomonFormManager(void)
{
    delete d;
}

gnomonDynamicFormMetadata *gnomonFormManager::getDynamicFormMetadata(int id) {
    if(contains(id)) {
        gnomonDynamicFormMetadata *metadata = d->forms[id]->metadata();
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
        const auto& times = d->forms[id]->times();
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
    if(contains(id) && d->forms[id]->times().contains(t)) {
        return d->forms[id]->metadataAtT(t).keys();
    } else {
        qWarning() << Q_FUNC_INFO << "Cannot get metadata for form " << id << " at time " << t;
        if(contains(id))
            qWarning() << Q_FUNC_INFO << "available times: " << d->forms[id]->times();
        return {};
    }
}

QString gnomonFormManager::formMetadataValueAtT(int id, double t, const QString& key) {
    if(contains(id)){
        auto metadata = d->forms[id]->metadataAtT(t);
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
        std::shared_ptr<gnomonAbstractDynamicForm> form = d->forms[id];
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

void gnomonFormManager::setFormDropped(std::shared_ptr<gnomonAbstractDynamicForm> form)
{
    //once dropped, a form cannot be deleted anymore. Otherwise, it will cause
    // onconsistency in the pipeline
    int index = d->forms.key(form);
    d->formDropped[index] = true;
}

#include "gnomonFormManager.moc"
//
// gnomonFormManager.cpp ends here
