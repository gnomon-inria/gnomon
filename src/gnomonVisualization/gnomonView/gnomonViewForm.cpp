#include "gnomonViewForm.h"

#include <gnomonCore/gnomonAlgorithm/gnomonMesh/gnomonAbstractMeshAdapter>
#include <gnomonCore/gnomonAlgorithm/gnomonCellComplex/gnomonAbstractCellComplexAdapter>
#include <gnomonCore/gnomonCommand/gnomonCellComplex/gnomonCellComplexAdapterCommand>
#include <gnomonCore/gnomonCommand/gnomonMesh/gnomonMeshAdapterCommand>
#include <gnomonCore/gnomonPythonPluginLoader.h>

#include "gnomonManager/gnomonFormManager.h"
#include "gnomonVisualizations/gnomonCellComplex/gnomonAbstractVisualizationCellComplex.h"
#include "gnomonVisualizations/gnomonCellImage/gnomonAbstractVisualizationCellImage.h"
#include "gnomonVisualizations/gnomonImage/gnomonAbstractVisualizationImage.h"
#include "gnomonVisualizations/gnomonBinaryImage/gnomonAbstractVisualizationBinaryImage.h"
#include "gnomonVisualizations/gnomonMesh/gnomonAbstractVisualizationMesh.h"
#include "gnomonVisualizations/gnomonPointCloud/gnomonAbstractVisualizationPointCloud.h"

#include "gnomonInteractorStyle/gnomonInteractorStyle.h"
#include "gnomonInteractorStyle/gnomonInteractorStyleXYZ.h"

#include <vtkCamera.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkInteractorStyleImage.h>
#include <vtkPNGWriter.h>
#include <vtkRenderer.h>
#include <vtkRendererCollection.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkWindowToImageFilter.h>

// #include <QVTKInteractor.h>
// #include <QVTKOpenGLNativeWidget.h>

// ///////////////////////////////////////////////////////////////////
// gnomonViewFormPrivate
// ///////////////////////////////////////////////////////////////////

class gnomonViewFormPrivate : public QObject // QVTKOpenGLNativeWidget
{
    Q_OBJECT

public:
     gnomonViewFormPrivate(QObject *parent = Q_NULLPTR);
    ~gnomonViewFormPrivate(void);

public:
    vtkRenderWindowInteractor *interactor(void)
    {
        return this->window->GetInteractor();
    }

public slots:
    void exportToManager(void);
    void clear(void);

public:
    void setViewMode(gnomonViewForm::Mode mode);
    void setSliceOrientation(gnomonViewForm::Orientation orientation);
    void updateOrientation(void);

public:
    void updateFormsTimes(void);

public:
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> window;
    vtkSmartPointer<vtkRenderer> renderer2D;
    vtkSmartPointer<vtkRenderer> renderer3D;

public:
    gnomonViewForm *q = nullptr;

public:
    gnomonViewForm::Mode mode = gnomonViewForm::VIEW_MODE_3D;
    gnomonViewForm::Orientation ori = gnomonViewForm::NONE;
    QMap<gnomonViewForm::Orientation, vtkSmartPointer<vtkCamera> > cameras;

public:
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > forms;
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > to_export;

    QMap<QString, QString> formVisualizationNames;
    QMap<QString, gnomonAbstractVisualization *> formVisualization;
    QMap<QString, QJsonObject > visualization_description;
    QMap<QString, bool > formVisibility;
    QVariantMap parameters;
public:
    QMap<QString, bool> acceptForms;

public:
    QMetaObject::Connection connect3D;
    QMetaObject::Connection connect2D;
    QMetaObject::Connection connectXY;
    QMetaObject::Connection connectXZ;
    QMetaObject::Connection connectYZ;
    QMetaObject::Connection connectSlice;
    QMetaObject::Connection connectTime;
    
public:
    void clearConnections(void);
    
public:
    gnomonInteractorStyle *default_style = nullptr;
    gnomonInteractorStyle *xyz_style = nullptr;
    QList<gnomonInteractorStyle *> available_styles;

    gnomonInteractorStyle *style = nullptr;

public:
    int syncing_count = 0; QTimer *syncing_timer = nullptr; bool synced = false; bool syncing = false;

public:
    bool enableLink = false;
    bool enableMenus = true;

public:
    bool input_view = false;
    bool in_pool = false;

public:
    QColor export_color = QColor("#cccccc");
    QSet<double> forms_times;

public:
    double xBounds[2] = {0,0}, yBounds[2] = {0,0}, zBounds[2] = {0,0};
    double c_x = 0, c_y = 0, c_z = 0;

public:
    double c_t = 0;

public:
    bool empty = true;

signals:
    void sliceOrientationChanged(int);

public slots:
    void setFormVisualization(const QString& name, const QString& visu_name, const QJsonObject& parameters = {});
    void updateFormVisualization(const QString& name, const QJsonObject& parameters = {});

public:
    QMap<QString, QMap<QString, gnomonAbstractAdapterCommand *> > adapterCommands;
    QMap<QString, QMap<QString, QString> > adapterTargets;
    QMap<QString, QMap<QString, QString> > adapterDescriptions;

    std::shared_ptr<gnomonAbstractDynamicForm> form_to_adapt = nullptr;

public slots:
    void adaptForm(const QString& adapter_plugin);
};

gnomonViewFormPrivate::gnomonViewFormPrivate(QObject *parent) : QObject(parent)
{
    QColor background_color = QColor("#00000000");

    this->renderer2D = vtkSmartPointer<vtkRenderer>::New();
    this->renderer2D->SetBackground(background_color.redF(), background_color.greenF(), background_color.blueF());

    this->renderer3D = vtkSmartPointer<vtkRenderer>::New();
    this->renderer3D->SetBackground(background_color.redF(), background_color.greenF(), background_color.blueF());

    this->default_style = new gnomonInteractorStyle();
    this->xyz_style = new gnomonInteractorStyleXYZ();

    this->available_styles.push_back(this->default_style);
    this->available_styles.push_back(this->xyz_style);

    static int count = 0;
}

gnomonViewFormPrivate::~gnomonViewFormPrivate(void)
{
    this->clearConnections();
    delete this->default_style;
    delete this->xyz_style;
}

void gnomonViewFormPrivate::exportToManager(void)
{
    for (const auto& key : this->forms.keys()) {

        QJsonObject json;
        json.insert("plugin_name",this->formVisualizationNames[key]);
        QJsonObject parameters;

        dtkCoreParameters visu_parameters = this->formVisualization[key]->parameters();
        for (auto param_name : visu_parameters.keys()){
            QVariantHash param_value = visu_parameters[param_name]->toVariantHash();
            parameters.insert(param_name, QJsonObject::fromVariantHash(param_value));
        }
        json.insert("parameters", parameters);

        this->visualization_description[key] = json;
        QImage image = this->formVisualization[key]->imageRendering();
        gnomonFormManager::instance()->addForm(this->forms[key], this->export_color, this->visualization_description[key],image, this->renderer3D->GetActiveCamera());
        q->emit exportedForm(this->forms[key]);
    }
}

void gnomonViewFormPrivate::clearConnections(void)
{
    disconnect(this->connect3D);
    disconnect(this->connect2D);
    disconnect(this->connectXY);
    disconnect(this->connectYZ);
    disconnect(this->connectXZ);
    disconnect(this->connectSlice);
    disconnect(this->connectTime);
}

