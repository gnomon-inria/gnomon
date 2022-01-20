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
        d->pipeline->addWriter(command);
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

    d->pipeline->addClonedForm(form, d->forms[item], item);

    gnomonAbstractWriterCommand *command = nullptr;
    QString writer_plugin;

    // TODO : Make it possible to choose an adapted writer plugin, rather than checking writer one by one (get inspiration from reader)

    if (gnomonBinaryImageSeries *binaryImage = dynamic_cast<gnomonBinaryImageSeries *>(form)) {
        writer_plugin = "binaryImageWriter";
        if (!d->commands.contains(writer_plugin)) {
            d->commands.insert(writer_plugin, new gnomonBinaryImageWriterCommand);
        }
    } else if (gnomonCellComplexSeries *cellcomplex = dynamic_cast<gnomonCellComplexSeries *>(form)) {
        writer_plugin = "gnomonCellComplexWriterPropertyTopomesh";
        if (!d->commands.contains(writer_plugin)) {
            d->commands.insert(writer_plugin, new gnomonCellComplexWriterCommand);
        }
    } else if (gnomonCellImageSeries *cellimage = dynamic_cast<gnomonCellImageSeries *>(form)) {
        writer_plugin = "gnomonCellImageWriterPropertySpatialImage";
        if (!d->commands.contains(writer_plugin)) {
            d->commands.insert(writer_plugin, new gnomonCellImageWriterCommand);
        }
    } else if (gnomonImageSeries *image = dynamic_cast<gnomonImageSeries *>(form)) {
        writer_plugin = "gnomonImageWriter";
        if (!d->commands.contains(writer_plugin)) {
            d->commands.insert(writer_plugin, new gnomonImageWriterCommand);
        }
    } else if (gnomonMeshSeries *mesh = dynamic_cast<gnomonMeshSeries *>(form)) {
        writer_plugin = "gnomonMeshWriterPropertyTopomesh";
        if (!d->commands.contains(writer_plugin)) {
            d->commands.insert(writer_plugin, new gnomonMeshWriterCommand);
        }
    }else if (gnomonPointCloudSeries *pointCloud = dynamic_cast<gnomonPointCloudSeries *>(form)) {
        writer_plugin = "pointCloudWriterPropertyTopomesh";
        if (!d->commands.contains(writer_plugin)) {
            d->commands.insert(writer_plugin, new gnomonPointCloudWriterCommand);
        }
    }
    d->formWriterCommand[item] = d->commands[writer_plugin];
    d->formWriterCommand[item]->setAlgorithmName(writer_plugin);

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

    d->pipeline->addClonedForm(form, d->forms[item], item);

    QString writer_plugin;

    if (gnomonDataFrameSeries *dataFrame = dynamic_cast<gnomonDataFrameSeries *>(form)) {
        writer_plugin = "gnomonDataFrameWriterPandas";
        if (!d->commands.contains(writer_plugin)) {
            d->commands.insert(writer_plugin, new gnomonDataFrameWriterCommand);
        }
    } else if (gnomonTreeSeries *tree = dynamic_cast<gnomonTreeSeries *>(form)) {
        writer_plugin = "gnomonTreeWriterTreex";
        if (!d->commands.contains(writer_plugin)) {
            d->commands.insert(writer_plugin, new gnomonTreeWriterCommand);
        }
    }
    d->formWriterCommand[item] = d->commands[writer_plugin];
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

    if (gnomonDataFrameSeries *dataFrame = dynamic_cast<gnomonDataFrameSeries *>(form)) {
        writer_plugin = "gnomonDataFrameWriterPandas";
        if (!d->commands.contains(writer_plugin)) {
            d->commands.insert(writer_plugin, new gnomonDataFrameWriterCommand);
        }
    }
    d->formWriterCommand[item] = d->commands[writer_plugin];
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

    d->pipeline = gnomonPipeline::instance();
}

gnomonFormManager::~gnomonFormManager(void)
{
    delete d;
}

gnomonFormManager *gnomonFormManager::s_instance = nullptr;

//
// gnomonFormManager.cpp ends here
