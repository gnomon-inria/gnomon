// Version: $Id$
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#include "gnomonVisualizations/gnomonAbstractVisualization.h"
#include "gnomonVisualizations/gnomonAbstractMatplotlibVisualization.h"

#include "gnomonFormManager.h"
#include "gnomonFormManager_p.h"
#include "gnomonFormManagerData.h"
#include "gnomonFormManagerFocus.h"
#include "gnomonFormManagerItem.h"
//#include "gnomonItemButton.h"

#include "gnomonView/gnomonViewForm.h"

#include <gnomonCore>
#include <gnomonPipeline>

#include <gnomonCore/gnomonCommand/gnomonBinaryImage/gnomonBinaryImageWriterCommand>
#include <gnomonCore/gnomonCommand/gnomonCellImage/gnomonCellImageWriterCommand>
#include <gnomonCore/gnomonCommand/gnomonCellComplex/gnomonCellComplexWriterCommand>
#include <gnomonCore/gnomonCommand/gnomonDataFrame/gnomonDataFrameWriterCommand>
#include <gnomonCore/gnomonCommand/gnomonImage/gnomonImageWriterCommand>
#include <gnomonCore/gnomonCommand/gnomonMesh/gnomonMeshWriterCommand>
#include <gnomonCore/gnomonCommand/gnomonPointCloud/gnomonPointCloudWriterCommand>
#include <gnomonCore/gnomonCommand/gnomonTree/gnomonTreeWriterCommand>

// #include <dtkFonts>
// #include <dtkThemes>
#include <dtkScript>

#include <vtkCamera.h>
#include <vtkGenericOpenGLRenderWindow.h>

// ///////////////////////////////////////////////////////////////////
// gnomonFormManagerPrivate
// ///////////////////////////////////////////////////////////////////

int gnomonFormManagerPrivate::item_counter = 0;

gnomonFormManagerPrivate::gnomonFormManagerPrivate(QObject *parent) : QObject(parent)
{
    // QString plugin_save = "gnomonplugins.core.io.gnomonAbstractFormWriter";
    // QString command = "import " + plugin_save;

    // int stat;

    // dtkScriptInterpreterPython::instance()->interpret(command, &stat);

    // this->writer =  gnomonCore::imagesSerieWriter::pluginFactory().create(plugin_save);

    // if(!this->writer) {
    //     qWarning() << "cannot create plugin " << plugin_save << " you won't be able to save images!!";
    // }

    // this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
}

gnomonFormManagerPrivate::~gnomonFormManagerPrivate(void)
{
    for (auto command: this->commands)
        delete command;
    this->commands.clear();
}

// gnomonFormManagerItem *gnomonFormManagerPrivate::create(gnomonAbstractDynamicForm * form, const QColor& color, const QImage& image)
// {
//     gnomonFormManagerItem *item = new gnomonFormManagerItem(color, QPixmap::fromImage(image), form->times().size(), this);

//     // connect(item, &gnomonFormManagerItem::destroy, [=] () {
//     //     this->contents->layout()->removeWidget(item);
//     //     this->forms.remove(item);
//     //     this->formWriterCommand.remove(item);
//     //     delete item;
//     // });

//     // gnomonFormManagerData *data = new gnomonFormManagerData(this);
//     // data->reference = item;
//     // data->name = form->current()->name();
//     // qDebug()<<Q_FUNC_INFO<<data->name;
//     // data->data_name = form->current()->dataName();
//     // qDebug()<<Q_FUNC_INFO<<data->data_name;
//     // data->data = form->current()->metadata();
//     // qDebug()<<Q_FUNC_INFO<<data->data;
//     // this->formData.insert(item, data);

//     return item;
// }


void gnomonFormManager::deleteForm(int id)
{
    if (!d->forms.contains(id)) {
        dtkWarn() << "Unknown forms id" << id << "can't delete it ";
        return;
    }
    d->forms.remove(id);
    // d->formVisualizations.remove(id);
    d->formCameras.remove(id);
    d->formData.remove(id);
    d->formWriterCommand.remove(id);
    // TODO: do something with pipeline ?
}

