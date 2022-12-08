#include "gnomonFormManager.h"

#include <memory> 

#include "gnomonVisualizations/gnomonAbstractVisualization.h"
#include "gnomonVisualizations/gnomonAbstractMatplotlibVisualization.h"


#include "gnomonView/gnomonViewForm.h"

#include <gnomonPipeline/gnomonPipelineManager.h>

#include <gnomonCore/gnomonCommand/gnomonBinaryImage/gnomonBinaryImageWriterCommand>
#include <gnomonCore/gnomonCommand/gnomonCellImage/gnomonCellImageWriterCommand>
#include <gnomonCore/gnomonCommand/gnomonCellComplex/gnomonCellComplexWriterCommand>
#include <gnomonCore/gnomonCommand/gnomonDataDict/gnomonDataDictWriterCommand>
#include <gnomonCore/gnomonCommand/gnomonDataFrame/gnomonDataFrameWriterCommand>
#include <gnomonCore/gnomonCommand/gnomonImage/gnomonImageWriterCommand>
#include <gnomonCore/gnomonCommand/gnomonLString/gnomonLStringWriterCommand>
#include <gnomonCore/gnomonCommand/gnomonMesh/gnomonMeshWriterCommand>
#include <gnomonCore/gnomonCommand/gnomonPointCloud/gnomonPointCloudWriterCommand>
#include <gnomonCore/gnomonCommand/gnomonTree/gnomonTreeWriterCommand>

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
    QHash<int, std::shared_ptr<gnomonAbstractVisualization> > formVisualizations;
    QHash<int, std::shared_ptr<gnomonAbstractMatplotlibVisualization> > formMatplotlibVisualizations;
    QHash<int, gnomonAbstractWriterCommand *> formWriterCommand;
    QHash<int, QImage> formData;
    QHash<int, vtkCamera *> formCameras;
    QHash<QString, gnomonAbstractWriterCommand *> commands;
    QHash<QString, int> formCounter;
    QHash<int, bool> formDropped;

    // TODO: to remove when destruction of visualizations will not cause a crash
    QList<std::shared_ptr<gnomonAbstractVisualization> > removedVisualizations;

public:
    gnomonViewForm *view = nullptr;

public:
    static int item_counter;

public:
    QMetaObject::Connection connection;
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
}

void gnomonFormManagerPrivate::insertForm(int item, std::shared_ptr<gnomonAbstractDynamicForm> form, const QImage& image)
{
    this->forms.insert(item, form);
    this->formData.insert(item, image);
    this->formDropped.insert(item, false);

    gnomonPipelineManager::instance()->setFormIndex(form, item);

    QString form_name = form->formName();
    this->addFormWriter(form_name, item);
    if (!this->formCounter.contains(form_name)) {
        this->formCounter.insert(form_name, 0);
    }
    this->formCounter[form_name]++;
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
            // TODO: to remove when destruction of visualizations will not cause a crash
            d->removedVisualizations.append(d->formVisualizations[id]);
            d->formVisualizations.remove(id);
        } else if (d->formMatplotlibVisualizations.contains(id)) {
            d->formMatplotlibVisualizations.remove(id);
        }
        d->formData.remove(id);
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

    this->addForm(output, {}, d->formData[first]);
}

void gnomonFormManager::saveAs(int id, const QString& f) const
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
        gnomonPipelineManager::instance()->addWriter(command);
    }
}

// ///////////////////////////////////////////////////////////////////
// gnomonFormManager
// ///////////////////////////////////////////////////////////////////

gnomonFormManager *gnomonFormManager::instance(void)
{
    if(!s_instance) {
        s_instance = new gnomonFormManager;
    }
    return s_instance;
}

void gnomonFormManager::addForm(std::shared_ptr<gnomonAbstractDynamicForm> form,  std::shared_ptr<gnomonAbstractVisualization> visualization, const QImage& image,  vtkCamera *cam)
{
    if (!d->forms.values().contains(form)) {
        int item = d->item_counter++;
        d->insertForm(item, form, image);
        d->formVisualizations.insert(item, visualization);
        d->formCameras.insert(item, cam);
        emit added(item);
    } else {
        emit alreadyAdded();
    }
}

void gnomonFormManager::addForm(std::shared_ptr<gnomonAbstractDynamicForm> form, std::shared_ptr<gnomonAbstractMatplotlibVisualization> visualization)
{
    if (!d->forms.values().contains(form)) {
        int item = d->item_counter++;
        QImage image = visualization->imageRendering();
        form->metadata()->moveToThread(QThread::currentThread());
        d->insertForm(item, form, image);
        d->formMatplotlibVisualizations.insert(item, visualization);
        emit added(item);
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
        d->formMatplotlibVisualizations.insert(item, nullptr);
        emit added(item);
    } else {
        emit alreadyAdded();
    }
}

std::shared_ptr<gnomonAbstractDynamicForm> gnomonFormManager::get(int index)
{
    return d->forms.value(index, nullptr);
}

std::shared_ptr<gnomonAbstractVisualization> gnomonFormManager::getVisualization(int index)
{
    return d->formVisualizations.value(index, nullptr);
}

vtkCamera *gnomonFormManager::getCamera(int index)
{
    return d->formCameras.value(index, nullptr);
}

QImage gnomonFormManager::thumbnail(int index)
{
    return d->formData.value(index, QImage());
}

gnomonFormManager::gnomonFormManager(QObject *parent) : QObject(parent)
{
    d = new gnomonFormManagerPrivate;
}

gnomonFormManager::~gnomonFormManager(void)
{
    delete d;
}

gnomonFormManager *gnomonFormManager::s_instance = nullptr;

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
    int index = d->forms.key(form);
    d->formDropped[index] = true;
}

#include "gnomonFormManager.moc"
//
// gnomonFormManager.cpp ends here