void gnomonViewFormPrivate::setViewMode(gnomonViewForm::Mode mode)
{
    this->mode = mode;

    if (this->mode == gnomonViewForm::VIEW_MODE_3D) {
        this->renderer2D->DrawOff();
        this->renderer2D->InteractiveOff();

        this->renderer3D->InteractiveOn();
        this->renderer3D->DrawOn();
    } else if (this->mode == gnomonViewForm::VIEW_MODE_2D) {
        this->renderer3D->DrawOff();
        this->renderer3D->InteractiveOff();

        this->renderer2D->InteractiveOn();
        this->renderer2D->DrawOn();
    }
}

void gnomonViewFormPrivate::setSliceOrientation(gnomonViewForm::Orientation orientation)
{
    this->ori = orientation;
    this->updateOrientation();
}


void gnomonViewFormPrivate::updateOrientation(void)
{
    // if(!this->cameras.contains(this->ori)) {
    if(true) {

        vtkSmartPointer<vtkCamera> cam = vtkCamera::New();
        cam->ParallelProjectionOn();
        cam->SetParallelScale(1);
        cam->SetFocalPoint((xBounds[0]+xBounds[1])/2,(yBounds[0]+yBounds[1])/2,(zBounds[0]+zBounds[1])/2);

        switch(this->ori)
        {
            case gnomonViewForm::SLICE_ORIENTATION_XY:
                cam->SetPosition((xBounds[0]+xBounds[1])/2,(yBounds[0]+yBounds[1])/2,zBounds[1]);
                cam->SetViewUp(0,1,0);
                cam->SetClippingRange((zBounds[1] - zBounds[0]) - 3.0, (zBounds[1] - zBounds[0]) + 3.0);
                break;

            case gnomonViewForm::SLICE_ORIENTATION_XZ:
                cam->SetPosition((xBounds[0]+xBounds[1])/2,yBounds[0],(zBounds[0]+zBounds[1])/2);
                cam->SetViewUp(0,0,1);
                cam->SetClippingRange((yBounds[1] - yBounds[0]) - 3.0, (yBounds[1] - yBounds[0]) + 3.0);
                break;

            case gnomonViewForm::SLICE_ORIENTATION_YZ:
                cam->SetPosition(xBounds[1],(yBounds[0]+yBounds[1])/2,(zBounds[0]+zBounds[1])/2);
                cam->SetViewUp(0,0,1);
                cam->SetClippingRange((xBounds[1] - xBounds[0]) - 3.0, (xBounds[1] - xBounds[0]) + 3.0);
                break;

            default:
                break;
        }
        this->renderer2D->SetActiveCamera(cam);
        this->renderer2D->ResetCamera();
        this->renderer2D->ResetCameraClippingRange();
        this->cameras[this->ori] = cam;
    }
    else {
        this->renderer2D->SetActiveCamera(this->cameras[this->ori]);
    }

    emit q->updated();

    this->interactor()->Render();
}

void gnomonViewFormPrivate::clear(void)
{
     for (const auto& key : this->formVisualization.keys()) {

         this->formVisualization[key]->disconnect();
         this->formVisualization[key]->clearConnections();
         this->formVisualization[key]->clear();
         // TODO: Fix offscreen rendering related segFault when destroying Visualization
         // delete this->formVisualization[key];

//         this->parameterLayouts[key]->disconnect();
//         delete this->parameterLayouts[key];

//         this->formVisualizationMenus[key]->removeItem(this->formVisualizationPaneItems[key]);
//
//         this->formVisualizationPaneItems[key]->disconnect();
//         this->formVisualizationPaneItems[key]->clear();
//         delete this->formVisualizationPaneItems[key];
//
//         this->view_menubar->removeMenu(this->formVisualizationMenus[key]);
//
//         this->formVisualizationMenus[key]->disconnect();
//         this->formVisualizationMenus[key]->clear();
//         delete this->formVisualizationMenus[key];
    }

    this->formVisualizationNames.clear();
    this->formVisualization.clear();
    this->forms.clear();
    // this->parameterLayouts.clear();
    // this->formVisualizationMenus.clear();
    // this->formVisualizationPaneItems.clear();

    // this->updateInteractorStyleMenu();

    this->empty = true;

    // this->refresh();
    // this->view_menubar->touch();

    q->render();
}

void gnomonViewFormPrivate::updateFormVisualization(const QString& name, const QJsonObject& parameters)
{
    auto&& visu = this->formVisualization[name];
    auto&& form = this->forms[name];
   
    visu->setView(q);

    bool update = false;

    if (name == "gnomonBinaryImage") {
        auto formVisualizationBinaryImage = (gnomonAbstractVisualizationBinaryImage *)visu;

        if (formVisualizationBinaryImage->binaryImage() != std::dynamic_pointer_cast<gnomonBinaryImageSeries>(form)) {
            formVisualizationBinaryImage->setBinaryImage(std::dynamic_pointer_cast<gnomonBinaryImageSeries>(form));
            update = true;
        }
    } else if (name == "gnomonCellComplex") {
        auto formVisualizationCellComplex = (gnomonAbstractVisualizationCellComplex *)visu;
        if (formVisualizationCellComplex->cellComplex() != std::dynamic_pointer_cast<gnomonCellComplexSeries>(form)) {
            formVisualizationCellComplex->setCellComplex(std::dynamic_pointer_cast<gnomonCellComplexSeries>(form));
            update = true;
        }
    } else if (name == "gnomonCellImage") {
        auto formVisualizationCellImage = (gnomonAbstractVisualizationCellImage *)visu;
        if (formVisualizationCellImage->cellImage() != std::dynamic_pointer_cast<gnomonCellImageSeries>(form)) {
            formVisualizationCellImage->setCellImage(std::dynamic_pointer_cast<gnomonCellImageSeries>(form));
            update = true;
        }
    } else if (name == "gnomonImage") {
        auto formVisualizationImage = (gnomonAbstractVisualizationImage *)visu;
        if (formVisualizationImage->image() != std::dynamic_pointer_cast<gnomonImageSeries>(form)) {
            formVisualizationImage->setImage(std::dynamic_pointer_cast<gnomonImageSeries>(form));
            update = true;
        }
    } else if (name == "gnomonMesh") {
        auto formVisualizationMesh = (gnomonAbstractVisualizationMesh *)visu;
        if (formVisualizationMesh->mesh() != std::dynamic_pointer_cast<gnomonMeshSeries>(form)) {
            formVisualizationMesh->setMesh(std::dynamic_pointer_cast<gnomonMeshSeries>(form));
            update = true;
        }
    } else if (name == "gnomonPointCloud") {
        auto formVisualizationPointCloud = (gnomonAbstractVisualizationPointCloud *)visu;
        if (formVisualizationPointCloud->pointCloud() != std::dynamic_pointer_cast<gnomonPointCloudSeries>(form)) {
            formVisualizationPointCloud->setPointCloud(std::dynamic_pointer_cast<gnomonPointCloudSeries>(form));
            update = true;
        }
    }

    dtkCoreParameters visu_parameters;
    for(auto& key: parameters.keys()) {
        QVariantHash param = parameters[key].toObject().toVariantHash();
        QString param_type = param["type"].toString();
        // TODO: Remove when fixed in dtk-core-python
        if (param_type.contains("dtkCoreParameterRange<") or param_type.contains("dtkCoreParameterNumeric<")) {
            param_type = param_type.remove(",void");
            param.insert("type", param_type);
        }
        auto *parameter = dtkCoreParameter::create(param);
        if(parameter) {
            visu_parameters[key] = parameter;
        } else {
            dtkWarn() << Q_FUNC_INFO << "this parameter is not handled properly: " << param_type << param;
        }
    }
    visu->setParameters(visu_parameters);

    if (update) {
        visu->update();
        q->switchTo3D();
    }

    visu->setVisible(this->formVisibility[name]);
}