void gnomonFormManager::saveAs(int id, const QString& f) const
{
    QString file_name = f;
    QUrl url(file_name);
    if (url.isLocalFile()){
        file_name = url.toLocalFile();
    }

    gnomonAbstractDynamicForm *form = d->forms[id];

    if(!file_name.isEmpty()) {
        // settings.beginGroup("general");
        // settings.setValue("last_saved_file", export_file_path);
        // settings.endGroup();
        auto command = d->formWriterCommand[id];
        command->setPath(file_name);
        command->setForm(form);
        command->redo();
        d->pipeline_manager->addWriter(command);
    }
}


// gnomonFormManagerItem *gnomonFormManagerPrivate::create(const QString& contents, const QColor& color, const QImage& image)
// {
//     gnomonFormManagerItem *item = new gnomonFormManagerItem(color, QPixmap::fromImage(image), 1, this);

//     // connect(item, &gnomonFormManagerItem::destroy, [=] () {
//     //     this->contents->layout()->removeWidget(item);
//     //     this->forms.remove(item);
//     //     this->formWriterCommand.remove(item);
//     //     delete item;
//     // });

//     // connect(item, &gnomonFormManagerItem::clicked, [=] () {
//     //     q->present(item);
//     // });

//     gnomonFormManagerData *data = new gnomonFormManagerData(this);
//     data->contents = contents;
//     this->formData.insert(item, data);

//     return item;
// }

// ///////////////////////////////////////////////////////////////////
// gnomonFormManager
// ///////////////////////////////////////////////////////////////////

gnomonFormManager *gnomonFormManager::instance(void)
{
    if(!s_instance)
        s_instance = new gnomonFormManager;

    return s_instance;
}

void gnomonFormManager::addForm(gnomonAbstractDynamicForm *form, const QColor& color, const QJsonObject &visualization_description, const QImage& image,  vtkCamera *cam)
{
    // gnomonAbstractVisualization *visualization,

    // QImage image = visualization->imageRendering();


//        gnomonFormManagerItem *item = d->create(form, color, image);
    int item = d->item_counter++;

    d->forms.insert(item, form->clone());
    // d->formVisualizations.insert(item, visualization);
    d->visualization_description.insert(item, visualization_description);
    d->formCameras.insert(item, cam);
    d->formData.insert(item, image);

    d->pipeline_manager->setFormIndex(form, item);
    d->pipeline_manager->addClonedForm(form, d->forms[item]);

    gnomonAbstractWriterCommand *command = nullptr;
    QString writer_plugin;
    QString form_name;

    // TODO : Make it possible to choose an adapted writer plugin, rather than checking writer one by one (get inspiration from reader)

    if (gnomonBinaryImageSeries *binaryImage = dynamic_cast<gnomonBinaryImageSeries *>(form)) {
        form_name = binaryImage->formName();
        if (!d->commands.contains(form_name)) {
            d->commands.insert(form_name, new gnomonBinaryImageWriterCommand);
        }
        writer_plugin = dynamic_cast<gnomonBinaryImageWriterCommand *>(d->commands[form_name])->availablePlugins()[0];
    } else if (gnomonCellComplexSeries *cellcomplex = dynamic_cast<gnomonCellComplexSeries *>(form)) {
        form_name = cellcomplex->formName();
        if (!d->commands.contains(form_name)) {
            d->commands.insert(form_name, new gnomonCellComplexWriterCommand);
        }
        writer_plugin = dynamic_cast<gnomonCellComplexWriterCommand *>(d->commands[form_name])->availablePlugins()[0];
    } else if (gnomonCellImageSeries *cellimage = dynamic_cast<gnomonCellImageSeries *>(form)) {
        form_name = cellimage->formName();
        if (!d->commands.contains(form_name)) {
            d->commands.insert(form_name, new gnomonCellImageWriterCommand);
        }
        writer_plugin = dynamic_cast<gnomonCellImageWriterCommand *>(d->commands[form_name])->availablePlugins()[0];
    } else if (gnomonImageSeries *image = dynamic_cast<gnomonImageSeries *>(form)) {
        form_name = image->formName();
        if (!d->commands.contains(form_name)) {
            d->commands.insert(form_name, new gnomonImageWriterCommand);
        }
        writer_plugin = dynamic_cast<gnomonImageWriterCommand *>(d->commands[form_name])->availablePlugins()[0];
    } else if (gnomonMeshSeries *mesh = dynamic_cast<gnomonMeshSeries *>(form)) {
        form_name = mesh->formName();
        if (!d->commands.contains(form_name)) {
            d->commands.insert(form_name, new gnomonMeshWriterCommand);
        }
        writer_plugin = dynamic_cast<gnomonMeshWriterCommand *>(d->commands[form_name])->availablePlugins()[0];
    } else if (gnomonPointCloudSeries *pointCloud = dynamic_cast<gnomonPointCloudSeries *>(form)) {
        form_name = pointCloud->formName();
        if (!d->commands.contains(form_name)) {
            d->commands.insert(form_name, new gnomonPointCloudWriterCommand);
        }
        writer_plugin = dynamic_cast<gnomonPointCloudWriterCommand *>(d->commands[form_name])->availablePlugins()[0];
    }
    d->formWriterCommand[item] = d->commands[form_name];
    d->formWriterCommand[item]->setAlgorithmName(writer_plugin);

    if (!d->formCounter.contains(form_name)) {
        d->formCounter.insert(form_name, 0);
    }
    d->formCounter[form_name]++;
    
    emit added(item);
}

