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

#include "gnomonImageManager.h"
#include "gnomonViewVolumic.h"
#include "gnomonToolBar.h"
#include "gnomonWorkspaceBrowser.h"
#include "gnomonWorkspaceFusion.h"
#include "gnomonWorkspaceSegmentation.h"
#include "gnomonWorkspacePreprocess.h"
#include "gnomonViewVolumicOverlay.h"

#include <gnomonCore/gnomonImagesSerieReaderCommand>
#include <gnomonCore/gnomonImagesSerie>

#include <gnomonVisualization/gnomonColorMapEditor.h>

#include <dtkImagingCore>

#include "gnomonLandmarkActor.h"

#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkCellPicker.h>
#include <vtkColorTransferFunction.h>
#include <vtkContourFilter.h>
#include <vtkDataArray.h>
#include <vtkDataSetMapper.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkImageBlend.h>
#include <vtkImageCast.h>
#include <vtkImageData.h>
#include <vtkImageMapToColors.h>
#include <vtkImagePlaneWidget.h>
#include <vtkImageViewer2.h>
#include <vtkImageMapToColors.h>
#include <vtkImageMapToWindowLevelColors.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkInteractorStyleImage.h>
#include <vtkLookupTable.h>
#include <vtkObjectFactory.h>
#include <vtkPiecewiseFunction.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRendererCollection.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkSmartVolumeMapper.h>
#include <vtkSphereSource.h>
#include <vtkVolume.h>
#include <vtkVolumeProperty.h>

#include <QVTKInteractor.h>
#include <QVTKOpenGLWidget.h>

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class gnomonViewVolumicList : public QListWidget
{
    Q_OBJECT

public:
     gnomonViewVolumicList(QWidget *parent = nullptr);
    ~gnomonViewVolumicList(void);

public:
};

gnomonViewVolumicList::gnomonViewVolumicList(QWidget *parent) : QListWidget(parent)
{
    this->setContentsMargins(0, 0, 0, 0);
}

gnomonViewVolumicList::~gnomonViewVolumicList(void)
{

}

// ///////////////////////////////////////////////////////////////////
// gnomonViewVolumicInteractorImage
// ///////////////////////////////////////////////////////////////////

class gnomonViewVolumicInteractorImage : public vtkInteractorStyleImage
{
public:
    static gnomonViewVolumicInteractorImage *New(void);

public:
    virtual void OnMouseMove(void) override
        {
            if(this->move_actor) {
                return;
            }

            vtkInteractorStyleImage::OnMouseMove();

        }

    virtual void OnLeftButtonDown(void) override
    {
        vtkInteractorStyleImage::OnLeftButtonDown();

        if(!this->picker)
            return;

        if(!this->picker->isToggled())
            return;

        if(!this->image)
            return;

        int *pos = this->GetInteractor()->GetEventPosition();

        vtkSmartPointer<vtkCellPicker> picker = vtkSmartPointer<vtkCellPicker>::New();
        picker->SetTolerance(0.0005);
        picker->Pick(pos[0], pos[1], 0, this->GetDefaultRenderer());

        if(picker->GetCellId() == -1) return;

        double *picked = picker->GetPickPosition();

        if(picker->GetActor()) {
            this->move_actor = picker->GetActor();
        } else {
            std::size_t id = q->addLandmark(this->landmark_id, picked[0], picked[1], picked[2]);

            emit q->landmarkAdded(id, picked[0], picked[1], picked[2]);
        }

        this->q->render();
    }

    virtual void OnLeftButtonUp(void) override
    {
        vtkInteractorStyleImage::OnLeftButtonUp();

        if(!this->picker)
            return;

        if(!this->picker->isToggled())
            return;

        if(!this->image)
            return;

        if(!move_actor) return;

        int *pos = this->GetInteractor()->GetEventPosition();

        vtkSmartPointer<vtkCellPicker> picker = vtkSmartPointer<vtkCellPicker>::New();
        picker->SetTolerance(0.0005);
        picker->Pick(pos[0], pos[1], 0, this->GetDefaultRenderer());

        if(picker->GetCellId() == -1) return;

        double *picked = picker->GetPickPosition();
        this->move_actor->SetPosition(picked[0], picked[1], picked[2]);

        this->move_actor = nullptr;

        this->q->render();
    }

    virtual void OnMiddleButtonDown() override
    {
        vtkInteractorStyleImage::OnMiddleButtonDown();

        if(!this->picker)
            return;

        if(!this->picker->isToggled())
            return;

        if(!this->image)
            return;

        int *pos = this->GetInteractor()->GetEventPosition();

        vtkSmartPointer<vtkCellPicker> picker = vtkSmartPointer<vtkCellPicker>::New();
        picker->SetTolerance(0.0005);
        picker->Pick(pos[0], pos[1], 0, this->GetDefaultRenderer());

        if(picker->GetCellId() == -1) return;

        if(!picker->GetActor()) return;

        double *p = picker->GetActor()->GetPosition();

        gnomonLandmarkActor *landmark = static_cast<gnomonLandmarkActor *>(picker->GetActor());

        q->removeLandmark(landmark->id());

        emit q->landmarkRemoved(landmark->id());

        this->q->render();
    }

public:
    gnomonViewVolumic *q = nullptr;
    gnomonViewVolumicOverlay *picker = nullptr;

    vtkActor *move_actor = nullptr;
    std::size_t landmark_id = 0;

public:
    vtkSmartPointer<vtkImageData> image = nullptr;
};