void gnomonViewFormPrivate::setFormVisualization(const QString& name, const QString& visu_name, const QJsonObject& parameters)
{
    //auto&& visu_name = this->formVisualizationNames[name];

    if (!this->formVisualizationNames.contains(name) || this->formVisualizationNames[name] != visu_name) {

        if (this->formVisualization[name]) {
            this->formVisualization[name]->clearConnections();
            this->formVisualization[name]->clear();
            // TODO: Fix offscreen rendering related segFault when destroying Visualization
            // delete this->formVisualization[name];
            // this->formVisualization[name] = nullptr;
        }

        if (name == "gnomonBinaryImage") {
            this->formVisualization[name] = gnomonVisualization::visualizationBinaryImage::pluginFactory().create(visu_name);
        } else if (name == "gnomonCellComplex") {
            this->formVisualization[name] = gnomonVisualization::visualizationCellComplex::pluginFactory().create(visu_name);
        } else if (name == "gnomonCellImage") {
            this->formVisualization[name] = gnomonVisualization::visualizationCellImage::pluginFactory().create(visu_name);
        } else if (name == "gnomonImage") {
            this->formVisualization[name] = gnomonVisualization::visualizationImage::pluginFactory().create(visu_name);
        } else if (name == "gnomonMesh") {
            this->formVisualization[name] = gnomonVisualization::visualizationMesh::pluginFactory().create(visu_name);
        } else if (name == "gnomonPointCloud") {
            this->formVisualization[name] = gnomonVisualization::visualizationPointCloud::pluginFactory().create(visu_name);
        }
    }

    this->formVisualizationNames[name] = visu_name;
    if (!this->formVisibility.contains(name)) {
        this->formVisibility[name] = true;
    }
    connect(this->formVisualization[name], &gnomonAbstractVisualization::parametersChanged, [=] () {
        emit q->formVisuParametersChanged();
    });

    this->updateFormVisualization(name, parameters);
}

void gnomonViewFormPrivate::updateFormsTimes(void)
{
    this->forms_times.clear();

    for (const auto& key : this->forms.keys()) {
        for(auto time : this->forms[key]->times()) {
            this->forms_times.insert(time);
        }
    }

    emit q->timeMaxChanged(q->timeMax());
    q->timesChanged();
}

void gnomonViewFormPrivate::adaptForm(const QString& adapter_plugin)
{
    std::shared_ptr<gnomonAbstractDynamicForm> form = this->form_to_adapt;

    if (std::shared_ptr<gnomonMeshSeries> mesh = std::dynamic_pointer_cast<gnomonMeshSeries>(form)) {
        gnomonMeshAdapterCommand *meshCommand = dynamic_cast<gnomonMeshAdapterCommand *>(this->adapterCommands["gnomonMesh"][adapter_plugin]);

        meshCommand->setInput(mesh);
        meshCommand->redo();
        std::shared_ptr<gnomonAbstractDynamicForm> adaptedMesh = meshCommand->output();
        if (adaptedMesh) {
            // gnomonPipelineManager::instance()->addAdapter(meshCommand);
            q->setForm("adaptedMesh",adaptedMesh);
        }
    } else if (std::shared_ptr<gnomonCellComplexSeries> cellComplex = std::dynamic_pointer_cast<gnomonCellComplexSeries>(form)) {
        gnomonCellComplexAdapterCommand *cellComplexCommand = dynamic_cast<gnomonCellComplexAdapterCommand *>(this->adapterCommands["gnomonCellComplex"][adapter_plugin]);

        cellComplexCommand->setInput(cellComplex);
        cellComplexCommand->redo();
        std::shared_ptr<gnomonAbstractDynamicForm> adaptedCellComplex = cellComplexCommand->output();
        if (adaptedCellComplex) {
            // gnomonPipelineManager::instance()->addAdapter(cellComplexCommand);
            q->setForm("adaptedCellComplex",adaptedCellComplex);
        }
    }

    // if (this->adapter_menu) {
    //     this->adapter_menu->close();
    // }
}

// ///////////////////////////////////////////////////////////////////
// gnomonViewForm
// ///////////////////////////////////////////////////////////////////

gnomonViewForm::gnomonViewForm(QObject *parent) : QObject(parent)
{
    d = new gnomonViewFormPrivate;
    d->q = this;

    loadPluginGroup("visualizationBinaryImage");
    loadPluginGroup("visualizationCellComplex");
    loadPluginGroup("visualizationCellImage");
    loadPluginGroup("visualizationImage");
    loadPluginGroup("visualizationMesh");
    loadPluginGroup("visualizationPointCloud");

    d->acceptForms["gnomonBinaryImage"] = false;
    d->acceptForms["gnomonCellComplex"] = false;
    d->acceptForms["gnomonCellImage"] = false;
    d->acceptForms["gnomonImage"] = false;
    d->acceptForms["gnomonMesh"] = false;
    d->acceptForms["gnomonPointCloud"] = false;

    for (const auto& form : d->acceptForms.keys()) {
        if (form=="gnomonMesh") {
            loadPluginGroup("meshAdapter");
            for (const auto& key : gnomonCore::meshAdapter::pluginFactory().keys())
            {
                gnomonAbstractMeshAdapter *adapter = dynamic_cast<gnomonAbstractMeshAdapter *>(gnomonCore::meshAdapter::pluginFactory().create(key));
                if (!d->adapterCommands.contains(form))
                {
                    QMap<QString, QString> empty_target;
                    d->adapterTargets[form] = empty_target;
                    QMap<QString, QString> empty_desc;
                    d->adapterDescriptions[form] = empty_desc;
                    QMap<QString, gnomonAbstractAdapterCommand *> empty_list;
                    d->adapterCommands[form] = empty_list;
                }
                d->adapterTargets[form][key] = adapter->target();
                d->adapterDescriptions[form][key] = adapter->documentation().split("\n")[1];
                d->adapterCommands[form][key] = new gnomonMeshAdapterCommand;
                d->adapterCommands[form][key]->setAlgorithmName(key);
                delete adapter;
            }
        } else if (form=="gnomonCellComplex") {
            loadPluginGroup("cellComplexAdapter");
            for (const auto& key : gnomonCore::cellComplexAdapter::pluginFactory().keys())
            {
                gnomonAbstractCellComplexAdapter *adapter = dynamic_cast<gnomonAbstractCellComplexAdapter *>(gnomonCore::cellComplexAdapter::pluginFactory().create(key));
                if (!d->adapterCommands.contains(form))
                {
                    QMap<QString, QString> empty_target;
                    d->adapterTargets[form] = empty_target;
                    QMap<QString, QString> empty_desc;
                    d->adapterDescriptions[form] = empty_desc;
                    QMap<QString, gnomonAbstractAdapterCommand *> empty_list;
                    d->adapterCommands[form] = empty_list;
                }
                d->adapterTargets[form][key] = adapter->target();
                d->adapterDescriptions[form][key] = adapter->documentation().split("\n")[1];
                d->adapterCommands[form][key] = new gnomonCellComplexAdapterCommand;
                d->adapterCommands[form][key]->setAlgorithmName(key);
                delete adapter;
            }
        }
    }

     connect(this, &gnomonViewForm::formAdded, [=] (const QString& key) {
         d->updateFormsTimes();
         d->empty = false;
         emit formsChanged();
     });
}

void gnomonViewForm::transmit(void)
{
    d->exportToManager();
}

void gnomonViewForm::associate(vtkGenericOpenGLRenderWindow *window)
{
    d->window = window;
//     d->window->SetInteractor(d->window->MakeRenderWindowInteractor());
// #if defined(Q_OS_LINUX)
//     d->window->GetInteractor()->Initialize();
// #endif
    d->window->AddRenderer(d->renderer2D);
    d->window->AddRenderer(d->renderer3D);

    this->setInteractorStyle(d->default_style);

    this->switchTo2D();
    this->switchTo2DXY();
    this->switchTo3D();

    d->updateOrientation();
    d->updateFormsTimes();
}

