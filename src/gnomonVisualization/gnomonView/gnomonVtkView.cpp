#include "gnomonVtkView.h"
#include "gnomonAbstractView_p.h"

#include <gnomonCore/gnomonAlgorithm/gnomonMesh/gnomonAbstractMeshAdapter>
#include <gnomonCore/gnomonAlgorithm/gnomonCellComplex/gnomonAbstractCellComplexAdapter>
#include <gnomonCore/gnomonCommand/gnomonCellComplex/gnomonCellComplexAdapterCommand>
#include <gnomonCore/gnomonCommand/gnomonMesh/gnomonMeshAdapterCommand>
#include <gnomonCore/gnomonPythonPluginLoader.h>

#include "gnomonManager/gnomonFormManager.h"
#include "gnomonPluginFactory.h"
#include "gnomonView/gnomonAbstractView.h"
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
#include <vtkAxesActor.h>
#include <vtkCallbackCommand.h>
#include <vtkCamera.h>
#include <vtkCaptionActor2D.h>
#include <vtkCubeAxesActor.h>
//#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkInteractorObserver.h>
#include <vtkInteractorStyle.h>
#include <vtkInteractorStyleImage.h>
#include <vtkMath.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkPNGWriter.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRendererCollection.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkTextProperty.h>
#include <vtkWindowToImageFilter.h>
#include <vtkSSAAPass.h>
#include <vtkOpenGLRenderer.h>
#include <vtkRenderStepsPass.h>
#include <vtkBoundingBox.h>

// #include <QVTKInteractor.h>
// #include <QVTKOpenGLNativeWidget.h>

// ///////////////////////////////////////////////////////////////////
// gnomonCameraParameters
// ///////////////////////////////////////////////////////////////////

class GNOMONVISUALIZATION_EXPORT gnomonCameraParameters
{
public:
    gnomonCameraParameters(void) = default;
    gnomonCameraParameters(double distance, double azimuth, double elevation, double roll) : distance(distance), azimuth(azimuth), elevation(elevation), roll(roll) {};
    ~gnomonCameraParameters(void) = default;

public:
    void fromVtkCamera(vtkSmartPointer<vtkCamera> cam);
    void toVtkCamera(vtkSmartPointer<vtkCamera> cam);

    void fromJson(const QJsonObject json);
    const QJsonObject toJson(void);

public:
    double distance = 1;
    double azimuth = 0;
    double elevation = 0;
    double roll = 0;
};

void gnomonCameraParameters::fromVtkCamera(vtkSmartPointer<vtkCamera> cam)
{
    double pos[3], foc[3], vec[3], z_vec[3], t_vec[3], up[3];
    cam->GetPosition(pos);
    cam->GetFocalPoint(foc);
    cam->GetViewUp(up);

    vtkMath::Subtract(pos, foc, vec);
    this->distance = vtkMath::Norm(vec);
    this->azimuth = vtkMath::DegreesFromRadians(atan2(vec[1], vec[0]));
    this->elevation = vtkMath::DegreesFromRadians(asin(vec[2] / distance));

    vtkMath::MultiplyScalar(vec, 1/this->distance);
    double z_axis[3] = {0, 0, 1};
    static const double eps = 256*std::numeric_limits<double>::epsilon();
    if (fabs(1 - fabs(vtkMath::Dot(vec, z_axis))) > eps) {
        vtkMath::Cross(z_axis, vec, t_vec);
        vtkMath::MultiplyScalar(t_vec, 1 / vtkMath::Norm(t_vec));
        vtkMath::Cross(vec, t_vec, z_vec);
        double up_t = vtkMath::Dot(t_vec, up);
        double up_z = vtkMath::Dot(z_vec, up);
        this->roll = vtkMath::DegreesFromRadians(atan2(up_t, up_z));
    } else {
        this->roll = vtkMath::DegreesFromRadians(atan2(-up[0], up[1]));
    }
}

void gnomonCameraParameters::toVtkCamera(vtkSmartPointer<vtkCamera> cam)
{
    double pos[3], foc[3], vec[3], up[3], new_pos[3];
    cam->GetPosition(pos);
    cam->GetFocalPoint(foc);

    double a = vtkMath::RadiansFromDegrees(this->azimuth);
    double e = vtkMath::RadiansFromDegrees(this->elevation);
    double r = vtkMath::RadiansFromDegrees(this->roll);

    vec[0] = cos(a)*cos(e);
    vec[1] = sin(a)*cos(e);
    vec[2] = sin(e);

    static const double eps = 256*std::numeric_limits<double>::epsilon();
    if (fabs(cos(e)) > eps) {
        double z_vec[3] = {-cos(a)*sin(e), -sin(a)*sin(e), cos(e)};
        double t_vec[3];
        vtkMath::Cross(vec, z_vec, t_vec);
        up[0] = -sin(r)*t_vec[0] + cos(r)*z_vec[0];
        up[1] = -sin(r)*t_vec[1] + cos(r)*z_vec[1];
        up[2] = -sin(r)*t_vec[2] + cos(r)*z_vec[2];
    } else {
        up[0] = -sin(r);
        up[1] = cos(r);
        up[2] = 0;
    }

    vtkMath::MultiplyScalar(vec, this->distance);
    vtkMath::Add(foc, vec, new_pos);
    cam->SetPosition(new_pos);
    cam->SetViewUp(up);
}

void gnomonCameraParameters::fromJson(const QJsonObject json)
{
    QVariantMap camera_map = json.toVariantMap();

    this->elevation = camera_map.value("elevation", 90).toDouble();
    this->azimuth = camera_map.value("azimuth", 0).toDouble();
    this->roll = camera_map.value("roll", 0).toDouble();
    this->distance = camera_map.value("distance", 100).toDouble();
}

const QJsonObject gnomonCameraParameters::toJson(void)
{
    QVariantMap camera_map;
    camera_map["elevation"] = this->elevation;
    camera_map["azimuth"] = this->azimuth;
    camera_map["roll"] = this->roll;
    camera_map["distance"] = this->distance;

    return QJsonObject::fromVariantMap(camera_map);
}

// ///////////////////////////////////////////////////////////////////
// gnomonVtkViewPrivate
// ///////////////////////////////////////////////////////////////////