vtkStandardNewMacro(gnomonViewVolumicInteractorImage);

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class gnomonViewVolumicPrivate : public QVTKOpenGLWidget
{
    Q_OBJECT

public:
    enum Orientation {
        SLICE_ORIENTATION_XY = 2,
        SLICE_ORIENTATION_XZ = 1,
        SLICE_ORIENTATION_YZ = 0
    };

public:
     gnomonViewVolumicPrivate(QWidget *parent = Q_NULLPTR);
    ~gnomonViewVolumicPrivate(void);

public slots:
    void enableInteractor(void);
    void disableInteractor(void);

public slots:
    void exportToManager(void);

public:
    QSize sizeHint(void) const;

protected:
    void resizeEvent(QResizeEvent *);

public:
    Orientation orientation(void);

public:
    void setSliceOrientation(Orientation orientation);

public:
    void toggleChannel(const QString&);
    void activateChannel(const QString&);

public:
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> window;
    vtkSmartPointer<vtkRenderer> renderer2D;
    vtkSmartPointer<vtkRenderer> renderer3D;

public:
    gnomonViewVolumic *q = nullptr;

public:
    vtkSmartPointer<vtkImagePlaneWidget> planeWidget[3];

public:
    vtkSmartPointer<vtkImageViewer2> viewer = nullptr;
    vtkSmartPointer<vtkVolume> volume = nullptr;
    vtkSmartPointer<vtkSmartVolumeMapper> volume_mapper = nullptr;

public:
    vtkSmartPointer<vtkImageBlend> blender = nullptr;

public:
    gnomonViewVolumicInteractorImage *image_interactor = nullptr;
    gnomonColorMapEditor *color_map_editor = nullptr;
    QMap<QString, QMap<double, QColor>> channels_lut;
    QString last_channel_toggled;

public:
    gnomonViewVolumicOverlay *export_button = nullptr;
    gnomonViewVolumicOverlay *renderer2D_button = nullptr;
    gnomonViewVolumicOverlay *renderer3D_button = nullptr;
    gnomonViewVolumicOverlay *renderer2D_XY = nullptr;
    gnomonViewVolumicOverlay *renderer2D_XZ = nullptr;
    gnomonViewVolumicOverlay *renderer2D_YZ = nullptr;
    gnomonViewVolumicOverlay *picker = nullptr;
    gnomonViewVolumicOverlay *blending = nullptr;
    gnomonViewVolumicOverlay *sync = nullptr;
    gnomonViewVolumicOverlay *stack = nullptr;
    QList<gnomonViewVolumicOverlay *> layers;
    gnomonViewVolumicOverlay *clut = nullptr;

public:
    gnomonViewVolumicList *blending_list = nullptr;

public:
    int syncing_count = 0;
    QTimer *syncing_timer = nullptr;
    bool synced = false;

public:
    gnomonImagesSeriePtr images_serie;

public:
    QSlider *slice_slider;

public:
    QSlider *time_slider;

public:
    QSlider *opacity;

public:
    int x = 0, c_x = 0;
    int y = 0, c_y = 0;
    int z = 0, c_z = 0;

signals:
    void sliceOrientationChanged(int);
};

gnomonViewVolumicPrivate::gnomonViewVolumicPrivate(QWidget *parent) : QVTKOpenGLWidget(parent)
{
    QColor background_color = QColor(GNOMON_STYLE_BACKGROUNDCOLOR);

    this->renderer2D = vtkSmartPointer<vtkRenderer>::New();
    this->renderer2D->SetBackground(background_color.redF(), background_color.greenF(), background_color.blueF());

    this->renderer3D = vtkSmartPointer<vtkRenderer>::New();
    this->renderer3D->SetBackground(background_color.redF(), background_color.greenF(), background_color.blueF());

    this->window = vtkGenericOpenGLRenderWindow::New();
    this->window->AddRenderer(this->renderer2D);
    this->window->AddRenderer(this->renderer3D);

    this->SetRenderWindow(this->window);
    this->setEnableHiDPI(true);

    this->export_button = new gnomonViewVolumicOverlay(fa::arrowcircleup, "", this);
    this->renderer2D_button = new gnomonViewVolumicOverlay(fa::square, "", this);
    this->renderer3D_button = new gnomonViewVolumicOverlay(fa::cube, "", this);
    this->renderer2D_XY = new gnomonViewVolumicOverlay(":gnomon/gnomonViewVolumic-XY.png", "", this);
    this->renderer2D_XY->toggle(true);
    this->renderer2D_XZ = new gnomonViewVolumicOverlay(":gnomon/gnomonViewVolumic-XZ-off.png", "", this);
    this->renderer2D_XZ->toggle(false);
    this->renderer2D_YZ = new gnomonViewVolumicOverlay(":gnomon/gnomonViewVolumic-YZ-off.png", "", this);
    this->renderer2D_YZ->toggle(false);
    this->picker = new gnomonViewVolumicOverlay(fa::crosshairs, "", this);
    this->picker->toggle(false);
    this->blending = new gnomonViewVolumicOverlay(fa::adjust, "", this);
    this->blending->toggle(false);
    this->sync = new gnomonViewVolumicOverlay(fa::unlock, "", this);
    this->sync->toggle(false);
    this->stack = new gnomonViewVolumicOverlay(fa::eye, "", this);
    this->stack->toggle(false);
    this->clut = new gnomonViewVolumicOverlay(fa::aligncenter, "", this);
    this->clut->toggle(false);
    this->color_map_editor = new gnomonColorMapEditor(this);
    this->color_map_editor->setVisible(false);
    this->blending_list = new gnomonViewVolumicList(this);
    this->blending_list->resize(200, 100);
    this->blending_list->setVisible(false);

    vtkImageData *dummy = vtkImageData::New();
    dummy->SetDimensions(1, 1, 1);
    dummy->SetSpacing(1, 1, 1);
#if VTK_MAJOR_VERSION <= 5
    dummy->SetNumberOfScalarComponents(4);
    dummy->SetScalarTypeToUnsignedChar();
#else
    dummy->AllocateScalars(VTK_UNSIGNED_CHAR,4);
#endif

    this->blender = vtkSmartPointer<vtkImageBlend>::New();

    this->viewer = vtkSmartPointer<vtkImageViewer2>::New();
    this->viewer->SetSliceOrientationToXY();
    this->viewer->SetRenderWindow(this->window);
    this->viewer->SetRenderer(this->renderer2D);
    this->viewer->SetupInteractor(this->GetInteractor());
    this->viewer->GetWindowLevel()->SetOutputFormatToRGB();
    this->viewer->SetInputData(dummy);

    for (int i = 0; i < 3; i++) {

        double color[3] = { 0, 0, 0 }; color[i] = 1;

        planeWidget[i] = vtkSmartPointer<vtkImagePlaneWidget>::New();
        planeWidget[i]->SetInteractor(this->GetInteractor());
        planeWidget[i]->SetInputData(dummy);
        planeWidget[i]->SetPlaneOrientation(i);
        planeWidget[i]->RestrictPlaneToVolumeOn();
        planeWidget[i]->GetPlaneProperty()->SetColor(color);
        planeWidget[i]->SetLeftButtonAction(vtkImagePlaneWidget::VTK_SLICE_MOTION_ACTION);
        planeWidget[i]->SetMarginSizeX(0);
        planeWidget[i]->SetMarginSizeY(0);
    }

    // ///////////////////////////////////////////////////////////////////

    this->image_interactor = gnomonViewVolumicInteractorImage::New();
    this->image_interactor->SetDefaultRenderer(this->renderer2D);
    this->image_interactor->picker = this->picker;

    this->GetInteractor()->SetInteractorStyle(this->image_interactor);

    // ///////////////////////////////////////////////////////////////////

    connect(this->export_button, SIGNAL(iconClicked()), this, SLOT(exportToManager()));

    connect(this->picker, &gnomonViewVolumicOverlay::iconClicked, [=] () {

            this->picker->toggle(!this->picker->isToggled());

            q->setCursor(this->picker->isToggled() ? Qt::CrossCursor : Qt::ArrowCursor);
    });

    connect(this->blending, &gnomonViewVolumicOverlay::iconClicked, [=] () {

            this->blending->toggle(!this->blending->isToggled());

            this->opacity->setVisible(this->blending->isToggled());
            this->blending_list->setVisible(this->blending->isToggled());

            if(!this->blending->isToggled()) {
                this->blending_list->clear();

                if (this->images_serie->image())
                    q->setImage(this->images_serie->image());

                this->blender->RemoveAllInputs();
            }
    });

    connect(this->sync, &gnomonViewVolumicOverlay::iconClicked, [=] () {

            this->sync->toggle(!this->sync->isToggled());

            if (this->sync->isToggled())
                emit q->linking();
            else
                emit q->unlinking();

            if (this->sync->isToggled() && !this->synced) {
                this->syncing_count = 0;
                if(!this->syncing_timer)
                    this->syncing_timer = new QTimer(this);
                connect(this->syncing_timer, &QTimer::timeout, [=] () {
                        this->sync->changeColor((this->syncing_count++ % 2) ? Qt::gray : Qt::white);
                        this->sync->update();
                        if (this->syncing_count == 11) {
                            this->sync->toggle(false);
                            this->syncing_timer->stop();
                            this->syncing_timer->disconnect();
                            delete this->syncing_timer;
                            this->syncing_timer = nullptr;
                            emit q->unlinking();
                        }
                    });
                this->syncing_timer->start(500);
            }
    });

    connect(this->stack, &gnomonViewVolumicOverlay::iconClicked, [=] () {

            for(auto& layer : this->layers) {
                layer->setVisible(!this->stack->isToggled());
            }

            this->stack->toggle(!this->stack->isToggled());
    });

    connect(this->clut, &gnomonViewVolumicOverlay::iconClicked, [=] () {

            this->color_map_editor->setVisible(!this->clut->isToggled());

            this->clut->toggle(!this->clut->isToggled());
        });

    connect(this->color_map_editor, &gnomonColorMapEditor::valueChanged, [=] (const QMap<double, QColor>& clut) {
            this->channels_lut[last_channel_toggled] = clut;
            q->applyLut(clut);
        });

}