gnomonViewForm::~gnomonViewForm(void)
{
    delete d;
}

void gnomonViewForm::switchTo3D(void)
{
    bool hasChanged = d->mode != gnomonViewForm::VIEW_MODE_3D;
    d->setViewMode(gnomonViewForm::VIEW_MODE_3D);

    if (hasChanged){
        emit switchedTo3D();
        emit modeChanged();
    }
}

void gnomonViewForm::switchTo2D(void)
{
    bool hasChanged = d->mode != gnomonViewForm::VIEW_MODE_2D;
    d->setViewMode(gnomonViewForm::VIEW_MODE_2D);

    if (hasChanged) {
        switch(d->ori) {
            case gnomonViewForm::SLICE_ORIENTATION_XY:
                this->switchTo2DXY();
                break;

            case gnomonViewForm::SLICE_ORIENTATION_XZ:
                this->switchTo2DXZ();
                break;

            case gnomonViewForm::SLICE_ORIENTATION_YZ:
                this->switchTo2DYZ();
                break;

            default:
                break;
        }

        emit switchedTo2D();
        emit modeChanged();
    }

    // d->slice_slider->setVisible(true);
    // d->slice_slider->setEnabled(true);
}

void gnomonViewForm::switchTo2DXY(void)
{
    emit sliceOrientationChanged(gnomonViewForm::SLICE_ORIENTATION_XY);

    bool hasChanged = d->ori != gnomonViewForm::SLICE_ORIENTATION_XY;
    d->setSliceOrientation(gnomonViewForm::SLICE_ORIENTATION_XY);

    if (hasChanged) {
        emit switchedTo2DXY();
        emit orientationChanged();
    }
    
}

void gnomonViewForm::switchTo2DXZ(void)
{
    emit sliceOrientationChanged(gnomonViewForm::SLICE_ORIENTATION_XZ);

    // TODO: Call the slider's callback directly
    // d->slice_slider->blockSignals(true);
    // d->slice_slider->setMinimum(d->yBounds[0]);
    // d->slice_slider->setMaximum(d->yBounds[1]);
    // d->slice_slider->blockSignals(false);
    // d->slice_slider->setValue(d->c_y);
    emit sliceChanged(d->c_y);

    bool hasChanged = d->ori != gnomonViewForm::SLICE_ORIENTATION_XZ;
    d->setSliceOrientation(gnomonViewForm::SLICE_ORIENTATION_XZ);

    // d->renderer2D_XY->toggle(false);
    // d->renderer2D_XZ->toggle(true);
    // d->renderer2D_YZ->toggle(false);

    if (hasChanged)
        emit switchedTo2DXZ();
        emit orientationChanged();
}

void gnomonViewForm::switchTo2DYZ(void)
{
    emit sliceOrientationChanged(gnomonViewForm::SLICE_ORIENTATION_YZ);

    // TODO: Call the slider's callback directly
    // d->slice_slider->blockSignals(true);
    // d->slice_slider->setMinimum(d->xBounds[0]);
    // d->slice_slider->setMaximum(d->xBounds[1]);
    // d->slice_slider->blockSignals(false);
    // d->slice_slider->setValue(d->c_x);
    // emit sliceChanged(d->c_x);

    bool hasChanged = d->ori != gnomonViewForm::SLICE_ORIENTATION_YZ;
    d->setSliceOrientation(gnomonViewForm::SLICE_ORIENTATION_YZ);

    // d->renderer2D_XY->toggle(false);
    // d->renderer2D_XZ->toggle(false);
    // d->renderer2D_YZ->toggle(true);

    if (hasChanged)
        emit switchedTo2DYZ();
        emit orientationChanged();
}

void gnomonViewForm::sliceChange(int value)
{
    bool valueChanged = false;

    switch(d->ori)
    {
        case gnomonViewForm::SLICE_ORIENTATION_XY:
            if (d->c_z != value) {
                d->c_z = value;
                valueChanged = true;
            }
            break;
        case gnomonViewForm::SLICE_ORIENTATION_XZ:
            if (d->c_y != value) {
                d->c_y = value;
                valueChanged = true;
            }
            break;
        case gnomonViewForm::SLICE_ORIENTATION_YZ:
            if (d->c_x != value){
                d->c_x = value;
                valueChanged = true;
            }
            break;
        default:
            break;
    };

    // TODO: Call the slider's callback directly
    // d->slice_slider->blockSignals(true);
    // d->slice_slider->setValue(value);
    // d->slice_slider->blockSignals(false);

    if (valueChanged)
        emit sliceChanged(value);

    d->interactor()->Render();
}


void gnomonViewForm::setCurrentTime(double value)
{
    QList<double> sorted_times = this->times();
    if(sorted_times.contains(value)) {
        double time = sorted_times[value];

        bool valueChanged = false;
        if (d->c_t != time) {
            valueChanged = true;
            d->c_t = time;
        }

        if (valueChanged) {
            emit timeChanged(time);
        }

        d->interactor()->Render();
    }
}

double gnomonViewForm::currentTime(void) const
{
    return d->c_t;
}

double gnomonViewForm::timeMax(void)
{
    QList<double> times = this->times();
    if (times.isEmpty()) {
        return -1;
    }  else {
        return this->times().last();
    }
}

QList<double> gnomonViewForm::times(void)
{
    QList<double> sorted_times = QList<double>(d->forms_times.begin(), d->forms_times.end());
    std::sort(sorted_times.begin(), sorted_times.end());

    return sorted_times;
}

void gnomonViewForm::tryLinking(void)
{
    // connect(d->sync, &gnomonOverlayButton::iconClicked, [=] ()
    // {
    //     d->sync->toggle(!d->sync->isToggled());

    //     if (d->sync->isToggled())
    //         emit linking();
    //     else
    //         emit unlinking();

    d->syncing = !d->syncing;

    if (d->syncing) {
        emit linking();
    } else {
        emit unlinking();
    }

    // if (d->syncing && !d->synced) {

    // d->syncing_count = 0;

    // if(!d->syncing_timer)
    //     d->syncing_timer = new QTimer(d);

    // connect(d->syncing_timer, &QTimer::timeout, [=] () {
    //     if (d->syncing_count == 11) {
    //         d->syncing = false;
    //         d->syncing_timer->stop();
    //         d->syncing_timer->disconnect();
    //         delete d->syncing_timer;
    //         d->syncing_timer = nullptr;
    //         emit unlinking();
    //     }
    // });

    // d->syncing_timer->start(500);

    // }

    emit syncingChanged();
}