class gnomonVtkViewPrivate : public QObject // QVTKOpenGLNativeWidget
{
    Q_OBJECT

public:
     gnomonVtkViewPrivate(QObject *parent = Q_NULLPTR);
    ~gnomonVtkViewPrivate(void);

public:
    vtkRenderWindowInteractor *interactor(void)
    {
        if (this->window) {
            return this->window->GetInteractor();
        } else {
            return nullptr;
        }
    }

public:
    void setViewMode(gnomonVtkView::Mode mode);
    void setSliceOrientation(gnomonVtkView::Orientation orientation);
    void updateOrientation(void);

public:
    void updateGrid(void);
    void updateAxes(void);

public:
    void addCameraObserver(vtkSmartPointer<vtkCamera> cam);

public:
    vtkSmartPointer<vtkRenderWindow> window;
    vtkSmartPointer<vtkRenderer> renderer2D;
    vtkSmartPointer<vtkRenderer> renderer3D;

public:
    gnomonVtkView *q = nullptr;

public:
    gnomonVtkView::Mode mode = gnomonVtkView::VIEW_MODE_3D;
    gnomonVtkView::Orientation ori = gnomonVtkView::NONE;
    gnomonVtkView::Representation representation = gnomonVtkView::VTK_REPRESENTATION_SURFACE;
    gnomonVtkView::Grid grid_type = gnomonVtkView::GRID_PLANES;
    gnomonVtkView::Orientation grid_orientation = gnomonVtkView::SLICE_ORIENTATION_XZ;
    QMap<gnomonVtkView::Orientation, vtkSmartPointer<vtkCamera> > cameras;

public:
    QMetaObject::Connection connect3D;
    QMetaObject::Connection connect2D;
    QMetaObject::Connection connectXY;
    QMetaObject::Connection connectXZ;
    QMetaObject::Connection connectYZ;
    QMetaObject::Connection connectSlice;
    QMetaObject::Connection connectTime;

    QMetaObject::Connection connectRenderWindowAdded;

public:
    void clearConnections(void);

public:
    vtkSmartPointer<vtkInteractorObserver> old_style = nullptr;
    std::shared_ptr<gnomonAbstractCellImageVtkVisualization> picking_visu = nullptr;
    QMetaObject::Connection connectPicked;

public:
    vtkSmartPointer<vtkCubeAxesActor> grid_actor = nullptr;
    vtkSmartPointer<vtkAxesActor> axes =  nullptr;
    vtkSmartPointer<vtkOrientationMarkerWidget> axes_widget = nullptr;

public:
    int syncing_count = 0; QTimer *syncing_timer = nullptr; bool synced = false; bool syncing = false;

public:
    bool enableLink = false;
    bool enableMenus = true;

public:
    bool in_pool = false;

public:
    QList<long> picked_cells;

public:
    double xBounds[2] = {0,0}, yBounds[2] = {0,0}, zBounds[2] = {0,0};
    double c_x = 0., c_y = 0., c_z = 0.;

public:
    QSettings *settings;
    QColor background_color;
    bool grid_visible;
    bool axes_visible;
    bool camera_fixed;

signals:
    void sliceOrientationChanged(int);

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
    this->renderer2D = vtkSmartPointer<vtkRenderer>::New();
    this->renderer3D = vtkSmartPointer<vtkRenderer>::New();
    this->renderer2D->UseFXAAOn(); // anti-aliasing
    this->renderer3D->UseFXAAOn(); // anti-aliasing
    // adding SSAA pass

    vtkOpenGLRenderer* glrenderer = vtkOpenGLRenderer::SafeDownCast(this->renderer3D);
    // get the basic VTK render steps
    vtkNew<vtkRenderStepsPass> basicPasses;

    // finally blur the resulting image
    // The blur delegates rendering the unblured image
    // to the basicPasses
    vtkNew<vtkSSAAPass> ssaa;
    ssaa->SetDelegatePass(basicPasses);

    // tell the renderer to use our render pass pipeline

    // deactivated for now as it's not working well
    //glrenderer->SetPass(ssaa);


    static int count = 0;
}

gnomonVtkViewPrivate::~gnomonVtkViewPrivate(void)
{
    this->clearConnections();

    for(auto map : adapterCommands) {
        for(auto command: map) {
            delete command;
        }
        map.clear();
    }
}

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


void gnomonVtkViewPrivate::updateGrid(void)
{
    if (this->grid_actor) {
        bool x_visible = this->mode==gnomonVtkView::VIEW_MODE_3D || this->ori!=gnomonVtkView::SLICE_ORIENTATION_YZ;
        bool y_visible = this->mode==gnomonVtkView::VIEW_MODE_3D || this->ori!=gnomonVtkView::SLICE_ORIENTATION_XZ;
        bool z_visible = this->mode==gnomonVtkView::VIEW_MODE_3D || this->ori!=gnomonVtkView::SLICE_ORIENTATION_XY;

        this->grid_actor->SetXAxisVisibility(x_visible);
        this->grid_actor->SetXAxisLabelVisibility(x_visible);
        this->grid_actor->SetXAxisTickVisibility(x_visible);
        this->grid_actor->SetXAxisMinorTickVisibility(false);
        this->grid_actor->SetDrawXGridlines(x_visible);

        this->grid_actor->SetYAxisVisibility(y_visible);
        this->grid_actor->SetYAxisLabelVisibility(y_visible);
        this->grid_actor->SetYAxisTickVisibility(y_visible);
        this->grid_actor->SetYAxisMinorTickVisibility(false);
        this->grid_actor->SetDrawYGridlines(y_visible);

        this->grid_actor->SetZAxisVisibility(z_visible);
        this->grid_actor->SetZAxisLabelVisibility(z_visible);
        this->grid_actor->SetZAxisTickVisibility(z_visible);
        this->grid_actor->SetZAxisMinorTickVisibility(false);
        this->grid_actor->SetDrawZGridlines(z_visible);

        QColor axis_color = QColor(255-this->background_color.red(), 255-this->background_color.green(), 255-this->background_color.blue());
        for (int i_dim=0; i_dim<3; i_dim++) {
            this->grid_actor->GetTitleTextProperty(i_dim)->SetColor(axis_color.redF(), axis_color.greenF(), axis_color.blueF());
            this->grid_actor->GetLabelTextProperty(i_dim)->SetColor(axis_color.redF(), axis_color.greenF(), axis_color.blueF());
        }

        if (!isnan(this->xBounds[0])) {
            double dx = abs(this->xBounds[1] - this->xBounds[0]);
            double dy = abs(this->yBounds[1] - this->yBounds[0]);
            double dz = abs(this->zBounds[1] - this->zBounds[0]);
            double l = std::max(std::max(dx, dy), dz);
            double golden_ratio = 1.61803398875;
            double ml = l / golden_ratio;

            double x_b[2] = {this->xBounds[0], this->xBounds[1]};
            if (dx < ml) {
                x_b[0] += dx / 2 - ml / 2;
                x_b[1] -= dx / 2 - ml / 2;
            }
            double y_b[2] = {this->yBounds[0], this->yBounds[1]};
            if (dy < ml) {
                y_b[0] += dy / 2 - ml / 2;
                y_b[1] -= dy / 2 - ml / 2;
            }
            double z_b[2] = {this->zBounds[0], this->zBounds[1]};
            if (dz < ml) {
                z_b[0] += dz / 2 - ml / 2;
                z_b[1] -= dz / 2 - ml / 2;
            }

            if (this->grid_type == gnomonVtkView::GRID_CUBE) {
                this->grid_actor->SetBounds(x_b[0], x_b[1], y_b[0], y_b[1], z_b[0], z_b[1]);
            } else if (this->grid_type == gnomonVtkView::GRID_PLANES) {
                if (this->grid_orientation == gnomonVtkView::SLICE_ORIENTATION_XY) {
                    this->grid_actor->SetBounds(x_b[0], x_b[1], y_b[0], y_b[1], 0, 0);
                } else if (this->grid_orientation == gnomonVtkView::SLICE_ORIENTATION_XZ) {
                    this->grid_actor->SetBounds(x_b[0], x_b[1], 0, 0, z_b[0], z_b[1]);
                } else if (this->grid_orientation == gnomonVtkView::SLICE_ORIENTATION_YZ) {
                    this->grid_actor->SetBounds(0, 0, y_b[0], y_b[1], z_b[0], z_b[1]);
                }
            }
        }
    }
}