gnomonViewVolumicPrivate::~gnomonViewVolumicPrivate(void)
{
    this->image_interactor->Delete();
}

void gnomonViewVolumicPrivate::enableInteractor(void)
{
    this->GetInteractor()->Enable();
}

void gnomonViewVolumicPrivate::disableInteractor(void)
{
    this->GetInteractor()->Disable();
}

void gnomonViewVolumicPrivate::exportToManager(void)
{
    if(!this->images_serie)
        return;

    QWidget *parent = this->parentWidget();

    while(!dynamic_cast< QStackedWidget *>(parent))
        parent = parent->parentWidget();

    QStackedWidget *stack = dynamic_cast<QStackedWidget *>(parent);

    if(gnomonWorkspaceBrowser* workspace = dynamic_cast<gnomonWorkspaceBrowser *>(stack->currentWidget()))
        gnomonImageManager::instance()->addImage(this->images_serie, gnomonToolBar::browser_color);
    else if(gnomonWorkspaceFusion* workspace = dynamic_cast<gnomonWorkspaceFusion *>(stack->currentWidget()))
        gnomonImageManager::instance()->addImage(this->images_serie, gnomonToolBar::fusion_color);
    else if(gnomonWorkspaceSegmentation* workspace = dynamic_cast<gnomonWorkspaceSegmentation *>(stack->currentWidget()))
        gnomonImageManager::instance()->addImage(this->images_serie, gnomonToolBar::segmentation_color);
    else if(gnomonWorkspacePreprocess* workspace = dynamic_cast<gnomonWorkspacePreprocess *>(stack->currentWidget()))
        gnomonImageManager::instance()->addImage(this->images_serie, gnomonToolBar::preprocess_color);
    else
        gnomonImageManager::instance()->addImage(this->images_serie, gnomonToolBar::registration_color);
}

QSize gnomonViewVolumicPrivate::sizeHint(void) const
{
    return QSize(800, 600);
}

void gnomonViewVolumicPrivate::resizeEvent(QResizeEvent *event)
{
    this->export_button->move(event->size().width() - 40, 10);
    this->renderer2D_button->move(10, 10);
    this->renderer3D_button->move(50, 10);
    this->renderer2D_XY->move(10,  50);
    this->renderer2D_XZ->move(10,  90);
    this->renderer2D_YZ->move(10, 130);
    this->picker->move(90, 10);
    this->blending->move(130, 10);
    this->sync->move(event->size().width() - 90, 10);
    this->stack->move(event->size().width() - 40, 50);
    this->opacity->move(20, event->size().height() - 100 - 10 - 30);
    this->blending_list->move(15, event->size().height() - 100 - 10);
    std::size_t i = 0;
    for(auto& layer : this->layers) {
        layer->move(event->size().width() - layer->width() + 15, 80 + i * 25);
        ++i;
    }
    this->clut->move(event->size().width() - 40, event->size().height() - 40);
    this->color_map_editor->move(event->size().width() - 140, event->size().height() - 150);

    QVTKOpenGLWidget::resizeEvent(event);
}

gnomonViewVolumicPrivate::Orientation gnomonViewVolumicPrivate::orientation(void)
{
    switch(this->viewer->GetSliceOrientation()) {
    case 2:
        return SLICE_ORIENTATION_XY;
    case 1:
        return SLICE_ORIENTATION_XZ;
    case 0:
        return SLICE_ORIENTATION_YZ;
    default:
        return SLICE_ORIENTATION_XY;
    }
}