void gnomonViewForm::link(gnomonViewForm *other)
{
    d->synced = true;

    if (d->mode == gnomonViewForm::VIEW_MODE_3D) {
        other->switchTo3D();
    } else if (d->mode == gnomonViewForm::VIEW_MODE_2D) {
        other->switchTo2D();
        if (d->ori == gnomonViewForm::SLICE_ORIENTATION_XY) {
            other->switchTo2DXY();
        } else if (d->ori == gnomonViewForm::SLICE_ORIENTATION_XZ) {
            other->switchTo2DXZ();
        } else if (d->ori == gnomonViewForm::SLICE_ORIENTATION_YZ) {
            other->switchTo2DYZ();
        }
    }

    d->renderer2D->SetActiveCamera(other->d->renderer2D->GetActiveCamera());
    d->renderer3D->SetActiveCamera(other->d->renderer3D->GetActiveCamera());

    other->d->window->AddObserver(vtkCommand::RenderEvent, this, &gnomonViewForm::render);
    this->render();

    d->clearConnections();
    d->connect3D = connect(other, &gnomonViewForm::switchedTo3D, [=] () {
        this->switchTo3D();
        d->renderer3D->SetActiveCamera(other->d->renderer3D->GetActiveCamera());
    });
    d->connect2D = connect(other, &gnomonViewForm::switchedTo2D, [=] () {
        this->switchTo2D();
        if (other->d->ori == gnomonViewForm::SLICE_ORIENTATION_XY) {
            this->switchTo2DXY();
        } else if (other->d->ori == gnomonViewForm::SLICE_ORIENTATION_XZ) {
            this->switchTo2DXZ();
        } else if (other->d->ori == gnomonViewForm::SLICE_ORIENTATION_YZ) {
            this->switchTo2DYZ();
        }
        d->renderer2D->SetActiveCamera(other->d->renderer2D->GetActiveCamera());
    });
    d->connectXY = connect(other, &gnomonViewForm::switchedTo2DXY, [=] () {
        this->switchTo2DXY();
        d->renderer2D->SetActiveCamera(other->d->renderer2D->GetActiveCamera());
    });
    d->connectXZ = connect(other, &gnomonViewForm::switchedTo2DXZ, [=] () {
        this->switchTo2DXZ();
        d->renderer2D->SetActiveCamera(other->d->renderer2D->GetActiveCamera());
    });
    d->connectYZ = connect(other, &gnomonViewForm::switchedTo2DYZ, [=] () {
        this->switchTo2DYZ();
        d->renderer2D->SetActiveCamera(other->d->renderer2D->GetActiveCamera());
    });
    d->connectSlice = connect(other, &gnomonViewForm::sliceChanged, [=] (int value) {
        this->sliceChange(value);
    });
    d->connectTime = connect(other, &gnomonViewForm::timeChanged, [=] (double value) {
        this->onTimeChanged(value);
    });

    emit syncedChanged();
}

void gnomonViewForm::unlink(gnomonViewForm *other)
{
    d->synced = false;

    vtkSmartPointer<vtkCamera> camera2D = vtkCamera::New();
    camera2D->ShallowCopy(d->renderer2D->GetActiveCamera());
    d->renderer2D->SetActiveCamera(camera2D);

    vtkSmartPointer<vtkCamera> camera3D = vtkCamera::New();
    camera3D->ShallowCopy(d->renderer3D->GetActiveCamera());
    d->renderer3D->SetActiveCamera(camera3D);

    d->clearConnections();

    emit syncedChanged();
}

void gnomonViewForm::disconnectTime() {
    disconnect(d->connectTime);
}

void gnomonViewForm::setExportColor(const QColor& color)
{
    d->export_color = color;
}

// void gnomonViewForm::toggleVisualizationPane(void)
// {
//     d->formVisualizationPane->toggle();
// }


QStringList gnomonViewForm::formNames(void)
{
     return d->forms.keys();
}

std::shared_ptr<gnomonAbstractDynamicForm> gnomonViewForm::form(const QString& name)
{
    if (d->forms.contains(name)) {
        return d->forms[name];
    } else {
        return nullptr;
    }
}

void gnomonViewForm::setForm(const QString& name, std::shared_ptr<gnomonAbstractDynamicForm> form, const QJsonObject &visualization)
{
    if (std::shared_ptr<gnomonBinaryImageSeries> binaryImage = std::dynamic_pointer_cast<gnomonBinaryImageSeries>(form)) {
        if (d->acceptForms["gnomonBinaryImage"]) {
            this->setBinaryImage(binaryImage, visualization);
        } else {
            emit badFormDropped("gnomonbinaryImage", acceptedForms().join(", "));
        }
    }  else if (std::shared_ptr<gnomonCellComplexSeries> cellComplex = std::dynamic_pointer_cast<gnomonCellComplexSeries>(form)) {
        if (d->acceptForms["gnomonCellComplex"]) {
            this->setCellComplex(cellComplex, visualization);
        } else {
            this->setAdaptedForm("gnomonCellComplex", cellComplex);
        }
    } else if (std::shared_ptr<gnomonCellImageSeries> cellImage = std::dynamic_pointer_cast<gnomonCellImageSeries>(form)) {
        if (d->acceptForms["gnomonCellImage"]) {
            this->setCellImage(cellImage, visualization);
        } else {
            emit badFormDropped("gnomonCellImage", acceptedForms().join(", "));
        }
    } else if (std::shared_ptr<gnomonImageSeries> image = std::dynamic_pointer_cast<gnomonImageSeries>(form)) {
        if (d->acceptForms["gnomonImage"]) {
            this->setImage(image, visualization);
        } else {
            emit badFormDropped("gnomonImage", acceptedForms().join(", "));
        }
    } else if (std::shared_ptr<gnomonMeshSeries> mesh = std::dynamic_pointer_cast<gnomonMeshSeries>(form)) {
        if (d->acceptForms["gnomonMesh"]) {
            this->setMesh(mesh, visualization);
        } else {
            this->setAdaptedForm("gnomonMesh", mesh);
        }
    } else if (std::shared_ptr<gnomonPointCloudSeries> pointCloud = std::dynamic_pointer_cast<gnomonPointCloudSeries>(form)) {
        if (d->acceptForms["gnomonPointCloud"]) {
            this->setPointCloud(pointCloud, visualization);
        } else {
            emit badFormDropped("gnomonPointCloud", acceptedForms().join(", "));
        }
    }
    return;
}

void gnomonViewForm::setAdaptedForm(const QString& name, std::shared_ptr<gnomonAbstractDynamicForm> form, gnomonAbstractVisualization *visualization)
{
    bool adapter_found = false;
    if (d->adapterCommands.contains(name)) {
        QVariantMap adapter_descs;
        for (const auto &key : d->adapterCommands[name].keys()) {
            if (d->acceptForms[d->adapterTargets[name][key]]) {
                adapter_descs[key] = d->adapterDescriptions[name][key];
                adapter_found = true;
            }
        }
        // TODO: Bind that to QML
        // if (adapter_descs.size() > 0) {
        //     d->form_to_adapt = form;
        //     d->adapter_menu = new gnomonFormAdapterMenu(adapter_descs);
        //     d->adapter_menu->setAttribute(Qt::WA_DeleteOnClose, true);
        //     d->adapter_menu->resize(dtkApp->window()->width() * 2/5, dtkApp->window()->height() - 40);
        //     d->adapter_menu->move(dtkApp->window()->frameGeometry().topLeft() + QPoint(86,0));
        //     d->adapter_menu->show();

        //     QObject *context = d->adapter_menu->rootObject();
        //     connect(context, SIGNAL(clicked(const QString&)), d, SLOT(adaptForm(const QString&)));

        //     QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(d->adapter_menu);
        // }
    }

    if(!adapter_found) {
        emit badFormDropped(name ,acceptedForms().join(", "));
    }
}

std::shared_ptr<gnomonCellImageSeries> gnomonViewForm::cellImage(void)
{
    if (d->forms.contains("gnomonCellImage")) {
        return std::dynamic_pointer_cast<gnomonCellImageSeries>(d->forms["gnomonCellImage"]);
    } else {
        return nullptr;
    }
}

void gnomonViewForm::setCellImage(std::shared_ptr<gnomonCellImageSeries> cellImage,const QJsonObject &visu_properties)
{
    d->forms["gnomonCellImage"] = cellImage;
    QString key = gnomonVisualization::visualizationCellImage::pluginFactory().keys()[0];
    if(visu_properties.contains("plugin_name")) {
        key = visu_properties["plugin_name"].toString();
    }

    QJsonObject parameters = {};
    if (visu_properties.contains("parameters")) {
        parameters = visu_properties["parameters"].toObject();
    }

    if ((!d->formVisualization.contains("gnomonCellImage"))||(!d->formVisualization["gnomonCellImage"]))
    {
        // d->formVisualization["gnomonCellImage"] = gnomonVisualization::visualizationCellImage::pluginFactory().create(key); 
        d->setFormVisualization("gnomonCellImage", key, parameters);
    } else {
        d->updateFormVisualization("gnomonCellImage", parameters);
    }
    emit formAdded("gnomonCellImage");
}