void gnomonVtkViewPrivate::updateAxes(void)
{
    if (this->axes_widget) {
        if (this->renderer3D) {
            this->axes_widget->SetCurrentRenderer(this->renderer3D);
        }

        bool show_3d = (this->mode == gnomonVtkView::VIEW_MODE_3D) && this->axes_visible;
        if (this->interactor()) {
            this->axes_widget->SetInteractor(this->interactor());

            if (show_3d) {
                this->axes_widget->SetEnabled(true);
                this->axes_widget->SetInteractive(true);
            } else {
                if (this->axes_widget->GetEnabled()) {
                    this->axes_widget->SetInteractive(false);  // must be enabled while setting interactive
                }
                this->axes_widget->SetEnabled(false);
            }
            q->render();
        }
    }
}

void gnomonVtkViewPrivate::addCameraObserver(vtkSmartPointer<vtkCamera> cam)
{
    vtkNew<vtkCallbackCommand> camera_callback;
    auto callback = [] (vtkObject* caller, long unsigned int eventId, void* clientData, void* callData) {
        auto view = static_cast<gnomonVtkView *>(clientData);
        emit view->cameraChanged();
    };
    camera_callback->SetCallback(callback);
    camera_callback->SetClientData(static_cast<void *>(this->q));
    cam->AddObserver(vtkCommand::ModifiedEvent, camera_callback);
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
    setObjectName("gnomonVtkView");
    dd = new gnomonVtkViewPrivate;
    dd->q = this;

    dd->addCameraObserver(dd->renderer3D->GetActiveCamera());

    dd->settings = new QSettings(QSettings::IniFormat, QSettings::UserScope, "inria", "gnomon");
    QString bg_color_hex = dd->settings->value("vtk/background_color", "#000000").toString();
    this->setBgColor(QColor(bg_color_hex));
    bool show_grid = dd->settings->value("vtk/grid", false).toBool();
    this->setGridVisible(show_grid);
    // TODO: Unable to display axes widget at creation : hidden by default
    // Interactor is wrong, even after associate, until the view is rendered
    //bool show_axes = dd->settings->value("vtk/axes", false).toBool();
    bool show_axes = false;
    this->setAxesVisible(show_axes);
    bool fixed_camera = dd->settings->value("vtk/fixed_camera", false).toBool();
    this->setCameraFixed(fixed_camera);

    d->acceptForms["gnomonBinaryImage"] = false;
    d->acceptForms["gnomonCellComplex"] = false;
    d->acceptForms["gnomonCellImage"] = false;
    d->acceptForms["gnomonImage"] = false;
    d->acceptForms["gnomonLString"] = false;
    d->acceptForms["gnomonMesh"] = false;
    d->acceptForms["gnomonPointCloud"] = false;

    connect(this, &gnomonVtkView::exportedForm, [=] (std::shared_ptr<gnomonAbstractDynamicForm> form) {
        int index = gnomonFormManager::instance()->formIndex(form->uuid());
        gnomonFormManager::instance()->setCamera(index, dd->renderer3D->GetActiveCamera());
    });
    connect(this, &gnomonVtkView::formsChanged, this, &gnomonVtkView::updateBounds);
}

gnomonVtkView::~gnomonVtkView(void)
{
    d->visualizationCommands.clear();
    delete dd;
}

void gnomonVtkView::associate(vtkRenderWindow *window)
{
    dd->window = window;
    dd->window->AddRenderer(dd->renderer2D);
    dd->window->AddRenderer(dd->renderer3D);

    this->switchTo3D();

    dd->updateOrientation();
    d->updateFormsTimes();
    dd->updateAxes();

    emit renderWindowAssociated();
    update();
}

void gnomonVtkView::switchTo3D(void)
{
    bool hasChanged = dd->mode != gnomonVtkView::VIEW_MODE_3D;
    dd->setViewMode(gnomonVtkView::VIEW_MODE_3D);

    if (hasChanged){
        if (dd->grid_actor) {
            dd->renderer2D->RemoveActor(dd->grid_actor);
            dd->renderer3D->AddActor(dd->grid_actor);
            dd->grid_actor->SetCamera(dd->renderer3D->GetActiveCamera());
        }
        dd->updateGrid();
        dd->updateAxes();
        emit switchedTo3D();
        emit modeChanged();
    }
}