void gnomonViewVolumicPrivate::setSliceOrientation(Orientation orientation)
{
    switch(orientation) {
    case SLICE_ORIENTATION_YZ:
        this->viewer->SetSliceOrientation(vtkImageViewer2::SLICE_ORIENTATION_YZ);
        emit sliceOrientationChanged(SLICE_ORIENTATION_YZ);
        break;
    case SLICE_ORIENTATION_XZ:
        this->viewer->SetSliceOrientation(vtkImageViewer2::SLICE_ORIENTATION_XZ);
        emit sliceOrientationChanged(SLICE_ORIENTATION_XZ);
        break;
    case SLICE_ORIENTATION_XY:
        this->viewer->SetSliceOrientation(vtkImageViewer2::SLICE_ORIENTATION_XY);
        emit sliceOrientationChanged(SLICE_ORIENTATION_XY);
        break;
    default:
        break;
    };
}

// ///////////////////////////////////////////////////////////////////
// gnomonViewVolumic
// ///////////////////////////////////////////////////////////////////

QMap<double, QColor> gnomonViewVolumic::grey_colormap = QMap<double, QColor>({
        {0., QColor(0, 0, 0, 255)},
        {1., QColor(255, 255, 255, 255)} });


gnomonViewVolumic::gnomonViewVolumic(QWidget *parent) : QFrame(parent)
{
    d = new gnomonViewVolumicPrivate;
    d->q = this;

    connect(d->renderer2D_button, SIGNAL(iconClicked()), this, SLOT(switchTo2D()));
    connect(d->renderer3D_button, SIGNAL(iconClicked()), this, SLOT(switchTo3D()));
    connect(d->renderer2D_XY, SIGNAL(iconClicked()), this, SLOT(switchTo2DXY()));
    connect(d->renderer2D_XZ, SIGNAL(iconClicked()), this, SLOT(switchTo2DXZ()));
    connect(d->renderer2D_YZ, SIGNAL(iconClicked()), this, SLOT(switchTo2DYZ()));

    d->image_interactor->q = this;

    d->slice_slider = new QSlider(this);
    d->slice_slider->setObjectName("prout");
    d->slice_slider->setOrientation(Qt::Vertical);
    d->slice_slider->setMinimum(0);
    d->slice_slider->setMaximum(1);
    d->slice_slider->setValue(0);

    connect(d->slice_slider, SIGNAL(valueChanged(int)), this, SLOT(sliceChange(int)));

    d->time_slider = new QSlider(this);
    d->time_slider->setObjectName("prout");
    d->time_slider->setOrientation(Qt::Horizontal);
    d->time_slider->setMinimum(0);
    d->time_slider->setMaximum(1);
    d->time_slider->setValue(0);
    d->time_slider->setEnabled(true);
    d->time_slider->setTickPosition(QSlider::TicksAbove);

    connect(d->time_slider, SIGNAL(valueChanged(int)), this, SLOT(timeChange(int)));


    // connect(d, static_cast<void(gnomonViewVolumicPrivate::*)(int)>(&gnomonViewVolumicPrivate::sliceOrientationChanged), this, &gnomonViewVolumic::sliceOrientationChanged);
    connect(d, &gnomonViewVolumicPrivate::sliceOrientationChanged, this, &gnomonViewVolumic::sliceOrientationChanged);

    QLabel *time_slider_label = new QLabel("time:");
    QGridLayout *layout = new QGridLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(d->slice_slider, 0, 0, 1, 1);
    layout->addWidget(d, 0, 1, 1, 1);
    layout->addWidget(time_slider_label,1,0,1,1);
    layout->addWidget(d->time_slider,1,1,1,1);

    this->setAcceptDrops(true);

    d->opacity = new QSlider(this);
    d->opacity->setOrientation(Qt::Horizontal);
    d->opacity->setMinimum(0);
    d->opacity->setMaximum(100);
    d->opacity->setValue(50);
    d->opacity->setFixedWidth(200);
    d->opacity->setVisible(false);

    connect(d->opacity, &QSlider::valueChanged, [=] (int value) {

        double v = double(value) / 100.0;

        d->blender->SetOpacity(0,     v);
        d->blender->SetOpacity(1, 1 - v);
        d->blender->Update();

        d->viewer->SetInputData(d->blender->GetOutput());

        this->render();
    });
}

gnomonViewVolumic::~gnomonViewVolumic(void)
{
    delete d;
}

void gnomonViewVolumic::link(gnomonViewVolumic *other)
{
    if (d->syncing_timer)
        d->syncing_timer->stop();

    d->sync->toggle(true);
    d->sync->changeIcon(fa::lock);

    d->synced = true;

    // ///////////////////////////////////////////////////////////////

    d->renderer2D->SetActiveCamera(other->d->renderer2D->GetActiveCamera());
    d->renderer3D->SetActiveCamera(other->d->renderer3D->GetActiveCamera());

    other->d->GetRenderWindow()->AddObserver(vtkCommand::RenderEvent, this, &gnomonViewVolumic::render);

    connect(other, SIGNAL(switchedTo3D()), this, SLOT(switchTo3D()));
    connect(other, SIGNAL(switchedTo2D()), this, SLOT(switchTo2D()));
    connect(other, SIGNAL(switchedTo2DXY()), this, SLOT(switchTo2DXY()));
    connect(other, SIGNAL(switchedTo2DXZ()), this, SLOT(switchTo2DXZ()));
    connect(other, SIGNAL(switchedTo2DYZ()), this, SLOT(switchTo2DYZ()));
    connect(other, SIGNAL(sliceChanged(int)), this, SLOT(sliceChange(int)));
}