std::shared_ptr<gnomonCellComplexSeries> gnomonViewForm::cellComplex(void)
{
    if (d->forms.contains("gnomonCellComplex")) {
        return std::dynamic_pointer_cast<gnomonCellComplexSeries>(d->forms["gnomonCellComplex"]);
    } else {
        return nullptr;
    }
}

void gnomonViewForm::setCellComplex(std::shared_ptr<gnomonCellComplexSeries> cellComplex, const QJsonObject &visu_properties)
{
    // d->forms["gnomonCellComplex"] = cellComplex;

    // if ((!d->formVisualization.contains("gnomonCellComplex"))||(!d->formVisualization["gnomonCellComplex"]))
    // {
    //     QString key = gnomonVisualization::visualizationCellComplex::pluginFactory().keys()[0];
    //     d->setFormVisualization("gnomonCellComplex", key);
    // } else {
    //     d->updateFormVisualization("gnomonCellComplex");
    // }
    
    // emit formAdded("gnomonCellComplex");

    d->forms["gnomonCellComplex"] = cellComplex;

    QString key = gnomonVisualization::visualizationCellComplex::pluginFactory().keys()[0];
    if(visu_properties.contains("plugin_name")) {
        key = visu_properties["plugin_name"].toString();
    } 
    // d->formVisualization["gnomonCellComplex"] = gnomonVisualization::visualizationCellImage::pluginFactory().create(key); 
    QJsonObject parameters = {};
    if (visu_properties.contains("parameters")) {
        parameters = visu_properties["parameters"].toObject();
    }

    if ((!d->formVisualization.contains("gnomonCellComplex"))||(!d->formVisualization["gnomonCellComplex"]))
    {
        d->setFormVisualization("gnomonCellComplex", key, parameters);
    } else {
        d->updateFormVisualization("gnomonCellComplex", parameters);
    }

    emit formAdded("gnomonCellComplex");



}

std::shared_ptr<gnomonImageSeries> gnomonViewForm::image(void)
{
    if (d->forms.contains("gnomonImage")) {
        return std::dynamic_pointer_cast<gnomonImageSeries>(d->forms["gnomonImage"]);
    } else {
        return nullptr;
    }
}

void gnomonViewForm::setImage(std::shared_ptr<gnomonImageSeries> image, const QJsonObject &visu_properties)
{
    // d->forms["gnomonImage"] = image;
    
    // if ((!d->formVisualization.contains("gnomonImage"))||(!d->formVisualization["gnomonImage"]))
    // {
    //     QString key = gnomonVisualization::visualizationImage::pluginFactory().keys()[0];
    //     d->setFormVisualization("gnomonImage", key);
    // } else {
    //     d->updateFormVisualization("gnomonImage");
    // }

    // emit formAdded("gnomonImage");

    d->forms["gnomonImage"] = image;

    QString key = gnomonVisualization::visualizationImage::pluginFactory().keys()[0];
    if(visu_properties.contains("plugin_name")) {
        key = visu_properties["plugin_name"].toString();
    }

    QJsonObject parameters = {};
    if (visu_properties.contains("parameters")) {
        parameters = visu_properties["parameters"].toObject();
    }

    if ((!d->formVisualization.contains("gnomonImage"))||(!d->formVisualization["gnomonImage"]))
    {
        // d->formVisualization["gnomonImage"] = gnomonVisualization::visualizationImage::pluginFactory().create(key);
        d->setFormVisualization("gnomonImage", key, parameters);
    } else {
        d->updateFormVisualization("gnomonImage", parameters);
    }

    emit formAdded("gnomonImage");
}


std::shared_ptr<gnomonBinaryImageSeries> gnomonViewForm::binaryImage(void)
{
    if (d->forms.contains("gnomonBinaryImage")) {
        return std::dynamic_pointer_cast<gnomonBinaryImageSeries>(d->forms["gnomonBinaryImage"]);
    } else {
        return nullptr;
    }
}

void gnomonViewForm::setBinaryImage(std::shared_ptr<gnomonBinaryImageSeries> image, const QJsonObject &visu_properties) {
    // d->forms["gnomonBinaryImage"] = image;

    // if ((!d->formVisualization.contains("gnomonBinaryImage"))||(!d->formVisualization["gnomonBinaryImage"]))
    // {
    //     QString key = gnomonVisualization::visualizationBinaryImage::pluginFactory().keys()[0];
    //     d->setFormVisualization("gnomonBinaryImage", key);
    // } else {
    //     d->updateFormVisualization("gnomonBinaryImage");
    // }

    // emit formAdded("gnomonBinaryImage");

    d->forms["gnomonBinaryImage"] = image;
    
    QString key = gnomonVisualization::visualizationBinaryImage::pluginFactory().keys()[0];
    if(visu_properties.contains("plugin_name")) {
        key = visu_properties["plugin_name"].toString();
    }

    QJsonObject parameters = {};
    if (visu_properties.contains("parameters")) {
        parameters = visu_properties["parameters"].toObject();
    }

    if ((!d->formVisualization.contains("gnomonBinaryImage"))||(!d->formVisualization["gnomonBinaryImage"]))
    {
        // d->formVisualization["gnomonBinaryImage"] = gnomonVisualization::visualizationCellImage::pluginFactory().create(visu_properties["plugin_name"].toString()); 
        d->setFormVisualization("gnomonBinaryImage", key, parameters);
    } else {
        d->updateFormVisualization("gnomonBinaryImage", parameters);
    }

    emit formAdded("gnomonBinaryImage");

}

std::shared_ptr<gnomonMeshSeries> gnomonViewForm::mesh(void)
{
    if (d->forms.contains("gnomonMesh")) {
        return std::dynamic_pointer_cast<gnomonMeshSeries>(d->forms["gnomonMesh"]);
    } else {
        return nullptr;
    }
}

void gnomonViewForm::setMesh(std::shared_ptr<gnomonMeshSeries> mesh, const QJsonObject &visu_properties)
{
    // d->forms["gnomonMesh"] = mesh;


    // if ((!d->formVisualization.contains("gnomonMesh"))||(!d->formVisualization["gnomonMesh"]))
    // {
    //     QString key = gnomonVisualization::visualizationMesh::pluginFactory().keys()[0];
    //     d->setFormVisualization("gnomonMesh", key);
    // } else {
    //     d->updateFormVisualization("gnomonMesh");
    // }

    // emit formAdded("gnomonMesh");

    d->forms["gnomonMesh"] = mesh;

    QString key = gnomonVisualization::visualizationMesh::pluginFactory().keys()[0];
    if(visu_properties.contains("plugin_name")) {
        key = visu_properties["plugin_name"].toString();
    }

    QJsonObject parameters = {};
    if (visu_properties.contains("parameters")) {
        parameters = visu_properties["parameters"].toObject();
    }

    if ((!d->formVisualization.contains("gnomonMesh"))||(!d->formVisualization["gnomonMesh"]))
    {
        // d->formVisualization["gnomonMesh"] = gnomonVisualization::visualizationCellImage::pluginFactory().create(key); 
        d->setFormVisualization("gnomonMesh", key, parameters);
    } else {
        d->updateFormVisualization("gnomonMesh", parameters);
    }

    emit formAdded("gnomonMesh");
}