void gnomonVtkView::switchTo2D(void)
{
    bool hasChanged = dd->mode != gnomonVtkView::VIEW_MODE_2D;
    dd->setViewMode(gnomonVtkView::VIEW_MODE_2D);

    if (hasChanged) {
        if (dd->grid_actor) {
            dd->renderer3D->RemoveActor(dd->grid_actor);
            dd->renderer2D->AddActor(dd->grid_actor);
            dd->grid_actor->SetCamera(dd->renderer2D->GetActiveCamera());
        }
        dd->updateGrid();
        dd->updateAxes();
        switch(dd->ori) {
            case gnomonVtkView::NONE:
                this->switchTo2DXY();
                break;
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
    bool hasChanged = dd->ori != gnomonVtkView::SLICE_ORIENTATION_XY;
    if (hasChanged) {
        dd->setSliceOrientation(gnomonVtkView::SLICE_ORIENTATION_XY);
        emit sliceOrientationChanged(gnomonVtkView::SLICE_ORIENTATION_XY);
        emit sliceChanged(dd->c_z);

        dd->updateGrid();
        emit switchedTo2DXY();
        emit orientationChanged();
    }

}

void gnomonVtkView::switchTo2DXZ(void)
{
    bool hasChanged = dd->ori != gnomonVtkView::SLICE_ORIENTATION_XZ;
    if(hasChanged) {
        dd->setSliceOrientation(gnomonVtkView::SLICE_ORIENTATION_XZ);
        emit sliceOrientationChanged(gnomonVtkView::SLICE_ORIENTATION_XZ);
        emit sliceChanged(dd->c_y);

        dd->updateGrid();
        emit switchedTo2DXZ();
        emit orientationChanged();
    }
}

void gnomonVtkView::switchTo2DYZ(void)
{
    bool hasChanged = dd->ori != gnomonVtkView::SLICE_ORIENTATION_YZ;
    if (hasChanged) {
        dd->setSliceOrientation(gnomonVtkView::SLICE_ORIENTATION_YZ);
        emit sliceOrientationChanged(gnomonVtkView::SLICE_ORIENTATION_YZ);
        emit sliceChanged(dd->c_x);

        dd->updateGrid();
        emit switchedTo2DYZ();
        emit orientationChanged();
    }
}

void gnomonVtkView::sliceChange(double value)
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

void gnomonVtkView::setPickedCells(QList<long> new_list)
{
    dd->picked_cells = new_list;
    emit pickedCellsChanged();
}

QList<long> gnomonVtkView::pickedCells(void)
{
    return dd->picked_cells;
}

void gnomonVtkView::setAcceptForm(const QString& form_type, bool accept)
{
    if(!d->acceptForms.contains(form_type)) { // Form not supported by VtkView
        return;
    }
    d->acceptForms[form_type] = accept;

    if(!accept) {
        return;
    }

    if(form_type == "gnomonBinaryImage") {
        d->visualizationCommands["gnomonBinaryImage"] = std::make_shared<gnomonBinaryImageVtkVisualizationCommand>();
    } else if(form_type == "gnomonCellComplex") {
        d->visualizationCommands["gnomonCellComplex"] = std::make_shared<gnomonCellComplexVtkVisualizationCommand>();
    } else if(form_type == "gnomonCellImage") {
        d->visualizationCommands["gnomonCellImage"] = std::make_shared<gnomonCellImageVtkVisualizationCommand>();
    } else if(form_type == "gnomonImage") {
        d->visualizationCommands["gnomonImage"] = std::make_shared<gnomonImageVtkVisualizationCommand>();
    } else if(form_type == "gnomonLString") {
        d->visualizationCommands["gnomonLString"] = std::make_shared<gnomonLStringVtkVisualizationCommand>();
    } else if(form_type == "gnomonMesh") {
        d->visualizationCommands["gnomonMesh"] = std::make_shared<gnomonMeshVtkVisualizationCommand>();
    } else if(form_type == "gnomonPointCloud") {
        d->visualizationCommands["gnomonPointCloud"] = std::make_shared<gnomonPointCloudVtkVisualizationCommand>();
    }

    d->visualizationCommands[form_type]->setView(this);
    connect(d->visualizationCommands[form_type].get(), &gnomonAbstractVisualizationCommand::visuParametersChanged, [=] () {
        emit formVisuParametersChanged();
    });

    if (form_type=="gnomonMesh") {
        loadPluginGroup("meshAdapter");
        for (const auto& key : gnomonCore::meshAdapter::pluginFactory().keys()) {
            gnomonAbstractMeshAdapter *adapter = dynamic_cast<gnomonAbstractMeshAdapter *>(gnomonCore::meshAdapter::pluginFactory().create(key));
            if (! dd->adapterCommands.contains(form_type)) {
                QMap<QString, QString> empty_target;
                dd->adapterTargets[form_type] = empty_target;
                QMap<QString, QString> empty_desc;
                dd->adapterDescriptions[form_type] = empty_desc;
                QMap<QString, gnomonAbstractAdapterCommand *> empty_list;
                dd->adapterCommands[form_type] = empty_list;
            }
            dd->adapterTargets[form_type][key] = adapter->target();
            dd->adapterDescriptions[form_type][key] = adapter->documentation().split("\n")[1];
            dd->adapterCommands[form_type][key] = new gnomonMeshAdapterCommand;
            dd->adapterCommands[form_type][key]->setAlgorithmName(key);
            delete adapter;
        }
    } else if (form_type=="gnomonCellComplex") {
        loadPluginGroup("cellComplexAdapter");
        for (const auto& key : gnomonCore::cellComplexAdapter::pluginFactory().keys()) {
            gnomonAbstractCellComplexAdapter *adapter = dynamic_cast<gnomonAbstractCellComplexAdapter *>(gnomonCore::cellComplexAdapter::pluginFactory().create(key));
            if (! dd->adapterCommands.contains(form_type)) {
                QMap<QString, QString> empty_target;
                dd->adapterTargets[form_type] = empty_target;
                QMap<QString, QString> empty_desc;
                dd->adapterDescriptions[form_type] = empty_desc;
                QMap<QString, gnomonAbstractAdapterCommand *> empty_list;
                dd->adapterCommands[form_type] = empty_list;
            }
            dd->adapterTargets[form_type][key] = adapter->target();
            dd->adapterDescriptions[form_type][key] = adapter->documentation().split("\n")[1];
            dd->adapterCommands[form_type][key] = new gnomonCellComplexAdapterCommand;
            dd->adapterCommands[form_type][key]->setAlgorithmName(key);
            delete adapter;
        }
    }
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
    dd->addCameraObserver(dd->renderer3D->GetActiveCamera());

    other->dd->window->AddObserver(vtkCommand::RenderEvent, this, &gnomonVtkView::render);
    this->render();

    dd->clearConnections();
    dd->connect3D = connect(other, &gnomonVtkView::switchedTo3D, [=] () {
        this->switchTo3D();
        dd->renderer3D->SetActiveCamera(other->dd->renderer3D->GetActiveCamera());
        dd->addCameraObserver(dd->renderer3D->GetActiveCamera());
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
        this->setCurrentTime(value);
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
    dd->addCameraObserver(camera3D);

    dd->clearConnections();

    emit syncedChanged();
}

void gnomonVtkView::disconnectTime() {
    disconnect(dd->connectTime);
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

void gnomonVtkView::removeForm(const QString& form_type)
{
    gnomonAbstractView::removeForm(form_type);
    d->updateFormsTimes();
    if (this->empty()) {
        this->setBounds(0, 0, 0, 0, 0, 0);
    }
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
        dd->updateGrid();
        if (!dd->camera_fixed) {
            dd->renderer2D->ResetCamera();
            dd->renderer3D->ResetCamera();
            emit cameraChanged();
        }
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

void gnomonVtkView::updateBounds(void)
{
    // TODO: delete setBounds from visu plugins at a later date
    vtkBoundingBox boundingBox;
    auto propsList = dd->renderer3D->GetViewProps();
    propsList->InitTraversal();
    if(propsList->GetNumberOfItems()>0) {
        for(vtkIdType a = 0; a < propsList->GetNumberOfItems(); ++a) {
            vtkProp *prop = propsList->GetNextProp();
            double *bounds = prop->GetBounds();
            if(bounds) {
                boundingBox.AddBounds(bounds);
            }
        }
        double bounds[6];
        boundingBox.GetBounds(bounds);
        this->setBounds(bounds);
    } else {
        this->setBounds(0, 0, 0, 0, 0, 0);
    }

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

void gnomonVtkView::setCameraXY(bool flip, bool turn) {
    vtkSmartPointer<vtkCamera> cam = dd->renderer3D->GetActiveCamera();
    cam->SetFocalPoint((dd->xBounds[0] + dd->xBounds[1]) / 2,
                       (dd->yBounds[0] + dd->yBounds[1]) / 2,
                       (dd->zBounds[0] + dd->zBounds[1]) / 2);
    cam->SetPosition((dd->xBounds[0] + dd->xBounds[1]) / 2, (dd->yBounds[0] + dd->yBounds[1]) / 2, flip? dd->zBounds[0] : dd->zBounds[1]);
    cam->SetViewUp(0, turn? -1 : 1, 0);
    dd->renderer3D->ResetCamera();
    emit cameraChanged();
    this->render();
}

void gnomonVtkView::setCameraXZ(bool flip, bool turn)
{
    vtkSmartPointer<vtkCamera> cam = dd->renderer3D->GetActiveCamera();
    cam->SetFocalPoint((dd->xBounds[0] + dd->xBounds[1]) / 2,
                       (dd->yBounds[0] + dd->yBounds[1]) / 2,
                       (dd->zBounds[0] + dd->zBounds[1]) / 2);
    cam->SetPosition((dd->xBounds[0] + dd->xBounds[1]) / 2, flip? dd->yBounds[0] : dd->yBounds[1], (dd->zBounds[0] + dd->zBounds[1]) / 2);
    cam->SetViewUp(0, 0, turn? -1 : 1);
    dd->renderer3D->ResetCamera();
    emit cameraChanged();
    this->render();
}

void gnomonVtkView::setCameraYZ(bool flip, bool turn)
{
    vtkSmartPointer<vtkCamera> cam = dd->renderer3D->GetActiveCamera();
    cam->SetFocalPoint((dd->xBounds[0] + dd->xBounds[1]) / 2,
                       (dd->yBounds[0] + dd->yBounds[1]) / 2,
                       (dd->zBounds[0] + dd->zBounds[1]) / 2);
    cam->SetPosition(flip? dd->xBounds[0] : dd->xBounds[1], (dd->yBounds[0] + dd->yBounds[1]) / 2, (dd->zBounds[0] + dd->zBounds[1]) / 2);
    cam->SetViewUp(0, 0, turn? -1 : 1);
    dd->renderer3D->ResetCamera();
    emit cameraChanged();
    this->render();
}

void gnomonVtkView::setCameraAzimuth(double angle)
{
    vtkSmartPointer<vtkCamera> cam = dd->renderer3D->GetActiveCamera();
    gnomonCameraParameters p;
    p.fromVtkCamera(cam);
    p.azimuth = angle;
    p.toVtkCamera(cam);
    this->render();
}

double gnomonVtkView::cameraAzimuth(void)
{
    vtkSmartPointer<vtkCamera> cam = dd->renderer3D->GetActiveCamera();
    gnomonCameraParameters p;
    p.fromVtkCamera(cam);
    return p.azimuth;
}

void gnomonVtkView::setCameraElevation(double angle)
{
    vtkSmartPointer<vtkCamera> cam = dd->renderer3D->GetActiveCamera();
    gnomonCameraParameters p;
    p.fromVtkCamera(cam);
    p.elevation = angle;
    p.toVtkCamera(cam);
    this->render();
}

double gnomonVtkView::cameraElevation(void)
{
    vtkSmartPointer<vtkCamera> cam = dd->renderer3D->GetActiveCamera();
    gnomonCameraParameters p;
    p.fromVtkCamera(cam);
    return p.elevation;
}

void gnomonVtkView::setCameraRoll(double angle)
{
    vtkSmartPointer<vtkCamera> cam = dd->renderer3D->GetActiveCamera();
    gnomonCameraParameters p;
    p.fromVtkCamera(cam);
    p.roll = angle;
    p.toVtkCamera(cam);
    this->render();
}

double gnomonVtkView::cameraRoll(void)
{
    vtkSmartPointer<vtkCamera> cam = dd->renderer3D->GetActiveCamera();
    gnomonCameraParameters p;
    p.fromVtkCamera(cam);
    return p.roll;
}

void gnomonVtkView::setCameraDistance(double distance)
{
    if (distance > 0) {
        vtkSmartPointer<vtkCamera> cam = dd->renderer3D->GetActiveCamera();
        gnomonCameraParameters p;
        p.fromVtkCamera(cam);
        p.distance = distance;
        p.toVtkCamera(cam);
        this->render();
        emit cameraChanged();
    }
}

double gnomonVtkView::cameraDistance(void)
{
    vtkSmartPointer<vtkCamera> cam = dd->renderer3D->GetActiveCamera();
    gnomonCameraParameters p;
    p.fromVtkCamera(cam);
    return p.distance;
}

void gnomonVtkView::setCamera(vtkCamera *cam)
{
    vtkSmartPointer<vtkCamera> camera3D = dd->renderer3D->GetActiveCamera();
//    camera3D->DeepCopy(cam);
    camera3D->SetFocalPoint(cam->GetFocalPoint());
    camera3D->SetViewUp(cam->GetViewUp());
    camera3D->SetPosition(cam->GetPosition());
    emit cameraChanged();
}

void gnomonVtkView::resetCamera()
{
    if (!dd->camera_fixed) {
        dd->renderer3D->ResetCamera();
        dd->renderer2D->ResetCamera();
        emit cameraChanged();
    }
}

void gnomonVtkView::saveCamera(const QString& file_url)
{
    vtkSmartPointer<vtkCamera> cam = dd->renderer3D->GetActiveCamera();
    gnomonCameraParameters p;
    p.fromVtkCamera(cam);
    QJsonObject camera_json = p.toJson();

    QJsonDocument document;
    document.setObject(camera_json);
    QByteArray bytes = document.toJson(QJsonDocument::Indented);

    QString file_path;
    const QUrl url(file_url);
    if (url.isLocalFile()) {
        file_path = QDir::toNativeSeparators(url.toLocalFile());
    } else {
        file_path = file_url;
    }

    QFile f(file_path);
    if(f.open(QIODevice::WriteOnly| QIODevice::Text)) {
        QTextStream out(&f);
        out<<bytes;
        f.close();
    } else {
        qWarning()<<"Could not save to file"<<file_path;
    }
}

void gnomonVtkView::loadCamera(const QString& file_url)
{
    QJsonObject camera_json;

    QString file_path;
    const QUrl url(file_url);
    if (url.isLocalFile()) {
        file_path = QDir::toNativeSeparators(url.toLocalFile());
    } else {
        file_path = file_url;
    }

    QFile f(file_path);
    if (f.open(QIODevice::ReadOnly)) {
        QByteArray bytes = f.readAll();
        f.close();

        QJsonParseError jsonError;
        QJsonDocument document = QJsonDocument::fromJson(bytes, &jsonError);
        if (jsonError.error != QJsonParseError::NoError) {
            qWarning()<<"Error reading file"<<file_path<<": "<<jsonError.errorString();
            return;
        }
        if (document.isObject()) {
            camera_json = document.object();
        }
    } else {
        qWarning()<<"Could not read file"<<file_path;
    }

    vtkSmartPointer<vtkCamera> cam = dd->renderer3D->GetActiveCamera();
    auto p = gnomonCameraParameters();
    p.fromJson(camera_json);
    p.toVtkCamera(cam);
    this->render();
}

void gnomonVtkView::setBgColor(const QColor& color)
{
    if (color != dd->background_color) {
        dd->background_color = color;
        renderer3D()->SetBackground(dd->background_color.redF(), dd->background_color.greenF(), dd->background_color.blueF());
        renderer2D()->SetBackground(dd->background_color.redF(), dd->background_color.greenF(), dd->background_color.blueF());

        if (dd->grid_actor) {
            QColor axis_color = QColor(255-dd->background_color.red(), 255-dd->background_color.green(), 255-dd->background_color.blue());
            for (int i_dim=0; i_dim<3; i_dim++) {
                dd->grid_actor->GetTitleTextProperty(i_dim)->SetColor(axis_color.redF(), axis_color.greenF(), axis_color.blueF());
                dd->grid_actor->GetLabelTextProperty(i_dim)->SetColor(axis_color.redF(), axis_color.greenF(), axis_color.blueF());
            }
        }

        this->render();
        emit bgColorChanged();

        dd->settings->setValue("vtk/background_color", dd->background_color.name());
    }
}

const QColor& gnomonVtkView::bgColor(void)
{
    return dd->background_color;
}

void gnomonVtkView::setGridVisible(bool visible)
{
    bool new_grid = false;
    if (!dd->grid_actor)  {
        new_grid = true;
        dd->grid_actor = vtkSmartPointer<vtkCubeAxesActor>::New();
        if (dd->mode == gnomonVtkView::VIEW_MODE_3D) {
            dd->renderer3D->AddActor(dd->grid_actor);
            dd->grid_actor->SetCamera(dd->renderer3D->GetActiveCamera());
        } else {
            dd->renderer2D->AddActor(dd->grid_actor);
            dd->grid_actor->SetCamera(dd->renderer2D->GetActiveCamera());
        }

        dd->grid_actor->SetUseTextActor3D(false);
        dd->grid_actor->SetUse2DMode(true);
        for (int i_dim=0; i_dim<3; i_dim++) {
            dd->grid_actor->GetLabelTextProperty(i_dim)->SetFontSize(8);
        }
        dd->grid_actor->SetXTitle("");
        dd->grid_actor->SetYTitle("");
        dd->grid_actor->SetZTitle("");

        dd->grid_actor->SetGridLineLocation(dd->grid_actor->VTK_GRID_LINES_FURTHEST);

        dd->grid_actor->SetFlyModeToStaticTriad();
        dd->grid_actor->SetInertia(2);
        dd->grid_actor->SetEnableDistanceLOD(true);
        dd->grid_actor->SetDistanceLODThreshold(1);
    }

    dd->updateGrid();

    if (visible != dd->grid_visible || new_grid) {
        dd->grid_visible = visible;
        dd->grid_actor->SetVisibility(dd->grid_visible);

        this->render();
        emit gridVisibleChanged();

        dd->settings->setValue("vtk/grid", dd->grid_visible);
    }
}

bool gnomonVtkView::gridVisible(void)
{
    return dd->grid_visible;
}

void gnomonVtkView::setGridType(Grid type)
{
    if (type != dd->grid_type) {
        dd->grid_type = type;
        dd->updateGrid();
        this->render();
        emit gridTypeChanged();
    }
}

gnomonVtkView::Grid gnomonVtkView::gridType(void)
{
    return dd->grid_type;
}

void gnomonVtkView::setGridOrientation(Orientation orientation)
{
    if (orientation != dd->grid_orientation) {
        dd->grid_orientation = orientation;
        dd->updateGrid();
        this->render();
        emit gridOrientationChanged();
    }
}

gnomonVtkView::Orientation gnomonVtkView::gridOrientation(void)
{
    return dd->grid_orientation;
}

void gnomonVtkView::setAxesVisible(bool visible)
{
    bool new_axes = false;
    if (!dd->axes_widget)  {
        new_axes = true;
        dd->axes =  vtkSmartPointer<vtkAxesActor>::New();
        dd->axes->GetXAxisCaptionActor2D()->GetCaptionTextProperty()->ItalicOff();
        dd->axes->GetYAxisCaptionActor2D()->GetCaptionTextProperty()->ItalicOff();
        dd->axes->GetZAxisCaptionActor2D()->GetCaptionTextProperty()->ItalicOff();

        dd->axes_widget = vtkSmartPointer<vtkOrientationMarkerWidget>::New();
        dd->axes_widget->SetOrientationMarker(dd->axes);
        dd->axes_widget->SetViewport(-0.2, -0.2, 0.2, 0.2);
    }

    if (visible != dd->axes_visible || new_axes) {
        dd->axes_visible = visible;

        dd->updateAxes();
        emit axesVisibleChanged();

        dd->settings->setValue("vtk/axes", dd->axes_visible);
    }
}

bool gnomonVtkView::axesVisible(void)
{
    return dd->axes_visible;
}

void gnomonVtkView::setCameraFixed(bool fixed)
{
    if (fixed != dd->camera_fixed) {
        dd->camera_fixed = fixed;
        emit cameraFixedChanged();

        dd->settings->setValue("vtk/fixed_camera", dd->camera_fixed);
    }
}

bool gnomonVtkView::cameraFixed(void)
{
    return dd->camera_fixed;
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

gnomonVtkView::Representation gnomonVtkView::representation(void)
{
    return dd->representation;
}

void gnomonVtkView::setRepresentation(gnomonVtkView::Representation representation)
{
    if (representation != dd->representation) {
        dd->representation = representation;

        auto actors = dd->renderer3D->GetActors();
        actors->InitTraversal();
        for (int i_p=0; i_p<actors->GetNumberOfItems(); i_p++) {
            auto actor = actors->GetNextActor();
            actor->GetProperty()->SetRepresentation(dd->representation);
        }

        auto actors_2d = dd->renderer2D->GetActors();
        actors_2d->InitTraversal();
        for (int i_p=0; i_p<actors_2d->GetNumberOfItems(); i_p++) {
            auto actor_2d = actors_2d->GetNextActor();
            actor_2d->GetProperty()->SetRepresentation(dd->representation);
        }

        this->render();
        emit representationChanged();
    }
}

void gnomonVtkView::render(void)
{
    dd->renderer2D->ResetCameraClippingRange();
    if (dd->interactor()) {
        dd->interactor()->Render();
    }
}

void gnomonVtkView::clear(void)
{
    gnomonAbstractView::clear();
    this->setBounds(0, 0, 0, 0, 0, 0);
    d->updateFormsTimes();
    this->render();
}

void gnomonVtkView::saveScreenshot(const QString& filename)
{
    QString file_path;
    const QUrl url(filename);
    if (url.isLocalFile()) {
        file_path = QDir::toNativeSeparators(url.toLocalFile());
    } else {
        file_path = filename;
    }

    //QImage image = this->toImage();
    //image.save(file_path);

    vtkSmartPointer<vtkWindowToImageFilter> windowToImageFilter = vtkSmartPointer<vtkWindowToImageFilter>::New();
    windowToImageFilter->SetInput(dd->window);
    windowToImageFilter->SetInputBufferTypeToRGBA();
    windowToImageFilter->ReadFrontBufferOff();

    vtkSmartPointer<vtkImageWriter> writer = nullptr;
    if (file_path.endsWith(".png")) {
        writer = vtkSmartPointer<vtkPNGWriter>::New();
    }

    if (writer) {
        writer->SetFileName(file_path.toStdString().c_str());
        writer->SetInputConnection(windowToImageFilter->GetOutputPort());
        writer->Write();
    }
}

QImage gnomonVtkView::toImage(void)
{
    if (dd->window) {
        // TODO: allow user to pass a size (through a dialog)
        // auto old_size = dd->window->GetSize();
        // dd->window->SetSize(1000, 1000);

        vtkSmartPointer<vtkWindowToImageFilter> windowToImageFilter = vtkSmartPointer<vtkWindowToImageFilter>::New();
        windowToImageFilter->SetInput(dd->window);
        windowToImageFilter->SetInputBufferTypeToRGBA();
        windowToImageFilter->ReadFrontBufferOff();
        windowToImageFilter->Update();

        vtkSmartPointer<vtkImageData> renderedImage = windowToImageFilter->GetOutput();
        int height = renderedImage->GetDimensions()[0];
        int width = renderedImage->GetDimensions()[1];
        QImage image(height, width, QImage::Format_RGBA64);

        QRgba64 *rgbaPtr = reinterpret_cast<QRgba64 *>(image.bits());
        for (int col = 0; col < width; ++col) {
            for (int row = 0; row < height; ++row) {
                double r, g, b, a;
                r = reinterpret_cast<unsigned char *>(renderedImage->GetScalarPointer(row , width-1 - col, 0))[0];
                g = reinterpret_cast<unsigned char *>(renderedImage->GetScalarPointer(row , width-1 - col, 0))[1];
                b = reinterpret_cast<unsigned char *>(renderedImage->GetScalarPointer(row , width-1 - col, 0))[2];
                a = reinterpret_cast<unsigned char *>(renderedImage->GetScalarPointer(row , width-1 - col, 0))[3];
                *(rgbaPtr) = QColor(r, g, b, a).rgba64();
                ++rgbaPtr;
            }
        }
        return image;
    } else {
        return QImage();
    }
}

void gnomonVtkView::startPicking() {
    if(!d->forms.contains("gnomonCellImage") ||
        d->visualizationCommands["gnomonCellImage"]->visualizationName() != "cellImageVtkVisualizationMarchingCubes") {
        qWarning() << "Picking not implemented for : "
                   <<  d->visualizationCommands["gnomonCellImage"]->visualizationName()
                   << " only cellImageVtkVisualizationMarchingCubes has picking";
        return;
    }

    // backup old interactor style
    dd->old_style = dd->interactor()->GetInteractorStyle();

    dd->picking_visu = std::dynamic_pointer_cast<gnomonAbstractCellImageVtkVisualization>(d->visualizationCommands["gnomonCellImage"]->visualization());

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

void gnomonVtkView::onSliceChanged(double slice)
{
    // d->slice_slider->setValue(slice);
    Q_UNUSED(slice);
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

void gnomonVtkView::drop(int index, bool new_visu)
{
    if (this->empty()) {
        if (vtkCamera *cam = gnomonFormManager::instance()->getCamera(index)) {
            this->setCamera(cam);
        }
    }
    gnomonAbstractView::drop(index, new_visu);
}

QJsonObject gnomonVtkView::serialize(void) {
    auto serialization = gnomonAbstractView::serialize();
    {
        vtkSmartPointer<vtkCamera> cam = dd->renderer3D->GetActiveCamera();
        gnomonCameraParameters p;
        p.fromVtkCamera(cam);
        QJsonObject camera_json = p.toJson();
        serialization.insert("active_camera", camera_json);
    }

    QJsonObject other_cameras;
    for(auto it = dd->cameras.keyValueBegin(); it!=dd->cameras.keyValueEnd(); it++) {
        vtkSmartPointer<vtkCamera> cam = it->second;
        gnomonCameraParameters p;
        p.fromVtkCamera(cam);
        QJsonObject camera_json = p.toJson();
        other_cameras.insert(QString::number(it->first), camera_json);
    }
    serialization.insert("other_cameras", other_cameras);

    serialization.insert("mode", dd->mode);
    serialization.insert("ori", dd->ori);
    serialization.insert("representation", dd->representation);
    serialization.insert("grid_type", dd->grid_type);
    serialization.insert("grid_orientation", dd->grid_orientation);
    serialization.insert("grid_visible", dd->grid_visible);
    serialization.insert("axes_visible", dd->axes_visible);
    serialization.insert("camera_fixed", dd->camera_fixed);
    serialization.insert("c_x", dd->c_x);
    serialization.insert("c_y", dd->c_y);
    serialization.insert("c_z", dd->c_z);
    serialization.insert("xBounds0", dd->xBounds[0]);
    serialization.insert("xBounds1", dd->xBounds[1]);
    serialization.insert("yBounds0", dd->yBounds[0]);
    serialization.insert("yBounds1", dd->yBounds[1]);
    serialization.insert("zBounds0", dd->zBounds[0]);
    serialization.insert("zBounds1", dd->zBounds[1]);

    QJsonArray picked_cells;
    for(const auto &val: dd->picked_cells) {
        picked_cells.append((int)val);
    }
    serialization.insert("picked_cells", picked_cells);

    return serialization;
}

void gnomonVtkView::unSerialize(const QJsonObject &serialization) {

    disconnect(dd->connectRenderWindowAdded);

    auto lambda = [=] () {
        gnomonAbstractView::unSerialize(serialization);

        QJsonObject other_cameras = serialization.value("other_cameras").toObject();
        for(auto key: other_cameras.keys()) {
            if(dd->cameras.contains((gnomonVtkView::Orientation)key.toInt())) {
                vtkSmartPointer<vtkCamera> cam = dd->cameras[(gnomonVtkView::Orientation)key.toInt()];
                gnomonCameraParameters p;
                p.fromJson(other_cameras[key].toObject());
                p.toVtkCamera(cam);
            }
        }

        auto mode = (gnomonVtkView::Mode)serialization.value("mode").toInt();
        auto ori = (gnomonVtkView::Orientation)serialization.value("ori").toInt();
        auto representation = (gnomonVtkView::Representation)serialization.value("representation").toInt();
        auto grid_type = (gnomonVtkView::Grid)serialization.value("grid_type").toInt();
        auto grid_orientation = (gnomonVtkView::Orientation)serialization.value("grid_orientation").toInt();

        dd->c_x = serialization.value("c_x").toDouble();
        dd->c_y = serialization.value("c_y").toDouble();
        dd->c_z = serialization.value("c_z").toDouble();

        double x0 = serialization.value("xBounds0").toDouble();
        double x1 = serialization.value("xBounds1").toDouble();
        double y0 = serialization.value("yBounds0").toDouble();
        double y1 = serialization.value("yBounds1").toDouble();
        double z0 = serialization.value("zBounds0").toDouble();
        double z1 = serialization.value("zBounds1").toDouble();
        setBounds(x0, x1, y0, y1, z0, z1);

        dd->ori = ori;
        setRepresentation(representation);
        if(mode == gnomonVtkView::Mode::VIEW_MODE_3D) {
            switchTo3D();
        } else {
            switchTo2D();
        }

        setGridType(grid_type);
        setGridOrientation(grid_orientation);
        setGridVisible(serialization.value("grid_visible").toBool());
        setAxesVisible(serialization.value("axes_visible").toBool());
        setCameraFixed(serialization.value("camera_fixed").toBool());

        auto camera_json = serialization["active_camera"].toObject();
        vtkSmartPointer<vtkCamera> cam = dd->renderer3D->GetActiveCamera();
        gnomonCameraParameters p;
        p.fromJson(camera_json);
        p.toVtkCamera(cam);

        dd->picked_cells.clear();
        QJsonArray picked_cells = serialization.value("picked_cells").toArray();
        for(const auto &val: picked_cells) {
            dd->picked_cells.append(val.toInt());
        }

    };

    if(dd->window) {
        lambda();
    } else {
        dd->connectRenderWindowAdded = connect(this, &gnomonVtkView::renderWindowAssociated, lambda);
    }

}

// ///////////////////////////////////////////////////////////////////

#include "gnomonVtkView.moc"

//
// gnomonVtkView.cpp ends here