void gnomonViewVolumic::unlink(gnomonViewVolumic *other)
{
    if (d->syncing_timer) {
        d->syncing_timer->stop();
        d->syncing_timer->disconnect();
        delete d->syncing_timer;
        d->syncing_timer = nullptr;
    }

    d->sync->toggle(false);
    d->sync->changeIcon(fa::unlock);

    d->synced = false;

    // ///////////////////////////////////////////////////////////////

    vtkSmartPointer<vtkCamera> camera2D = vtkCamera::New();
    camera2D->ShallowCopy(d->renderer2D->GetActiveCamera());

    vtkSmartPointer<vtkCamera> camera3D = vtkCamera::New();
    camera3D->ShallowCopy(d->renderer2D->GetActiveCamera());

    d->renderer2D->SetActiveCamera(camera2D);
    d->renderer3D->SetActiveCamera(camera3D);

    // ///////////////////////////////////////////////////////////////

    disconnect(other, SIGNAL(switchedTo3D()), this, SLOT(switchTo3D()));
    disconnect(other, SIGNAL(switchedTo2D()), this, SLOT(switchTo2D()));
    disconnect(other, SIGNAL(switchedTo2DXY()), this, SLOT(switchTo2DXY()));
    disconnect(other, SIGNAL(switchedTo2DXZ()), this, SLOT(switchTo2DXZ()));
    disconnect(other, SIGNAL(switchedTo2DYZ()), this, SLOT(switchTo2DYZ()));
    disconnect(other, SIGNAL(sliceChanged(int)), this, SLOT(sliceChange(int)));
}

void gnomonViewVolumic::switchTo3D(void)
{
    if (d->renderer3D_button->isToggled()) return;

    d->renderer2D_button->toggle(false);
    d->renderer2D_button->setEnabled(true);

    d->renderer3D_button->toggle(true);
    d->renderer3D_button->setEnabled(false);

    d->renderer2D_XY->setVisible(false);
    d->renderer2D_XZ->setVisible(false);
    d->renderer2D_YZ->setVisible(false);

    d->renderer2D->DrawOff();
    d->renderer2D->InteractiveOff();

    vtkSmartPointer<vtkInteractorStyleTrackballCamera> style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();

    d->GetInteractor()->SetInteractorStyle(style);

    d->renderer3D->InteractiveOn();
    d->renderer3D->DrawOn();

    d->slice_slider->setEnabled(false);

    d->planeWidget[0]->Off();
    d->planeWidget[1]->Off();
    d->planeWidget[2]->Off();

    d->planeWidget[0]->On();
    d->planeWidget[1]->On();
    d->planeWidget[2]->On();

    this->render();

    emit switchedTo3D();
}

void gnomonViewVolumic::switchTo2D(void)
{
    if (d->renderer2D_button->isToggled()) return;

    d->renderer2D_button->toggle(true);
    d->renderer2D_button->setEnabled(false);

    d->renderer3D_button->toggle(false);
    d->renderer3D_button->setEnabled(true);

    d->renderer2D_XY->setVisible(true);
    d->renderer2D_XZ->setVisible(true);
    d->renderer2D_YZ->setVisible(true);

    d->renderer3D->DrawOff();
    d->renderer3D->InteractiveOff();

    d->GetInteractor()->SetInteractorStyle(d->image_interactor);

    d->renderer2D->InteractiveOn();
    d->renderer2D->DrawOn();

    d->slice_slider->setEnabled(true);

    d->planeWidget[0]->Off();
    d->planeWidget[1]->Off();
    d->planeWidget[2]->Off();

    if (d->renderer2D_XY->isToggled()) {
        d->viewer->SetSlice(d->c_z);
        d->planeWidget[0]->On();
        d->planeWidget[1]->On();
        d->planeWidget[2]->Off();
    }

    if (d->renderer2D_XZ->isToggled()) {
        d->viewer->SetSlice(d->c_y);
        d->planeWidget[0]->On();
        d->planeWidget[1]->Off();
        d->planeWidget[2]->On();
    }

    if (d->renderer2D_YZ->isToggled()) {
        d->viewer->SetSlice(d->c_x);
        d->planeWidget[0]->Off();
        d->planeWidget[1]->On();
        d->planeWidget[2]->On();
    }

    this->render();

    emit switchedTo2D();
}

void gnomonViewVolumic::switchTo2DXY(void)
{
    if (d->renderer2D_XY->isToggled()) return;

    d->renderer2D_XY->changePath(":gnomon/gnomonViewVolumic-XY.png");
    d->renderer2D_XZ->changePath(":gnomon/gnomonViewVolumic-XZ-off.png");
    d->renderer2D_YZ->changePath(":gnomon/gnomonViewVolumic-YZ-off.png");

    d->renderer2D_XY->toggle(true);
    d->renderer2D_XZ->toggle(false);
    d->renderer2D_YZ->toggle(false);

    d->setSliceOrientation(gnomonViewVolumicPrivate::SLICE_ORIENTATION_XY);

    d->viewer->SetSlice(d->c_z);

    d->planeWidget[0]->On();
    d->planeWidget[1]->On();
    d->planeWidget[2]->Off();

    d->slice_slider->blockSignals(true);
    d->slice_slider->setMaximum(d->z);
    d->slice_slider->setValue(d->c_z);
    d->slice_slider->blockSignals(false);

    emit switchedTo2DXY();
}

void gnomonViewVolumic::switchTo2DXZ(void)
{
    if (d->renderer2D_XZ->isToggled()) return;

    d->renderer2D_XY->changePath(":gnomon/gnomonViewVolumic-XY-off.png");
    d->renderer2D_XZ->changePath(":gnomon/gnomonViewVolumic-XZ.png");
    d->renderer2D_YZ->changePath(":gnomon/gnomonViewVolumic-YZ-off.png");

    d->renderer2D_XY->toggle(false);
    d->renderer2D_XZ->toggle(true);
    d->renderer2D_YZ->toggle(false);

    d->setSliceOrientation(gnomonViewVolumicPrivate::SLICE_ORIENTATION_XZ);

    d->viewer->SetSlice(d->c_y);

    d->planeWidget[0]->On();
    d->planeWidget[1]->Off();
    d->planeWidget[2]->On();

    d->slice_slider->blockSignals(true);
    d->slice_slider->setMaximum(d->y);
    d->slice_slider->setValue(d->c_y);
    d->slice_slider->blockSignals(false);

    emit switchedTo2DXZ();
}