std::shared_ptr<gnomonPointCloudSeries> gnomonViewForm::pointCloud(void)
{
    if (d->forms.contains("gnomonPointCloud")) {
        return std::dynamic_pointer_cast<gnomonPointCloudSeries>(d->forms["gnomonPointCloud"]);
    } else {
        return nullptr;
    }
}

void gnomonViewForm::setPointCloud(std::shared_ptr<gnomonPointCloudSeries> pointCloud, const QJsonObject &visu_properties)
{
    // d->forms["gnomonPointCloud"] = pointCloud;
    
    // if ((!d->formVisualization.contains("gnomonPointCloud"))||(!d->formVisualization["gnomonPointCloud"]))
    // {
    //     QString key = gnomonVisualization::visualizationPointCloud::pluginFactory().keys()[0];
    //     d->setFormVisualization("gnomonPointCloud", key);
    // } else {
    //     d->updateFormVisualization("gnomonPointCloud");
    // }
    
    // emit formAdded("gnomonPointCloud");

    d->forms["gnomonPointCloud"] = pointCloud;

    QString key = gnomonVisualization::visualizationPointCloud::pluginFactory().keys()[0];
    if(visu_properties.contains("plugin_name")) {
        key = visu_properties["plugin_name"].toString();
    }

    QJsonObject parameters = {};
    if (visu_properties.contains("parameters")) {
        parameters = visu_properties["parameters"].toObject();
    }
    
    if ((!d->formVisualization.contains("gnomonPointCloud"))||(!d->formVisualization["gnomonPointCloud"]))
    {
        // d->formVisualization["gnomonPointCloud"] = gnomonVisualization::visualizationCellImage::pluginFactory().create(key); 
        d->setFormVisualization("gnomonPointCloud", key, parameters);
    } else {
        d->updateFormVisualization("gnomonPointCloud", parameters);
    }
    
    emit formAdded("gnomonPointCloud");

}

QString gnomonViewForm::formVisuName(const QString& name)
{
    QString visu_name;
    if (d->forms.contains(name)) {
        visu_name = d->formVisualizationNames[name];
    }
    return visu_name;
}

QStringList gnomonViewForm::formVisualizations(const QString& name)
{   
    QStringList visu_names;
    
    if (d->forms.contains(name)) {  
        if (name == "gnomonBinaryImage") {
             visu_names = gnomonVisualization::visualizationBinaryImage::pluginFactory().keys();
         } else if (name == "gnomonCellComplex") {
             visu_names = gnomonVisualization::visualizationCellComplex::pluginFactory().keys();
         } else if (name == "gnomonCellImage") {
             visu_names = gnomonVisualization::visualizationCellImage::pluginFactory().keys();
         } else if (name == "gnomonImage") {
             visu_names = gnomonVisualization::visualizationImage::pluginFactory().keys();
        } else if (name == "gnomonMesh") {
             visu_names = gnomonVisualization::visualizationMesh::pluginFactory().keys();
         } else if (name == "gnomonPointCloud") {
             visu_names = gnomonVisualization::visualizationPointCloud::pluginFactory().keys();
         }
    }
    
    return visu_names;
}

void gnomonViewForm::setFormVisuName(const QString& name, const QString& visu_name)
{
    if (d->forms.contains(name)) {
        d->setFormVisualization(name, visu_name);
    }
}

QJSValue gnomonViewForm::formVisuParameters(const QString& name)
{
    if (d->forms.contains(name)) {
        QJSValue parameters = dtkCoreParameterCollection(d->formVisualization[name]->parameters()).toJSValue(this->parent());
        QMap<QString, QString> parameter_groups = d->formVisualization[name]->parameterGroups();

        QJSValueIterator it(parameters);
        while (it.hasNext()) {
            it.next();
            QString group = parameter_groups.contains(it.name()) ? parameter_groups[it.name()] : "";
            it.value().setProperty("group", group != "" ? group : nullptr);
        }

        return parameters;
    } else {
        return QJSValue();
    }
}

void gnomonViewForm::setFormVisible(const QString& name, bool visible)
{
    if (d->formVisualization.contains(name)) {
        if (d->formVisualization[name]) {
            d->formVisualization[name]->setVisible(visible);
            d->formVisibility[name] = visible;
        }
    }

    this->render();
}

void gnomonViewForm::removeForm(const QString& name)
{
    if (d->formVisualization.contains(name)) {
        if (d->formVisualization[name]) {
            d->formVisualization[name]->disconnect();
            d->formVisualization[name]->clearConnections();
            d->formVisualization[name]->clear();
            // TODO: Fix offscreen rendering related segFault when destroying Visualization
            // delete d->formVisualization[name];
        }
    }
    d->formVisualization.remove(name);
    d->formVisualizationNames.remove(name);
    d->formVisibility.remove(name);
    d->forms.remove(name);

    d->updateFormsTimes();
    if (d->forms.isEmpty()) {
        d->empty = true;
    }

    this->render();

    emit formsChanged();
}

void gnomonViewForm::setBounds(double bounds[6])
{
    bool changed = false;

    if (bounds[0] != d->xBounds[0]) {
        d->xBounds[0] = bounds[0];
        changed = true;
    }
    if (bounds[1] != d->xBounds[1]) {
        d->xBounds[1] = bounds[1];
        changed = true;
    }
    if (bounds[2] != d->yBounds[0]) {
        d->yBounds[0] = bounds[2];
        changed = true;
    }
    if (bounds[3] != d->yBounds[1]) {
        d->yBounds[1] = bounds[3];
        changed = true;
    }
    if (bounds[4] != d->zBounds[0]) {
        d->zBounds[0] = bounds[4];
        changed = true;
    }
    if (bounds[5] != d->zBounds[1]) {
        d->zBounds[1] = bounds[5];
        changed = true;
    }

    if (changed) {
        emit boundsChanged();

//         d->c_x = (d->xBounds[0]+d->xBounds[1])/2;
//         d->c_y = (d->yBounds[0]+d->yBounds[1])/2;
//         d->c_z = (d->zBounds[0]+d->zBounds[1])/2;

//         d->cameras.clear();

//         switch(d->ori)
//         {
//             case gnomonViewFormPrivate::SLICE_ORIENTATION_XY:
//                 d->slice_slider->blockSignals(true);
//                 d->slice_slider->setMinimum(d->zBounds[0]);
//                 d->slice_slider->setMaximum(d->zBounds[1]);
//                 d->slice_slider->setValue(d->c_z);
//                 d->slice_slider->blockSignals(false);
//                 break;
//             case gnomonViewFormPrivate::SLICE_ORIENTATION_XZ:
//                 d->slice_slider->blockSignals(true);
//                 d->slice_slider->setMinimum(d->yBounds[0]);
//                 d->slice_slider->setMaximum(d->yBounds[1]);
//                 d->slice_slider->setValue(d->c_y);
//                 d->slice_slider->blockSignals(false);
//                 break;
//             case gnomonViewFormPrivate::SLICE_ORIENTATION_YZ:
//                 d->slice_slider->blockSignals(true);
//                 d->slice_slider->setMinimum(d->xBounds[0]);
//                 d->slice_slider->setMaximum(d->xBounds[1]);
//                 d->slice_slider->setValue(d->c_x);
//                 d->slice_slider->blockSignals(false);
//                 break;
//             default:
//                 break;
//         };

        d->renderer2D->ResetCamera();
        d->renderer3D->ResetCamera();
    }
}

void gnomonViewForm::setBounds(double xMin, double xMax, double yMin, double yMax, double zMin, double zMax)
{
    double bounds[6];
    bounds[0] = xMin;
    bounds[1] = xMax;
    bounds[2] = yMin;
    bounds[3] = yMax;
    bounds[4] = zMin;
    bounds[5] = zMax;
    this->setBounds(bounds);
}