void gnomonFormManager::addForm(gnomonAbstractDynamicForm * form, const QColor& color, gnomonAbstractMatplotlibVisualization* visualization)
{
    QImage image = visualization->imageRendering();

//    gnomonFormManagerItem *item = d->create(form, color, image);
    int item = d->item_counter++;

    d->forms.insert(item, form->clone());
    d->formMatplotlibVisualizations.insert(item, visualization);
    d->formData.insert(item, image);

    d->pipeline_manager->setFormIndex(form, item);
    d->pipeline_manager->addClonedForm(form, d->forms[item]);

    QString writer_plugin;
    QString form_name;

    if (gnomonDataFrameSeries *dataFrame = dynamic_cast<gnomonDataFrameSeries *>(form)) {
        form_name = dataFrame->formName();
        if (!d->commands.contains(form_name)) {
            d->commands.insert(form_name, new gnomonDataFrameWriterCommand);
        }
        writer_plugin = dynamic_cast<gnomonDataFrameWriterCommand *>(d->commands[form_name])->availablePlugins()[0];
    } else if (gnomonTreeSeries *tree = dynamic_cast<gnomonTreeSeries *>(form)) {
        form_name = tree->formName();
        if (!d->commands.contains(form_name)) {
            d->commands.insert(form_name, new gnomonTreeWriterCommand);
        }
        writer_plugin = dynamic_cast<gnomonTreeWriterCommand *>(d->commands[form_name])->availablePlugins()[0];
    }
    d->formWriterCommand[item] = d->commands[form_name];
    d->formWriterCommand[item]->setAlgorithmName(writer_plugin);

    emit added(item);
}


void gnomonFormManager::addForm(gnomonAbstractDynamicForm * form, const QColor& color, const QImage& image)
{
//    gnomonFormManagerItem *item = d->create(form, color, image);
    int item = d->item_counter++;

    d->forms.insert(item, form);
    d->formMatplotlibVisualizations.insert(item, nullptr);
    d->formData.insert(item, image);

    QString writer_plugin;
    QString form_name;

    if (gnomonDataFrameSeries *dataFrame = dynamic_cast<gnomonDataFrameSeries *>(form)) {
        form_name = dataFrame->formName();
        if (!d->commands.contains(form_name)) {
            d->commands.insert(form_name, new gnomonDataFrameWriterCommand);
        }
        writer_plugin = dynamic_cast<gnomonDataFrameWriterCommand *>(d->commands[form_name])->availablePlugins()[0];
    } else if (gnomonTreeSeries *tree = dynamic_cast<gnomonTreeSeries *>(form)) {
        form_name = tree->formName();
        if (!d->commands.contains(form_name)) {
            d->commands.insert(form_name, new gnomonTreeWriterCommand);
        }
        writer_plugin = dynamic_cast<gnomonTreeWriterCommand *>(d->commands[form_name])->availablePlugins()[0];
    }
    d->formWriterCommand[item] = d->commands[form_name];
    d->formWriterCommand[item]->setAlgorithmName(writer_plugin);

    emit added(item);
}

