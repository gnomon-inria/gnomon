#include "gnomonVtkView.h"
#include "gnomonAbstractView_p.h"

#include <gnomonCore/gnomonAlgorithm/gnomonMesh/gnomonAbstractMeshAdapter>
#include <gnomonCore/gnomonAlgorithm/gnomonCellComplex/gnomonAbstractCellComplexAdapter>
#include <gnomonCore/gnomonCommand/gnomonCellComplex/gnomonCellComplexAdapterCommand>
#include <gnomonCore/gnomonCommand/gnomonMesh/gnomonMeshAdapterCommand>
#include <gnomonCore/gnomonPythonPluginLoader.h>

#include "gnomonManager/gnomonFormManager.h"
#include "gnomonPluginFactory.h"
#include "gnomonVisualizations/gnomonCellComplex/gnomonAbstractCellComplexVtkVisualization.h"
#include "gnomonVisualizations/gnomonCellImage/gnomonAbstractCellImageVtkVisualization.h"
#include "gnomonVisualizations/gnomonImage/gnomonAbstractImageVtkVisualization.h"
#include "gnomonVisualizations/gnomonBinaryImage/gnomonAbstractBinaryImageVtkVisualization.h"
#include "gnomonVisualizations/gnomonLString/gnomonAbstractLStringVtkVisualization.h"
#include "gnomonVisualizations/gnomonMesh/gnomonAbstractMeshVtkVisualization.h"
#include "gnomonVisualizations/gnomonPointCloud/gnomonAbstractPointCloudVtkVisualization.h"

#include <gnomonVisualization/gnomonCommand/gnomonAbstractVisualizationCommand>
#include <gnomonVisualization/gnomonCommand/gnomonBinaryImage/gnomonBinaryImageVtkVisualizationCommand>
#include <gnomonVisualization/gnomonCommand/gnomonCellComplex/gnomonCellComplexVtkVisualizationCommand>
#include <gnomonVisualization/gnomonCommand/gnomonCellImage/gnomonCellImageVtkVisualizationCommand>
#include <gnomonVisualization/gnomonCommand/gnomonImage/gnomonImageVtkVisualizationCommand>
#include <gnomonVisualization/gnomonCommand/gnomonLString/gnomonLStringVtkVisualizationCommand>
#include <gnomonVisualization/gnomonCommand/gnomonMesh/gnomonMeshVtkVisualizationCommand>
#include <gnomonVisualization/gnomonCommand/gnomonPointCloud/gnomonPointCloudVtkVisualizationCommand>

#include "gnomonInteractorStyle/gnomonInteractorStyle.h"

#include <memory>
#include <vtkCamera.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkInteractorObserver.h>
#include <vtkInteractorStyle.h>
#include <vtkInteractorStyleImage.h>
#include <vtkPNGWriter.h>
#include <vtkRenderer.h>
#include <vtkRendererCollection.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkWindowToImageFilter.h>

// #include <QVTKInteractor.h>
// #include <QVTKOpenGLNativeWidget.h>


// ///////////////////////////////////////////////////////////////////
// gnomonVtkViewPrivate
// ///////////////////////////////////////////////////////////////////

class gnomonVtkViewPrivate : public QObject // QVTKOpenGLNativeWidget
{
    Q_OBJECT

public:
    typedef struct {
        QString currentFormType;
        int currentFormIndex = -1;
        QMap<QString, QString> visuSelected;  // formType --> visu_name
        QMap<QString, QVariantMap> parameters; // visu_name --> parameters

    } VtkViewParameters;

public:
     gnomonVtkViewPrivate(QObject *parent = Q_NULLPTR);
    ~gnomonVtkViewPrivate(void);

public:
    vtkRenderWindowInteractor *interactor(void)
    {
        return this->window->GetInteractor();
    }

public:
    void setViewMode(gnomonVtkView::Mode mode);
    void setSliceOrientation(gnomonVtkView::Orientation orientation);
    void updateOrientation(void);

public:
    void updateFormsTimes(void);

public:
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> window;
    vtkSmartPointer<vtkRenderer> renderer2D;
    vtkSmartPointer<vtkRenderer> renderer3D;

public:
    gnomonVtkView *q = nullptr;

public:
    gnomonVtkView::Mode mode = gnomonVtkView::VIEW_MODE_3D;
    gnomonVtkView::Orientation ori = gnomonVtkView::NONE;
    QMap<gnomonVtkView::Orientation, vtkSmartPointer<vtkCamera> > cameras;

public:
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > to_export;

    QMap<QString, QString> formVisualizationNames;
    QMap<QString, std::shared_ptr<gnomonAbstractVtkVisualization> > formVisualization;
    QMap<QString, QJsonObject > visualization_description;
    QMap<QString, bool > formVisibility;
    VtkViewParameters viewParameters;


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
    vtkSmartPointer<vtkInteractorObserver> old_style = nullptr;
    std::shared_ptr<gnomonAbstractCellImageVtkVisualization> picking_visu = nullptr;
    QMetaObject::Connection connectPicked;

public:
    int syncing_count = 0; QTimer *syncing_timer = nullptr; bool synced = false; bool syncing = false;

public:
    bool enableLink = false;
    bool enableMenus = true;

public:
    bool in_pool = false;

public:
    QSet<double> forms_times;
    QList<long> picked_cells;

public:
    double xBounds[2] = {0,0}, yBounds[2] = {0,0}, zBounds[2] = {0,0};
    double c_x = 0, c_y = 0, c_z = 0;

public:
    double c_t = 0;

signals:
    void sliceOrientationChanged(int);

public slots:
    void setFormVisualization(const QString& formType, const QString& visu_name, const QVariantMap &parameters = {});

public:
    QMap<QString, QMap<QString, gnomonAbstractAdapterCommand *> > adapterCommands;
    QMap<QString, QMap<QString, QString> > adapterTargets;
    QMap<QString, QMap<QString, QString> > adapterDescriptions;