void gnomonViewVolumic::switchTo2DYZ(void)
{
    if (d->renderer2D_YZ->isToggled()) return;

    d->renderer2D_XY->changePath(":gnomon/gnomonViewVolumic-XY-off.png");
    d->renderer2D_XZ->changePath(":gnomon/gnomonViewVolumic-XZ-off.png");
    d->renderer2D_YZ->changePath(":gnomon/gnomonViewVolumic-YZ.png");

    d->renderer2D_XY->toggle(false);
    d->renderer2D_XZ->toggle(false);
    d->renderer2D_YZ->toggle(true);

    d->setSliceOrientation(gnomonViewVolumicPrivate::SLICE_ORIENTATION_YZ);

    d->viewer->SetSlice(d->c_x);

    d->planeWidget[0]->Off();
    d->planeWidget[1]->On();
    d->planeWidget[2]->On();

    d->slice_slider->blockSignals(true);
    d->slice_slider->setMaximum(d->x);
    d->slice_slider->setValue(d->c_x);
    d->slice_slider->blockSignals(false);

    emit switchedTo2DYZ();
}

void gnomonViewVolumic::sliceChange(int value)
{
    if (d->viewer->GetSlice() == value)
        return;

    d->viewer->SetSlice(value);

    if (d->renderer2D_XY->isToggled()) {
        d->planeWidget[2]->SetSliceIndex(value);
        d->c_z = value;
    }

    if (d->renderer2D_XZ->isToggled()) {
        d->planeWidget[1]->SetSliceIndex(value);
        d->c_y = value;
    }

    if (d->renderer2D_YZ->isToggled()) {
        d->planeWidget[0]->SetSliceIndex(value);
        d->c_x = value;
    }

    d->GetInteractor()->Render();

    d->slice_slider->blockSignals(true);
    d->slice_slider->setValue(value);
    d->slice_slider->blockSignals(false);

    emit sliceChanged(value);
}

void gnomonViewVolumic::timeChange(int value)
{
    if (d->images_serie->time() == value)
        return;

    d->images_serie->setTime(value);
    if(d->images_serie->image())
        setImage(d->images_serie->image());

    d->time_slider->blockSignals(true);
    d->time_slider->setValue(value);
    d->time_slider->setToolTip(QString("current time: %1").arg(value));
    d->time_slider->blockSignals(false);

    emit timeChanged(value);
}

void gnomonViewVolumic::setImagesSerie(gnomonImagesSeriePtr images_serie, const QMap<double, QColor>& source)
{
    d->images_serie = images_serie;
    if(d->images_serie->image())
        setImage(d->images_serie->image(), source);
}

void gnomonViewVolumic::setBlending(bool blend)
{
    d->blending->toggle(blend);
    if(!blend) {
        if(d->images_serie && d->images_serie->image()) this->setImage(d->images_serie->image());
    }

    d->opacity->setVisible(blend);
    d->blending_list->setVisible(blend);
    d->blending_list->clear();
    d->blender->RemoveAllInputs();
}

void gnomonViewVolumic::setImage(dtkImage* i, const QMap<double, QColor>& source)
{
    // 2D

    dtkImageConverter *converter = dtkImaging::converter::pluginFactory().create("dtkVtkImageConverter");
    converter->setInput(i);
    converter->convert();

    vtkImageData *image = static_cast<vtkImageData *>(converter->output());

    delete converter;

    d->image_interactor->image = image;

    d->x = image->GetDimensions()[0];
    d->y = image->GetDimensions()[1];
    d->z = image->GetDimensions()[2];

    d->c_x = d->x/2;
    d->c_y = d->y/2;
    d->c_z = d->z/2;

    d->time_slider->setMaximum(d->images_serie->times().last());
    d->time_slider->blockSignals(true);
    d->time_slider->setValue(d->images_serie->time());
    d->time_slider->setToolTip(QString("current time: %1").arg(d->images_serie->time()));
    d->time_slider->blockSignals(false);

    // ///////////////////////////////////////////////////////////////////
    //
    // ///////////////////////////////////////////////////////////////////

    vtkSmartPointer<vtkColorTransferFunction> color_function = nullptr;
    vtkSmartPointer<vtkImageMapToColors> image_color =nullptr;
    double bounds[2];
    image->GetPointData()->GetScalars()->GetRange(bounds);
    color_function = vtkSmartPointer<vtkColorTransferFunction>::New();
    //color_function->RemoveAllPoints();

    for (const auto& val : source.keys()) {
        double node = val*bounds[1] + (1-val)*bounds[0];
        color_function->AddRGBPoint(node, source[val].red()/255.,  source[val].green()/255.,  source[val].blue()/255.);
    }

    color_function->ClampingOn();
    color_function->Modified();

    image_color = vtkSmartPointer<vtkImageMapToColors>::New();
    image_color->SetLookupTable(color_function);
    image_color->SetOutputFormatToRGBA();
    image_color->SetInputData(image);
    image_color->Update();

    if (d->blending->isToggled()) {
        QString label = QString("Layer %1").arg(d->blender->GetNumberOfInputs());

        d->blending_list->addItem(label);

        d->blender->AddInputData(image_color->GetOutput());

        d->blender->SetOpacity(0, 0.5);
        d->blender->SetOpacity(1, 0.5);
        d->blender->Update();

        d->viewer->SetInputData(d->blender->GetOutput());

    } else {
        d->viewer->SetInputData(image_color->GetOutput());
    }

    // ///////////////////////////////////////////////////////////////////

    // 3D

    int imageDims[3]; image->GetDimensions(imageDims);

    for(int i = 0; i < 3; i++) {
        d->planeWidget[i]->SetInputData(image);
        d->planeWidget[i]->SetSliceIndex(imageDims[i]/2);
        d->planeWidget[i]->DisplayTextOn();
        d->planeWidget[i]->InteractionOn();
    }

    if (d->renderer2D_XY->isToggled()) {
        d->planeWidget[0]->On();
        d->planeWidget[1]->On();
        d->planeWidget[2]->Off();

        d->viewer->SetSlice(d->z/2);
        d->slice_slider->setMaximum(d->z);
        d->slice_slider->blockSignals(true);
        d->slice_slider->setValue(d->z/2);
        d->slice_slider->blockSignals(false);
    }

    if (d->renderer2D_XZ->isToggled()) {
        d->planeWidget[0]->On();
        d->planeWidget[1]->Off();
        d->planeWidget[2]->On();

        d->viewer->SetSlice(d->y/2);
        d->slice_slider->setMaximum(d->y);
        d->slice_slider->blockSignals(true);
        d->slice_slider->setValue(d->y/2);
        d->slice_slider->blockSignals(false);
    }

    if (d->renderer2D_YZ->isToggled()) {
        d->planeWidget[0]->Off();
        d->planeWidget[1]->On();
        d->planeWidget[2]->On();

        d->viewer->SetSlice(d->x/2);
        d->slice_slider->setMaximum(d->x);
        d->slice_slider->blockSignals(true);
        d->slice_slider->setValue(d->x/2);
        d->slice_slider->blockSignals(false);
    }

    if(!d->volume_mapper)
        d->volume_mapper = vtkSmartPointer<vtkSmartVolumeMapper>::New();

    d->volume_mapper->SetInputData(image);
    d->volume_mapper->SetRequestedRenderMode(vtkSmartVolumeMapper::DefaultRenderMode);
    d->volume_mapper->Modified();
    d->volume_mapper->Update();

    if(!d->volume)
        d->volume = vtkSmartPointer<vtkVolume>::New();

    image->GetPointData()->GetScalars()->GetRange(bounds);
    vtkSmartPointer<vtkPiecewiseFunction> opacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
    opacity->AddPoint(   bounds[0],                0.00);
    opacity->AddPoint(1*(bounds[1]-bounds[0])/2/4, 0.00);
    opacity->AddPoint(   bounds[1],                1.00);

    vtkSmartPointer<vtkVolumeProperty> property = vtkSmartPointer<vtkVolumeProperty>::New();
    property->SetScalarOpacity(opacity);
    property->SetColor(color_function);
    property->ShadeOff();
    property->SetInterpolationType(VTK_LINEAR_INTERPOLATION);

    d->volume->SetMapper(d->volume_mapper);
    d->volume->SetProperty(property);
    d->volume->Modified();
    d->volume->Update();

    d->renderer3D->AddActor(d->volume);

    d->renderer2D->ResetCamera();
    d->renderer3D->ResetCamera();

    this->render();
}