void gnomonViewForm::getBounds(double bounds[6])
{
    bounds[0] = d->xBounds[0];
    bounds[1] = d->xBounds[1];
    bounds[2] = d->yBounds[0];
    bounds[3] = d->yBounds[1];
    bounds[4] = d->zBounds[0];
    bounds[5] = d->zBounds[1];
}

double gnomonViewForm::xMin(void) const
{
    return d->xBounds[0];
}

double gnomonViewForm::xMax(void) const
{
    return d->xBounds[1];
}

double gnomonViewForm::yMin(void) const
{
    return d->yBounds[0];
}

double gnomonViewForm::yMax(void) const
{
    return d->yBounds[1];
}

double gnomonViewForm::zMin(void) const
{
    return d->zBounds[0];
}

double gnomonViewForm::zMax(void) const
{
    return d->zBounds[1];
}

gnomonViewForm::Mode gnomonViewForm::mode(void) const
{
    return d->mode;
}

void gnomonViewForm::setCamera(vtkCamera *cam)
{
    vtkSmartPointer<vtkCamera> camera3D = d->renderer3D->GetActiveCamera();
//    camera3D->DeepCopy(cam);
    camera3D->SetFocalPoint(cam->GetFocalPoint());
    camera3D->SetViewUp(cam->GetViewUp());
    camera3D->SetPosition(cam->GetPosition());
}

void gnomonViewForm::setAcceptForm(const QString& name, bool accept)
{
    if (d->acceptForms.contains(name)) {
        d->acceptForms[name] = accept;
    }
}

QStringList gnomonViewForm::acceptedForms(void)
{
    QStringList forms;
    for (const auto& name : d->acceptForms.keys()) {
        if (d->acceptForms[name]) {
            forms << name;
        }
    }
    return forms;
}


void gnomonViewForm::setEnableLinking(bool enable)
{
    d->enableLink = enable;
    // d->refresh();
}

void gnomonViewForm::setEnableMenus(bool enable)
{
    d->enableMenus = enable;
    // d->refresh();
}

vtkRenderWindowInteractor *gnomonViewForm::interactor(void)
{
    return d->interactor();
}

vtkRenderer *gnomonViewForm::renderer2D(void)
{
    return d->renderer2D;
}

vtkRenderer *gnomonViewForm::renderer3D(void)
{
    return d->renderer3D;
}

// dtkWidgetsMenu *gnomonViewForm::menu(void)
// {
//     return d->menu();
// }

// dtkWidgetsMenuBar *gnomonViewForm::menubar(void)
// {
//     return d->view_menubar;
// }

gnomonViewForm::Orientation gnomonViewForm::orientation(void)
{
    return d->ori;
}

void gnomonViewForm::render(void)
{
    d->renderer2D->ResetCameraClippingRange();
    d->interactor()->Render();
}

void gnomonViewForm::update(void)
{
     for (const auto& key : d->formVisualization.keys()) {
         gnomonAbstractVisualization *v = d->formVisualization[key];
         if(v) {
             v->update();
             v->setVisible(d->formVisibility[key]);
         }
     }
}

void gnomonViewForm::clear(void)
{
    d->clear();
    d->updateFormsTimes();
    d->empty = true;
    emit formsChanged();
}

void gnomonViewForm::onSliceChanged(int slice)
{
    // d->slice_slider->setValue(slice);

    Q_UNUSED(slice);
}

void gnomonViewForm::onTimeChanged(double time)
{
    QList<double> sorted_times = QList<double>(d->forms_times.begin(), d->forms_times.end());

    if (sorted_times.contains(time)) {
        std::sort(sorted_times.begin(), sorted_times.end());
        int value = sorted_times.indexOf(time);
        this->setCurrentTime(value);
    }
}

void gnomonViewForm::setInputView(bool input)
{
    if (input != d->input_view) {
        d->input_view = input;
        emit inputViewChanged();
    }
}

void gnomonViewForm::setInPool(bool inpool)
{
    d->in_pool = inpool;
}

bool gnomonViewForm::inputView(void)
{
    return d->input_view;
}

bool gnomonViewForm::inPool(void)
{
    return d->in_pool;
}

bool gnomonViewForm::synced(void)
{
    return d->synced;
}

bool gnomonViewForm::syncing(void)
{
    return d->syncing;
}

void gnomonViewForm::setInteractorStyle(gnomonInteractorStyle *style)
{
    gnomonInteractorStyle *new_style;

    if (style) {
        new_style = style;
    } else {
        new_style = d->default_style;
    }

    if (d->style) {
        d->style->disable();
    }

    d->style = new_style;

    this->interactor()->SetInteractorStyle(d->style);

    d->style->setView(this);

    // if (d->renderer3D_button->isToggled()) {
         d->style->setMode("3D");
         d->style->SetDefaultRenderer(this->renderer3D());
    // } else {
    //     d->style->setMode("2D");
    //     d->style->SetDefaultRenderer(this->renderer2D());
    // }

#if !defined(Q_OS_LINUX)
    this->interactor()->Enable();
#endif

    // d->updateKeys();
}

void gnomonViewForm::updateShortcutKeys(void)
{
    // d->updateKeys();
}

// void gnomonViewForm::dragEnterEvent(QDragEnterEvent *event)
// {
//     if (event->mimeData()->hasText()) {
//         event->accept();
//         return;
//     }

//     event->ignore();
// }

// void gnomonViewForm::dragLeaveEvent(QDragLeaveEvent *event)
// {
//     event->accept();
// }

// void gnomonViewForm::dragMoveEvent(QDragMoveEvent *event)
// {
//     event->accept();
// }

// void gnomonViewForm::dropEvent(QDropEvent *event)
// {
//     QString path = event->mimeData()->text();

//     if (path.startsWith(":")) {
//         int form_index = path.remove(":").toInt();
//         gnomonAbstractDynamicForm *form = gnomonFormManager::instance()->get(form_index);
//         if (d->empty) {
//             if (vtkCamera *cam = gnomonFormManager::instance()->getCamera(form_index)) {
//                 this->setCamera(cam);
//             }
//         }
//         this->setForm("formManager", form, gnomonFormManager::instance()->getVisualization(form_index));

//         event->accept();
//     } else if (path.startsWith("file://")) {
//         emit fileDropped(path);
//         event->accept();
//     }
//     // ///////////////////////////////////////////////////////////////

// }

void gnomonViewForm::drop(int index)
{
    std::shared_ptr<gnomonAbstractDynamicForm> form = gnomonFormManager::instance()->get(index);

    if (d->empty) {
        if (vtkCamera *cam = gnomonFormManager::instance()->getCamera(index)) {
            this->setCamera(cam);
        }
    }

    QJsonObject visu_descr = gnomonFormManager::instance()->getVisuDescription(index);
    
    this->setForm("formManager", form, visu_descr);

    d->interactor()->Render();
}

gnomonDynamicFormMetadata* gnomonViewForm::formMetadata(const QString &name) {
    gnomonDynamicFormMetadata *ptr = d->forms[name]->metadata();
    QQmlEngine::setObjectOwnership(ptr, QQmlEngine::CppOwnership);
    return ptr;
}

// void gnomonViewForm::resizeEvent(QResizeEvent *event)
// {
//       d->style_menubar->setFixedHeight(32*(1+d->available_styles.size())+32);
//       d->style_menubar->move(QPoint(0, 32*(1+d->formVisualizationMenus.size())+32));
// }

// ///////////////////////////////////////////////////////////////////

#include "gnomonViewForm.moc"

//
// gnomonViewForm.cpp ends here