    std::shared_ptr<gnomonAbstractDynamicForm> form_to_adapt = nullptr;

public slots:
    void adaptForm(const QString& adapter_plugin);
};

gnomonVtkViewPrivate::gnomonVtkViewPrivate(QObject *parent) : QObject(parent)
{
    // TODO: expose as a view parameter
    QColor background_color = QColor("#00000000");

    this->renderer2D = vtkSmartPointer<vtkRenderer>::New();
    this->renderer2D->SetBackground(background_color.redF(), background_color.greenF(), background_color.blueF());

    this->renderer3D = vtkSmartPointer<vtkRenderer>::New();
    this->renderer3D->SetBackground(background_color.redF(), background_color.greenF(), background_color.blueF());

    static int count = 0;
    viewParameters.currentFormIndex = -1;
}

gnomonVtkViewPrivate::~gnomonVtkViewPrivate(void)
{
    this->clearConnections();
}

/* void gnomonVtkViewPrivate::exportToManager(void)
{
    for (const auto& key : this->forms.keys()) {
        QImage image = this->formVisualization[key]->imageRendering();
        gnomonFormManager::instance()->addForm(this->forms[key], this->formVisualization[key],image, this->renderer3D->GetActiveCamera());
        q->emit exportedForm(this->forms[key]);
    }
} */

void gnomonVtkViewPrivate::clearConnections(void)
{
    disconnect(this->connect3D);
    disconnect(this->connect2D);
    disconnect(this->connectXY);
    disconnect(this->connectYZ);
    disconnect(this->connectXZ);
    disconnect(this->connectSlice);
    disconnect(this->connectTime);
}

void gnomonVtkViewPrivate::setViewMode(gnomonVtkView::Mode mode)
{
    this->mode = mode;

    if (this->mode == gnomonVtkView::VIEW_MODE_3D) {
        this->renderer2D->DrawOff();
        this->renderer2D->InteractiveOff();

        this->renderer3D->InteractiveOn();
        this->renderer3D->DrawOn();
    } else if (this->mode == gnomonVtkView::VIEW_MODE_2D) {
        this->renderer3D->DrawOff();
        this->renderer3D->InteractiveOff();

        this->renderer2D->InteractiveOn();
        this->renderer2D->DrawOn();
    }
}

void gnomonVtkViewPrivate::setSliceOrientation(gnomonVtkView::Orientation orientation)
{
    this->ori = orientation;
    this->updateOrientation();
}