dtkImage *gnomonViewVolumic::image(void)
{
    return d->images_serie->image();
}

gnomonImagesSeriePtr gnomonViewVolumic::imagesSerie(void)
{
    return d->images_serie;
}

std::vector<gnomonLandmark> gnomonViewVolumic::landmarks(void)
{
    std::vector<gnomonLandmark> landmarks;
    vtkActorCollection* actors_collection_2d =  d->renderer2D->GetActors();

    actors_collection_2d->InitTraversal();
    gnomonLandmark point;
    for(std::size_t i = 0; i < actors_collection_2d->GetNumberOfItems(); ++i) {
        gnomonLandmarkActor *landmark = dynamic_cast<gnomonLandmarkActor *>(actors_collection_2d->GetNextActor());
        if(!landmark) continue;


        landmark->GetPosition(point.pos);

        landmarks.push_back(point);
    }
    return landmarks;
}

vtkRenderWindowInteractor *gnomonViewVolumic::interactor(void)
{
    return d->GetInteractor();
}

vtkRenderer *gnomonViewVolumic::renderer2D(void)
{
    return d->renderer2D;
}

vtkRenderer *gnomonViewVolumic::renderer3D(void)
{
    return d->renderer3D;
}

void gnomonViewVolumic::render(void)
{
    d->GetInteractor()->Render();
}

void gnomonViewVolumic::onSliceChanged(int slice)
{
    d->slice_slider->setValue(slice);
}

void gnomonViewVolumic::onTimeChanged(int time)
{
    d->time_slider->setValue(time);
}

void gnomonViewVolumic::applyLut(const QMap<double, QColor>& source)
{
    if (!d->image_interactor->image)
        return;

    vtkSmartPointer<vtkColorTransferFunction> color_function = vtkSmartPointer<vtkColorTransferFunction>::New();

    double bounds[2];
    d->image_interactor->image->GetPointData()->GetScalars()->GetRange(bounds);

    for (const auto& val : source.keys()) {
        double node = val*bounds[1] + (1-val)*bounds[0];
        color_function->AddRGBPoint(node, source[val].red()/255.,  source[val].green()/255.,  source[val].blue()/255.);
    }

    color_function->ClampingOn();
    color_function->Modified();

    vtkSmartPointer<vtkImageMapToColors> image_color = vtkSmartPointer<vtkImageMapToColors>::New();
    image_color->SetLookupTable(color_function);
    image_color->SetOutputFormatToRGBA();
    image_color->SetInputData( d->image_interactor->image);
    image_color->Update();

    if(d->blending->isToggled()) {
        d->blender->ReplaceNthInputConnection(d->blender->GetNumberOfInputs()-1, image_color->GetOutputPort());
        d->blender->Update();
    }
    else {
        d->viewer->SetInputData(image_color->GetOutput());

        d->volume_mapper->SetInputData(d->image_interactor->image);
        d->image_interactor->image->GetPointData()->GetScalars()->GetRange(bounds);

        vtkSmartPointer<vtkPiecewiseFunction> opacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
        opacity->AddPoint(   bounds[0],                0.00);
        opacity->AddPoint(1*(bounds[1]-bounds[0])/8,   0.00);
        opacity->AddPoint(   bounds[1],                1.00);

        vtkSmartPointer<vtkVolumeProperty> property = vtkSmartPointer<vtkVolumeProperty>::New();
        property->SetScalarOpacity(opacity);
        property->SetColor(color_function);
        property->ShadeOff();
        property->SetInterpolationType(VTK_LINEAR_INTERPOLATION);

        d->volume->SetMapper(d->volume_mapper);
        d->volume->SetProperty(property);
        d->volume->Modified();
        d->volume->Update();
    }

    this->render();
}

void gnomonViewVolumic::onChannelChanged(const QString& channel,
                                         const QMap<double, QColor>& source)
{
    if(!d->images_serie) return;
    d->last_channel_toggled = channel;

    d->images_serie->setChannel(channel);
    dtkImage *img = d->images_serie->image();
    if (!img) {
        qWarning() << Q_FUNC_INFO << "Resulting image is void.";
        return;
    }

    this->setImage(img, source);
}