gnomonAbstractDynamicForm *gnomonFormManager::get(int index)
{
    return d->forms.value(index, nullptr);
}

// gnomonAbstractVisualization *gnomonFormManager::getVisualization(int index)
// {
//     return d->formVisualizations.value(index, nullptr);
// }

QJsonObject gnomonFormManager::getVisuDescription(int index)
{
    return d->visualization_description.value(index);
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
    d->q = this;

    d->pipeline_manager = gnomonPipelineManager::instance();
}

gnomonFormManager::~gnomonFormManager(void)
{
    delete d;
}

gnomonFormManager *gnomonFormManager::s_instance = nullptr;

gnomonDynamicFormMetadata *gnomonFormManager::getDynamicFormMetadata(int id) {
    if(contains(id)) {
        auto ptr = d->forms[id]->metadata();
        QQmlEngine::setObjectOwnership(ptr, QQmlEngine::CppOwnership);
        return ptr;
    } else {
        return new gnomonDynamicFormMetadata();
    }
}

bool gnomonFormManager::contains(int id) {
    return d->forms.contains(id);
}

QVariantList gnomonFormManager::timeKeys(int id) {
    if(contains(id)) {
        const auto& times = d->forms[id]->times();
        QVariantList out;
        //out.reserve(times.size());
        for(double time : times) {
            out.append(time);
        }
        return out;
    } else {
        return {};
    }
}

QStringList gnomonFormManager::formMetadataKeysAtT(int id, double t) {
    if(contains(id)) {
        return d->forms[id]->at(t)->metadata().keys();
    } else {
        return {};
    }
}

QString gnomonFormManager::formMetadataValueAtT(int id, double t, const QString& key) {
    if(contains(id)){
        auto metadata = d->forms[id]->at(t)->metadata();
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
        gnomonAbstractDynamicForm* form = d->forms[id];
        gnomonAbstractWriterCommand* writer_command = d->formWriterCommand[id];
        QStringList extensions = writer_command->extensions();
        if (gnomonBinaryImageSeries *binaryImage = dynamic_cast<gnomonBinaryImageSeries *>(form)) {
            filter += binaryImage->formName().remove("gnomon");
        } else if (gnomonCellComplexSeries *cellComplex = dynamic_cast<gnomonCellComplexSeries *>(form)) {
            filter += cellComplex->formName().remove("gnomon");
        } else if (gnomonCellImageSeries *cellImage = dynamic_cast<gnomonCellImageSeries *>(form)) {
            filter += cellImage->formName().remove("gnomon");
        } else if (gnomonDataDictSeries *dataDict = dynamic_cast<gnomonDataDictSeries *>(form)) {
            filter += dataDict->formName().remove("gnomon");
        } else if (gnomonDataFrameSeries *dataFrame = dynamic_cast<gnomonDataFrameSeries *>(form)) {
            filter += dataFrame->formName().remove("gnomon");
        } else if (gnomonImageSeries *image = dynamic_cast<gnomonImageSeries *>(form)) {
            filter += image->formName().remove("gnomon");
        } else if (gnomonMeshSeries *mesh = dynamic_cast<gnomonMeshSeries *>(form)) {
            filter += mesh->formName().remove("gnomon");
        } else if (gnomonPointCloudSeries *pointCloud = dynamic_cast<gnomonPointCloudSeries *>(form)) {
            filter += pointCloud->formName().remove("gnomon");
        } else if (gnomonTreeSeries *tree = dynamic_cast<gnomonTreeSeries *>(form)) {
            filter += tree->formName().remove("gnomon");
        }
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

//
// gnomonFormManager.cpp ends here