void gnomonVtkViewPrivate::updateOrientation(void)
{
    // if(!this->cameras.contains(this->ori)) {
    if(true) {

        vtkSmartPointer<vtkCamera> cam = vtkCamera::New();
        cam->ParallelProjectionOn();
        cam->SetParallelScale(1);
        cam->SetFocalPoint((xBounds[0]+xBounds[1])/2,(yBounds[0]+yBounds[1])/2,(zBounds[0]+zBounds[1])/2);

        switch(this->ori)
        {
            case gnomonVtkView::SLICE_ORIENTATION_XY:
                cam->SetPosition((xBounds[0]+xBounds[1])/2,(yBounds[0]+yBounds[1])/2,zBounds[1]);
                cam->SetViewUp(0,1,0);
                cam->SetClippingRange((zBounds[1] - zBounds[0]) - 3.0, (zBounds[1] - zBounds[0]) + 3.0);
                break;

            case gnomonVtkView::SLICE_ORIENTATION_XZ:
                cam->SetPosition((xBounds[0]+xBounds[1])/2,yBounds[0],(zBounds[0]+zBounds[1])/2);
                cam->SetViewUp(0,0,1);
                cam->SetClippingRange((yBounds[1] - yBounds[0]) - 3.0, (yBounds[1] - yBounds[0]) + 3.0);
                break;

            case gnomonVtkView::SLICE_ORIENTATION_YZ:
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

void gnomonVtkViewPrivate::setFormVisualization(const QString& formType, const QString& visu_name, const QVariantMap &parameters)
{
    // saving current parameters before change
    QMap<QString, std::shared_ptr<gnomonAbstractVtkVisualization>>::iterator i;
    for (i = formVisualization.begin(); i != formVisualization.end(); ++i) {
        const auto& _visu_name = formVisualizationNames[i.key()];
        viewParameters.parameters[_visu_name] = i.value()->visuParameters();
    }

    auto visu_parameters = parameters;
    if(viewParameters.parameters.contains(visu_name) && parameters.size()==0) {
        visu_parameters = viewParameters.parameters[visu_name];
    }

    q->d->visualizationCommands[formType]->setForm(q->form(formType));
    q->d->visualizationCommands[formType]->setFormVisualization(visu_name, visu_parameters);
    auto&& visu = q->d->visualizationCommands[formType]->visualization();
    this->formVisualization[formType] = std::static_pointer_cast<gnomonAbstractVtkVisualization>(visu);
    emit q->formVisualizationChanged();

    this->formVisualizationNames[formType] = visu_name;
    if (!this->formVisibility.contains(formType)) {
        this->formVisibility[formType] = true;
    }

    viewParameters.visuSelected[formType] = visu_name;

    connect(visu.get(), &gnomonAbstractVtkVisualization::parametersChanged, [=] () {
        emit q->formVisuParametersChanged();
    });

    q->formVisuParametersChanged();
    q->switchTo3D();
}

void gnomonVtkViewPrivate::updateFormsTimes(void)
{
    this->forms_times.clear();

    for (const auto& key : q->formNames()) {
        for(auto time : q->form(key)->times()) {
            this->forms_times.insert(time);
        }
    }

    emit q->timeMaxChanged(q->timeMax());
    q->timesChanged();
}

void gnomonVtkViewPrivate::adaptForm(const QString& adapter_plugin)
{
    std::shared_ptr<gnomonAbstractDynamicForm> form = this->form_to_adapt;

    if (std::shared_ptr<gnomonMeshSeries> mesh = std::dynamic_pointer_cast<gnomonMeshSeries>(form)) {
        gnomonMeshAdapterCommand *meshCommand = dynamic_cast<gnomonMeshAdapterCommand *>(this->adapterCommands["gnomonMesh"][adapter_plugin]);

        meshCommand->setInput(mesh);
        meshCommand->redo();
        std::shared_ptr<gnomonAbstractDynamicForm> adaptedMesh = meshCommand->output();
        if (adaptedMesh) {
            // gnomonPipelineManager::instance()->addAdapter(meshCommand);
            //TODO create visu
            qWarning() << Q_FUNC_INFO << "Not implemented";
            //q->setForm("adaptedMesh",adaptedMesh);
        }
    } else if (std::shared_ptr<gnomonCellComplexSeries> cellComplex = std::dynamic_pointer_cast<gnomonCellComplexSeries>(form)) {
        gnomonCellComplexAdapterCommand *cellComplexCommand = dynamic_cast<gnomonCellComplexAdapterCommand *>(this->adapterCommands["gnomonCellComplex"][adapter_plugin]);

        cellComplexCommand->setInput(cellComplex);
        cellComplexCommand->redo();
        std::shared_ptr<gnomonAbstractDynamicForm> adaptedCellComplex = cellComplexCommand->output();
        if (adaptedCellComplex) {
            // gnomonPipelineManager::instance()->addAdapter(cellComplexCommand);
            //q->setForm("adaptedCellComplex",adaptedCellComplex);
            qWarning() << Q_FUNC_INFO << "Not implemented";
        }
    }

    // if (this->adapter_menu) {
    //     this->adapter_menu->close();
    // }
}

// ///////////////////////////////////////////////////////////////////
// gnomonVtkView
// ///////////////////////////////////////////////////////////////////

gnomonVtkView::gnomonVtkView(QObject *parent) : gnomonAbstractView(parent)
{
    dd = new gnomonVtkViewPrivate;
    dd->q = this;

    d->visualizationCommands["gnomonBinaryImage"] = new gnomonBinaryImageVtkVisualizationCommand;
    d->visualizationCommands["gnomonCellComplex"] = new gnomonCellComplexVtkVisualizationCommand;
    d->visualizationCommands["gnomonCellImage"] = new gnomonCellImageVtkVisualizationCommand;
    d->visualizationCommands["gnomonImage"] = new gnomonImageVtkVisualizationCommand;
    d->visualizationCommands["gnomonLString"] = new gnomonLStringVtkVisualizationCommand;
    d->visualizationCommands["gnomonMesh"] = new gnomonMeshVtkVisualizationCommand;
    d->visualizationCommands["gnomonPointCloud"] = new gnomonPointCloudVtkVisualizationCommand;
    for (const auto &formType: d->visualizationCommands.keys()) {
        d->visualizationCommands[formType]->setView(this);
        d->acceptForms[formType] = false;
    }

    for (const auto& form : d->visualizationCommands.keys()) {
        if (form=="gnomonMesh") {
            loadPluginGroup("meshAdapter");
            for (const auto& key : gnomonCore::meshAdapter::pluginFactory().keys())
            {
                gnomonAbstractMeshAdapter *adapter = dynamic_cast<gnomonAbstractMeshAdapter *>(gnomonCore::meshAdapter::pluginFactory().create(key));
                if (! dd->adapterCommands.contains(form))
                {
                    QMap<QString, QString> empty_target;
                     dd->adapterTargets[form] = empty_target;
                    QMap<QString, QString> empty_desc;
                     dd->adapterDescriptions[form] = empty_desc;
                    QMap<QString, gnomonAbstractAdapterCommand *> empty_list;
                     dd->adapterCommands[form] = empty_list;
                }
                 dd->adapterTargets[form][key] = adapter->target();
                 dd->adapterDescriptions[form][key] = adapter->documentation().split("\n")[1];
                 dd->adapterCommands[form][key] = new gnomonMeshAdapterCommand;
                 dd->adapterCommands[form][key]->setAlgorithmName(key);
                delete adapter;
            }
        } else if (form=="gnomonCellComplex") {
            loadPluginGroup("cellComplexAdapter");
            for (const auto& key : gnomonCore::cellComplexAdapter::pluginFactory().keys())
            {
                gnomonAbstractCellComplexAdapter *adapter = dynamic_cast<gnomonAbstractCellComplexAdapter *>(gnomonCore::cellComplexAdapter::pluginFactory().create(key));
                if (! dd->adapterCommands.contains(form))
                {
                    QMap<QString, QString> empty_target;
                     dd->adapterTargets[form] = empty_target;
                    QMap<QString, QString> empty_desc;
                     dd->adapterDescriptions[form] = empty_desc;
                    QMap<QString, gnomonAbstractAdapterCommand *> empty_list;
                     dd->adapterCommands[form] = empty_list;
                }
                 dd->adapterTargets[form][key] = adapter->target();
                 dd->adapterDescriptions[form][key] = adapter->documentation().split("\n")[1];
                 dd->adapterCommands[form][key] = new gnomonCellComplexAdapterCommand;
                 dd->adapterCommands[form][key]->setAlgorithmName(key);
                delete adapter;
            }
        }
    }

     connect(this, &gnomonVtkView::formAdded, [=] (const QString& key) {
         dd->updateFormsTimes();
         emit formsChanged();
     });

    // just need to find a signal that's actually emitted when a parameter changes :|
    connect(this, &gnomonVtkView::formVisuParametersChanged, [=] () {
        QMap<QString, std::shared_ptr<gnomonAbstractVtkVisualization>>::iterator i;
        for (i =  dd->formVisualization.begin(); i !=  dd->formVisualization.end(); ++i) {
            const auto& visu_name =  dd->formVisualizationNames[i.key()];
            dd->viewParameters.parameters[visu_name] = i.value()->visuParameters();
        }
    });
}

/*void gnomonVtkView::transmit(void)
{
    d->exportToManager();
}*/

void gnomonVtkView::restoreState(void)
{
    if (!this->empty()) {
        for (const auto &key :  dd->formVisualization.keys()) {
             dd->formVisualization[key]->clearConnections();
             dd->formVisualization[key]->clear();
             dd->formVisualization[key]->setView(this);
            // dd->formVisualization[d->viewParameters.visuSelected[key]]->setVisuParameters(d->viewParameters.parameters[key]->);
             dd->formVisualization[key]->update();
             dd->formVisualization[key]->setVisible( dd->formVisibility[key]);
        }
    }
}

void gnomonVtkView::associate(vtkGenericOpenGLRenderWindow *window)
{
    dd->window = window;
    dd->window->AddRenderer(dd->renderer2D);
    dd->window->AddRenderer(dd->renderer3D);

    this->switchTo2D();
    this->switchTo2DXY();
    this->switchTo3D();

    dd->updateOrientation();
    dd->updateFormsTimes();
}

gnomonVtkView::~gnomonVtkView(void)
{
    for(auto&& form_visu :  dd->formVisualization) {
        if(form_visu && form_visu->view() == this)
            form_visu->clear();
    }
     dd->formVisualization.clear();
    delete dd;
}

void gnomonVtkView::switchTo3D(void)
{
    bool hasChanged = dd->mode != gnomonVtkView::VIEW_MODE_3D;
    dd->setViewMode(gnomonVtkView::VIEW_MODE_3D);

    if (hasChanged){
        emit switchedTo3D();
        emit modeChanged();
    }
}

void gnomonVtkView::switchTo2D(void)
{
    bool hasChanged = dd->mode != gnomonVtkView::VIEW_MODE_2D;
    dd->setViewMode(gnomonVtkView::VIEW_MODE_2D);

    if (hasChanged) {
        switch(dd->ori) {
            case gnomonVtkView::SLICE_ORIENTATION_XY:
                this->switchTo2DXY();
                break;

            case gnomonVtkView::SLICE_ORIENTATION_XZ:
                this->switchTo2DXZ();
                break;

            case gnomonVtkView::SLICE_ORIENTATION_YZ:
                this->switchTo2DYZ();
                break;

            default:
                break;
        }

        emit switchedTo2D();
        emit modeChanged();
    }
}

void gnomonVtkView::switchTo2DXY(void)
{
    emit sliceOrientationChanged(gnomonVtkView::SLICE_ORIENTATION_XY);
    emit sliceChanged(dd->c_z);

    bool hasChanged = dd->ori != gnomonVtkView::SLICE_ORIENTATION_XY;
    dd->setSliceOrientation(gnomonVtkView::SLICE_ORIENTATION_XY);

    if (hasChanged) {
        emit switchedTo2DXY();
        emit orientationChanged();
    }

}

void gnomonVtkView::switchTo2DXZ(void)
{
    emit sliceOrientationChanged(gnomonVtkView::SLICE_ORIENTATION_XZ);
    emit sliceChanged(dd->c_y);

    bool hasChanged = dd->ori != gnomonVtkView::SLICE_ORIENTATION_XZ;
    dd->setSliceOrientation(gnomonVtkView::SLICE_ORIENTATION_XZ);

    if (hasChanged)
        emit switchedTo2DXZ();
        emit orientationChanged();
}

void gnomonVtkView::switchTo2DYZ(void)
{
    emit sliceOrientationChanged(gnomonVtkView::SLICE_ORIENTATION_YZ);
    emit sliceChanged(dd->c_x);

    bool hasChanged = dd->ori != gnomonVtkView::SLICE_ORIENTATION_YZ;
    dd->setSliceOrientation(gnomonVtkView::SLICE_ORIENTATION_YZ);

    if (hasChanged)
        emit switchedTo2DYZ();
        emit orientationChanged();
}

void gnomonVtkView::sliceChange(int value)
{
    bool valueChanged = false;

    switch(dd->ori)
    {
        case gnomonVtkView::SLICE_ORIENTATION_XY:
            if (dd->c_z != value) {
                dd->c_z = value;
                valueChanged = true;
            }
            break;
        case gnomonVtkView::SLICE_ORIENTATION_XZ:
            if (dd->c_y != value) {
                dd->c_y = value;
                valueChanged = true;
            }
            break;
        case gnomonVtkView::SLICE_ORIENTATION_YZ:
            if (dd->c_x != value){
                dd->c_x = value;
                valueChanged = true;
            }
            break;
        default:
            break;
    };

    if (valueChanged)
        emit sliceChanged(value);

    dd->interactor()->Render();
}


void gnomonVtkView::setCurrentTime(double time)
{
    QList<double> sorted_times = this->times();
    if(sorted_times.contains(time)) {
        if (dd->c_t != time) {
            dd->c_t = time;
            emit timeChanged(dd->c_t);
        }
    }
}

double gnomonVtkView::currentTime(void) const
{
    return dd->c_t;
}

double gnomonVtkView::timeMax(void)
{
    QList<double> times = this->times();
    if (times.isEmpty()) {
        return -1;
    }  else {
        return this->times().last();
    }
}

QList<double> gnomonVtkView::times(void)
{
    QList<double> sorted_times = QList<double>(dd->forms_times.begin(), dd->forms_times.end());
    std::sort(sorted_times.begin(), sorted_times.end());

    return sorted_times;
}

void gnomonVtkView::setPickedCells(QList<long> new_list)
{
    dd->picked_cells = new_list;
    emit pickedCellsChanged();
}

QList<long> gnomonVtkView::pickedCells(void)
{
    return dd->picked_cells;
}

void gnomonVtkView::tryLinking(void)
{
    dd->syncing = !dd->syncing;

    if (dd->syncing) {
        emit linking();
    } else {
        emit unlinking();
    }

    emit syncingChanged();
}

void gnomonVtkView::link(gnomonVtkView *other)
{
    dd->synced = true;

    if (dd->mode == gnomonVtkView::VIEW_MODE_3D) {
        other->switchTo3D();
    } else if (dd->mode == gnomonVtkView::VIEW_MODE_2D) {
        other->switchTo2D();
        if (dd->ori == gnomonVtkView::SLICE_ORIENTATION_XY) {
            other->switchTo2DXY();
        } else if (dd->ori == gnomonVtkView::SLICE_ORIENTATION_XZ) {
            other->switchTo2DXZ();
        } else if (dd->ori == gnomonVtkView::SLICE_ORIENTATION_YZ) {
            other->switchTo2DYZ();
        }
    }

    dd->renderer2D->SetActiveCamera(other->dd->renderer2D->GetActiveCamera());
    dd->renderer3D->SetActiveCamera(other->dd->renderer3D->GetActiveCamera());

    other->dd->window->AddObserver(vtkCommand::RenderEvent, this, &gnomonVtkView::render);
    this->render();

    dd->clearConnections();
    dd->connect3D = connect(other, &gnomonVtkView::switchedTo3D, [=] () {
        this->switchTo3D();
        dd->renderer3D->SetActiveCamera(other->dd->renderer3D->GetActiveCamera());
    });
    dd->connect2D = connect(other, &gnomonVtkView::switchedTo2D, [=] () {
        this->switchTo2D();
        if (other->dd->ori == gnomonVtkView::SLICE_ORIENTATION_XY) {
            this->switchTo2DXY();
        } else if (other->dd->ori == gnomonVtkView::SLICE_ORIENTATION_XZ) {
            this->switchTo2DXZ();
        } else if (other->dd->ori == gnomonVtkView::SLICE_ORIENTATION_YZ) {
            this->switchTo2DYZ();
        }
        dd->renderer2D->SetActiveCamera(other->dd->renderer2D->GetActiveCamera());
    });
    dd->connectXY = connect(other, &gnomonVtkView::switchedTo2DXY, [=] () {
        this->switchTo2DXY();
        dd->renderer2D->SetActiveCamera(other->dd->renderer2D->GetActiveCamera());
    });
    dd->connectXZ = connect(other, &gnomonVtkView::switchedTo2DXZ, [=] () {
        this->switchTo2DXZ();
        dd->renderer2D->SetActiveCamera(other->dd->renderer2D->GetActiveCamera());
    });
    dd->connectYZ = connect(other, &gnomonVtkView::switchedTo2DYZ, [=] () {
        this->switchTo2DYZ();
        dd->renderer2D->SetActiveCamera(other->dd->renderer2D->GetActiveCamera());
    });
    dd->connectSlice = connect(other, &gnomonVtkView::sliceChanged, [=] (int value) {
        this->sliceChange(value);
    });
    dd->connectTime = connect(other, &gnomonVtkView::timeChanged, [=] (double value) {
        this->onTimeChanged(value);
    });

    emit syncedChanged();
}

void gnomonVtkView::unlink(gnomonVtkView *other)
{
    dd->synced = false;

    vtkSmartPointer<vtkCamera> camera2D = vtkCamera::New();
    camera2D->ShallowCopy(dd->renderer2D->GetActiveCamera());
    dd->renderer2D->SetActiveCamera(camera2D);

    vtkSmartPointer<vtkCamera> camera3D = vtkCamera::New();
    camera3D->ShallowCopy(dd->renderer3D->GetActiveCamera());
    dd->renderer3D->SetActiveCamera(camera3D);

    dd->clearConnections();

    emit syncedChanged();
}

void gnomonVtkView::disconnectTime() {
    disconnect(dd->connectTime);
}

void gnomonVtkView::setForm(const QString& name, std::shared_ptr<gnomonAbstractDynamicForm> form, std::shared_ptr<gnomonAbstractVtkVisualization> visualization)
{
    QString form_name = form->formName();
    if (d->acceptForms[form_name]) {
        QString visu_name;
        QVariantMap parameters;

        if(visualization) {
            visu_name = visualization->pluginName();
            parameters = visualization->visuParameters();
        } else {
            if (dd->formVisualization.contains(form_name) &&  dd->formVisualization[form_name]) {
                std::shared_ptr<gnomonAbstractVtkVisualization> current_visu =  dd->formVisualization[form_name];
                visu_name = current_visu->pluginName();
                parameters = current_visu->visuParameters();
            } else {
                visu_name = d->visualizationCommands[form_name]->algorithmName();
            }
        }

        d->forms[form_name] = form;
        dd->setFormVisualization(form_name, visu_name, parameters);
        emit formAdded(form_name);
    } else {
        emit badFormDropped(form->formName(), acceptedForms().join(", "));
    }
    return;
}

void gnomonVtkView::setAdaptedForm(const QString& name, std::shared_ptr<gnomonAbstractDynamicForm> form)
{
    bool adapter_found = false;
    if ( dd->adapterCommands.contains(name)) {
        QVariantMap adapter_descs;
        for (const auto &key :  dd->adapterCommands[name].keys()) {
            if (d->acceptForms[ dd->adapterTargets[name][key]]) {
                adapter_descs[key] =  dd->adapterDescriptions[name][key];
                adapter_found = true;
            }
        }

        qWarning() << Q_FUNC_INFO << "Not implemented";
        // TODO: Bind that to QML
        // if (adapter_descs.size() > 0) {
        //     d->form_to_adapt = form;
        //      dd->adapter_menu = new gnomonFormAdapterMenu(adapter_descs);
        //      dd->adapter_menu->setAttribute(Qt::WA_DeleteOnClose, true);
        //      dd->adapter_menu->resize(dtkApp->window()->width() * 2/5, dtkApp->window()->height() - 40);
        //      dd->adapter_menu->move(dtkApp->window()->frameGeometry().topLeft() + QPoint(86,0));
        //      dd->adapter_menu->show();

        //     QObject *context =  dd->adapter_menu->rootObject();
        //     connect(context, SIGNAL(clicked(const QString&)), d, SLOT(adaptForm(const QString&)));

        //     QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect( dd->adapter_menu);
        // }
    }

    if(!adapter_found) {
        emit badFormDropped(name ,acceptedForms().join(", "));
    }
}


std::shared_ptr<gnomonBinaryImageSeries> gnomonVtkView::binaryImage(void)
{
    if (d->forms.contains("gnomonBinaryImage")) {
        return std::dynamic_pointer_cast<gnomonBinaryImageSeries>(d->forms["gnomonBinaryImage"]);
    } else {
        return nullptr;
    }
}

std::shared_ptr<gnomonCellComplexSeries> gnomonVtkView::cellComplex(void)
{
    if (d->forms.contains("gnomonCellComplex")) {
        return std::dynamic_pointer_cast<gnomonCellComplexSeries>(d->forms["gnomonCellComplex"]);
    } else {
        return nullptr;
    }
}

std::shared_ptr<gnomonCellImageSeries> gnomonVtkView::cellImage(void)
{
    if (d->forms.contains("gnomonCellImage")) {
        return std::dynamic_pointer_cast<gnomonCellImageSeries>(d->forms["gnomonCellImage"]);
    } else {
        return nullptr;
    }
}

std::shared_ptr<gnomonImageSeries> gnomonVtkView::image(void)
{
    if (d->forms.contains("gnomonImage")) {
        return std::dynamic_pointer_cast<gnomonImageSeries>(d->forms["gnomonImage"]);
    } else {
        return nullptr;
    }
}


std::shared_ptr<gnomonLStringSeries> gnomonVtkView::lString(void)
{
    if (d->forms.contains("gnomonLString")) {
        return std::dynamic_pointer_cast<gnomonLStringSeries>(d->forms["gnomonLString"]);
    } else {
        return nullptr;
    }
}

std::shared_ptr<gnomonMeshSeries> gnomonVtkView::mesh(void)
{
    if (d->forms.contains("gnomonMesh")) {
        return std::dynamic_pointer_cast<gnomonMeshSeries>(d->forms["gnomonMesh"]);
    } else {
        return nullptr;
    }
}

std::shared_ptr<gnomonPointCloudSeries> gnomonVtkView::pointCloud(void)
{
    if (d->forms.contains("gnomonPointCloud")) {
        return std::dynamic_pointer_cast<gnomonPointCloudSeries>(d->forms["gnomonPointCloud"]);
    } else {
        return nullptr;
    }
}

QString gnomonVtkView::formVisuName(const QString& name)
{
    QString visu_name;
    if (d->forms.contains(name)) {
        visu_name =  dd->formVisualizationNames[name];
    }
    return visu_name;
}

QVariantList gnomonVtkView::formVisualizations(const QString& name)
{
    if (d->forms.contains(name)) {
        return d->visualizationCommands[name]->pluginFactory()->dataList();
    }
    return {};
}

void gnomonVtkView::setFormVisuName(const QString& name, const QString& visu_name)
{
    if (d->forms.contains(name)  && !visu_name.isEmpty()) {
        if(dd->viewParameters.parameters.contains(visu_name)) {
            // setting the parameters does not work so ignoring it for now
            //dd->setFormVisualization(name, visu_name, d->viewParameters.parameters[visu_name]);
            dd->setFormVisualization(name, visu_name);
        } else {
            dd->setFormVisualization(name, visu_name);
        }
    }
}

QJSValue gnomonVtkView::formVisuParameters(const QString& name)
{
    if (d->forms.contains(name)) {
        QJSValue parameters = dtkCoreParameterCollection( dd->formVisualization[name]->parameters()).toJSValue(this->parent());
        QMap<QString, QString> parameter_groups =  dd->formVisualization[name]->parameterGroups();

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

QVariant gnomonVtkView::formVisuParameter(const QString& name, const QString& parameter_name)
{
    if (d->forms.contains(name)) {
        auto params =  dd->formVisualization[name]->parameters();
        if (params.keys().contains(parameter_name)) {
            dtkCoreParameter *param = params.value(parameter_name);
            // TODO: More specific cases to handle?
            if (auto string_param = dynamic_cast<dtkCoreParameterSimple<QString> *>(param))
            {
                return QVariant(string_param->value());
            } else {
                return param->variant();
            }
        } else {
            return QVariant();
        }
    } else {
        return QVariant();
    }
}

void gnomonVtkView::setFormVisuParameter(const QString& name, const QString& parameter_name, const QVariant& value)
{
    if (d->forms.contains(name)) {
        auto visu =  dd->formVisualization[name];
        visu->setParameter(parameter_name, value);
    }
}

void gnomonVtkView::setFormVisible(const QString& name, bool visible)
{
    if ( dd->formVisualization.contains(name)) {
        if ( dd->formVisualization[name]) {
             dd->formVisualization[name]->setVisible(visible);
             dd->formVisibility[name] = visible;
        }
    }

    this->render();
}

void gnomonVtkView::removeForm(const QString& name)
{
    if ( dd->formVisualization.contains(name)) {
        if ( dd->formVisualization[name]) {
             dd->formVisualization[name]->disconnect();
             dd->formVisualization[name]->clearConnections();
             dd->formVisualization[name]->clear();
        }
    }
     dd->formVisualization.remove(name);
    if ( dd->formVisualizationNames.contains(name)) {
        dd->viewParameters.parameters.remove( dd->formVisualizationNames[name]);
    }
     dd->formVisualizationNames.remove(name);
     dd->formVisibility.remove(name);
    d->forms.remove(name);
    dd->viewParameters.visuSelected.remove(name);

    dd->updateFormsTimes();

    this->render();

    emit formsChanged();
}

void gnomonVtkView::setBounds(double bounds[6])
{
    bool changed = false;

    if (bounds[0] != dd->xBounds[0]) {
        dd->xBounds[0] = bounds[0];
        changed = true;
    }
    if (bounds[1] != dd->xBounds[1]) {
        dd->xBounds[1] = bounds[1];
        changed = true;
    }
    if (bounds[2] != dd->yBounds[0]) {
        dd->yBounds[0] = bounds[2];
        changed = true;
    }
    if (bounds[3] != dd->yBounds[1]) {
        dd->yBounds[1] = bounds[3];
        changed = true;
    }
    if (bounds[4] != dd->zBounds[0]) {
        dd->zBounds[0] = bounds[4];
        changed = true;
    }
    if (bounds[5] != dd->zBounds[1]) {
        dd->zBounds[1] = bounds[5];
        changed = true;
    }

    if (changed) {
        emit boundsChanged();
        dd->renderer2D->ResetCamera();
        dd->renderer3D->ResetCamera();
    }
}

void gnomonVtkView::setBounds(double xMin, double xMax, double yMin, double yMax, double zMin, double zMax)
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

void gnomonVtkView::getBounds(double bounds[6])
{
    bounds[0] = dd->xBounds[0];
    bounds[1] = dd->xBounds[1];
    bounds[2] = dd->yBounds[0];
    bounds[3] = dd->yBounds[1];
    bounds[4] = dd->zBounds[0];
    bounds[5] = dd->zBounds[1];
}

double gnomonVtkView::xMin(void) const
{
    return dd->xBounds[0];
}

double gnomonVtkView::xMax(void) const
{
    return dd->xBounds[1];
}

double gnomonVtkView::yMin(void) const
{
    return dd->yBounds[0];
}

double gnomonVtkView::yMax(void) const
{
    return dd->yBounds[1];
}

double gnomonVtkView::zMin(void) const
{
    return dd->zBounds[0];
}

double gnomonVtkView::zMax(void) const
{
    return dd->zBounds[1];
}

gnomonVtkView::Mode gnomonVtkView::mode(void) const
{
    return dd->mode;
}

void gnomonVtkView::setCamera(vtkCamera *cam)
{
    vtkSmartPointer<vtkCamera> camera3D = dd->renderer3D->GetActiveCamera();
//    camera3D->DeepCopy(cam);
    camera3D->SetFocalPoint(cam->GetFocalPoint());
    camera3D->SetViewUp(cam->GetViewUp());
    camera3D->SetPosition(cam->GetPosition());
}

void gnomonVtkView::setEnableLinking(bool enable)
{
    dd->enableLink = enable;
    // d->refresh();
}

void gnomonVtkView::setEnableMenus(bool enable)
{
    dd->enableMenus = enable;
    // d->refresh();
}

vtkRenderWindowInteractor *gnomonVtkView::interactor(void)
{
    return dd->interactor();
}

vtkRenderer *gnomonVtkView::renderer2D(void)
{
    return dd->renderer2D;
}

vtkRenderer *gnomonVtkView::renderer3D(void)
{
    return dd->renderer3D;
}

gnomonVtkView::Orientation gnomonVtkView::orientation(void)
{
    return dd->ori;
}

void gnomonVtkView::render(void)
{
    dd->renderer2D->ResetCameraClippingRange();
    dd->interactor()->Render();
}

void gnomonVtkView::update(void)
{
    for (const auto& key :  dd->formVisualization.keys()) {
         dd->formVisualization[key]->update();
         dd->formVisualization[key]->setVisible( dd->formVisibility[key]);
    }
}

void gnomonVtkView::clear(void)
{
    for (auto fv : dd->formVisualization) {
        fv->disconnect();
        fv->clearConnections();
        fv->clear();
    }
    dd->formVisualizationNames.clear();
    dd->formVisibility.clear();
    dd->formVisualization.clear();

    dd->updateFormsTimes();
    this->render();

    gnomonAbstractView::clear();
}

void gnomonVtkView::startPicking() {
    if(! dd->formVisualization["gnomonCellImage"] ||
        dd->formVisualization["gnomonCellImage"]->pluginName() != "CellImageVtkVisualizationMarchingCubes") {
        qWarning() << "Picking not implemented for : "
                   <<  dd->formVisualizationNames["gnomonCellImage"]
                   << " only CellImageVtkVisualizationMarchingCubes has picking";
        return;
    }

    // backup old interactor style
    dd->old_style = dd->interactor()->GetInteractorStyle();

    dd->picking_visu = std::dynamic_pointer_cast<gnomonAbstractCellImageVtkVisualization>( dd->formVisualization["gnomonCellImage"]);

    if(dd->mode == gnomonVtkView::VIEW_MODE_3D) {
        dd->picking_visu->on3D();
    } else {
        dd->picking_visu->on2D();
    }
    dd->interactor()->SetInteractorStyle(dd->picking_visu->interactorStyle());
    dd->picking_visu->interactorStyle()->setView(this);
    dd->connectPicked = connect(dd->picking_visu.get(), &gnomonAbstractCellImageVtkVisualization::pickedCells, this, &gnomonVtkView::setPickedCells);
}

void gnomonVtkView::stopPicking() {
    if(!dd->old_style)
        return;

    dd->picking_visu->stopPicking();
    disconnect(dd->connectPicked);

    // set interactorstyle to old style
    dd->interactor()->SetInteractorStyle(dd->old_style);
    dd->old_style = nullptr;
    dd->picking_visu = nullptr;
    this->render();
}

void gnomonVtkView::onSliceChanged(int slice)
{
    // d->slice_slider->setValue(slice);

    Q_UNUSED(slice);
}

void gnomonVtkView::onTimeChanged(double time)
{
    this->setCurrentTime(time);
}

void gnomonVtkView::setInPool(bool inpool)
{
    dd->in_pool = inpool;
}

bool gnomonVtkView::inPool(void)
{
    return dd->in_pool;
}

bool gnomonVtkView::synced(void)
{
    return dd->synced;
}

bool gnomonVtkView::syncing(void)
{
    return dd->syncing;
}

void gnomonVtkView::updateShortcutKeys(void)
{
    // d->updateKeys();
}

void gnomonVtkView::drop(int index)
{
    std::shared_ptr<gnomonAbstractDynamicForm> form = gnomonFormManager::instance()->get(index);

    if (this->empty()) {
        if (vtkCamera *cam = gnomonFormManager::instance()->getCamera(index)) {
            this->setCamera(cam);
        }
    }
    this->setForm("formManager", form, gnomonFormManager::instance()->getVisualization(index));

    dd->interactor()->Render();
    gnomonFormManager::instance()->setFormDropped(form);
}

gnomonDynamicFormMetadata* gnomonVtkView::formMetadata(const QString &name) {
    gnomonDynamicFormMetadata *ptr = d->forms[name]->metadata();
    QQmlEngine::setObjectOwnership(ptr, QQmlEngine::CppOwnership);
    return ptr;
}

void gnomonVtkView::notifyFormSelected(int index, QString formType) {
    dd->viewParameters.currentFormIndex = index;
    dd->viewParameters.currentFormType = std::move(formType);
}

int gnomonVtkView::lastFormIndexSelected() {
    return dd->viewParameters.currentFormIndex;
}

QString gnomonVtkView::lastFromTypeSelected() {
    return dd->viewParameters.currentFormType;
}

QString gnomonVtkView::lastVisuSelected(QString formType) {
    return dd->viewParameters.visuSelected[formType];
}

// ///////////////////////////////////////////////////////////////////

#include "gnomonVtkView.moc"

//
// gnomonVtkView.cpp ends here