std::size_t gnomonViewVolumic::addLandmark(std::size_t id, double x, double y, double z)
{
    Q_ASSERT(id == d->image_interactor->landmark_id);

    Q_ASSERT(QObject::sender() != this);

    dtkImage *img = this->image();
    if(!img) return 0;

    dtkArray<double> spacing = img->spacing();
    double x_length = spacing[0] * double(img->xDim());
    double y_length = spacing[1] * img->yDim();
    double z_length = spacing[2] * img->zDim();

    double radius = std::sqrt(x_length * x_length + y_length * y_length + z_length * z_length) * 0.5 * 0.02;

    vtkSmartPointer<vtkSphereSource> sphere_source =
        vtkSmartPointer<vtkSphereSource>::New();
    sphere_source->SetRadius(radius);

    vtkSmartPointer<vtkPolyDataMapper> mapper =
        vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(sphere_source->GetOutputPort());

    vtkSmartPointer<gnomonLandmarkActor> actor =
        vtkSmartPointer<gnomonLandmarkActor>::New();
    actor->setId(id);
    actor->SetMapper(mapper);
    actor->SetPosition(x, y, z);

    double a = double(id % 51) / 50.;
    double b = double(50 - id % 51) / 50.;
    actor->GetProperty()->SetColor((id % 2 != 0) ? a : b, (id % 3 == 0) ? a : b, (id % 4 == 0) ? a : b);

    d->renderer2D->AddActor(actor);
    d->renderer3D->AddActor(actor);

    d->GetInteractor()->Render();

    ++d->image_interactor->landmark_id;

    return id;
}

void gnomonViewVolumic::removeLandmark(std::size_t id)
{
    vtkActorCollection* actors_collection_2d =  d->renderer2D->GetActors();
    vtkActorCollection* actors_collection_3d =  d->renderer3D->GetActors();

    actors_collection_2d->InitTraversal();
    for(std::size_t i = 0; i < actors_collection_2d->GetNumberOfItems(); ++i) {
        gnomonLandmarkActor *landmark = dynamic_cast<gnomonLandmarkActor *>(actors_collection_2d->GetNextActor());
        if(!landmark) continue;

        if(landmark->id() == id) {
            d->renderer2D->RemoveActor(landmark);
            break;
        }
    }

    actors_collection_3d->InitTraversal();
    for(std::size_t i = 0; i < actors_collection_3d->GetNumberOfItems(); ++i) {
        gnomonLandmarkActor *landmark = dynamic_cast<gnomonLandmarkActor *>(actors_collection_3d->GetNextActor());
        if(!landmark) continue;

        if(landmark->id() == id) {
            d->renderer3D->RemoveActor(landmark);
            break;
        }
    }

    d->GetInteractor()->Render();
}

void gnomonViewVolumic::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasText()) {
        event->accept();
        return;
    }

    event->ignore();
}

void gnomonViewVolumic::dragLeaveEvent(QDragLeaveEvent *event)
{
    event->accept();
}

void gnomonViewVolumic::dragMoveEvent(QDragMoveEvent *event)
{
    event->accept();
}

void gnomonViewVolumic::dropEvent(QDropEvent *event)
{
    QString path = event->mimeData()->text();

    if(path.startsWith(":")) {
        gnomonImagesSeriePtr images_serie = gnomonImageManager::instance()->get(path.remove(":").toInt());
        emit channelsChanged(images_serie->channels());
        this->setImagesSerie(images_serie);
    } else {
        gnomonImagesSerieReaderCommand * command = new gnomonImagesSerieReaderCommand("gnomonImagesSerieReader");

        if (command) {
            command->setPath(path.remove("file://"));
            command->redo();
            gnomonImagesSeriePtr images_serie = gnomonImagesSeriePtr(command->imagesSerie());
            emit channelsChanged(images_serie->channels());
            emit timeChanged(images_serie->time());
            this->setImagesSerie(images_serie);
        } else {
            qWarning() << Q_FUNC_INFO << "No reader founds for input: " << path;
        }

        delete command;
    }

    for(auto& layer : d->layers) {
        delete layer;
    }
    d->layers.clear();

    QStringList layer_names = d->images_serie->channels();

    std::size_t i = 0;
    for(const QString& layer : layer_names) {
        gnomonViewVolumicOverlay *layer_overlay = new gnomonViewVolumicOverlay(fa::eye, layer, this);
        layer_overlay->move(d->size().width() - layer_overlay->width() + 15, 80 + i * 25);
        if(i == 0) {
            layer_overlay->toggle(true);
            layer_overlay->activate(true);
        } else {
            layer_overlay->toggle(false);
        }
        layer_overlay->setVisible(d->stack->isToggled());
        connect(layer_overlay, &gnomonViewVolumicOverlay::iconClicked, [=] () {
                qDebug() << "iconClicked";
                d->toggleChannel(layer_overlay->text());
            });
        connect(layer_overlay, &gnomonViewVolumicOverlay::textClicked, [=] () {
                d->activateChannel(layer_overlay->text());
            });
        d->layers << layer_overlay;
        ++i;
    }

    // ///////////////////////////////////////////////////////////////

    event->accept();
}
void gnomonViewVolumicPrivate::activateChannel(const QString& channel_to_activate)
{
    for(auto& layer : this->layers) {
        if(layer->text() != channel_to_activate) {
            layer->activate(false);
            layer->toggle(false);
        } else {
            layer->activate(true);
            layer->toggle(false);
        }
    }
    this->toggleChannel(channel_to_activate);
}

void gnomonViewVolumicPrivate::toggleChannel(const QString& channel_to_toggle)
{

    //count the number of channels toggled
    std::size_t nb_layers_toggled = 0;
    QString activated_layer;
    for(auto& layer : this->layers) {
        if(layer->text() == channel_to_toggle) layer->toggle(!layer->isToggled());
        if(layer->isToggled()) ++nb_layers_toggled;
        if(layer->isActivated()) activated_layer = layer->text();
    }

    Q_ASSERT(activated_layer != "");

    switch(nb_layers_toggled) {

    case 0:
        // q->setBlending(false);
        q->onChannelChanged(activated_layer, this->channels_lut[activated_layer]);
        break;

    case 1:
        q->setBlending(false);
        q->onChannelChanged(activated_layer, this->channels_lut[activated_layer]);
        break;

    default:
        q->setBlending(true);
        std::size_t i = 0;
        for(auto& layer : this->layers) {
            if(layer->isToggled()) {
                q->onChannelChanged(activated_layer, this->channels_lut[activated_layer]);
            }
        }
    }

}

// ///////////////////////////////////////////////////////////////////

#include "gnomonViewVolumic.moc"

//
// gnomonViewVolumic.cpp ends here
