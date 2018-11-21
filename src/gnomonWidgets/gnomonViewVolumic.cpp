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

#include <gnomonCore/gnomonImagesSerieReaderCommand.h>

#include <gnomonStyle>
#include <gnomonFonts>

#include <dtkImagingCore>

#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkCellPicker.h>
#include <vtkColorTransferFunction.h>
#include <vtkContourFilter.h>
#include <vtkDataArray.h>
#include <vtkDataSetMapper.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkGlyph3D.h>
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
#include <vtkResliceImageViewer.h>
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
// gnomonViewVolumicOverlay
// ///////////////////////////////////////////////////////////////////

class gnomonViewVolumicOverlay : public QLabel
{
    Q_OBJECT

public:
     gnomonViewVolumicOverlay(fa::icon, QWidget *parent = nullptr);
     gnomonViewVolumicOverlay(const QString& path, QWidget *parent = nullptr);
    ~gnomonViewVolumicOverlay(void);

    void changeColor(const QColor&);
    void changePath(const QString&);

    QColor defaultColor(void);

signals:
    void clicked(void);

protected:
    void mousePressEvent(QMouseEvent *);

public:
    bool on = false;

public:
    gnomonFontAwesome *font;
    fa::icon           icon;
    QColor             default_color;
};

gnomonViewVolumicOverlay::gnomonViewVolumicOverlay(fa::icon icon, QWidget *parent) : QLabel(parent)
{
    this->default_color = QColor("#ffffff");

    this->icon = icon;

    this->font = new gnomonFontAwesome(this);
    this->font->initFontAwesome();
    this->font->setDefaultOption("color", this->default_color);

    this->setPixmap(this->font->icon(icon).pixmap(24, 24));

    this->setStyleSheet("background: none;");
}

gnomonViewVolumicOverlay::gnomonViewVolumicOverlay(const QString& path, QWidget *parent) : QLabel(parent)
{
    this->setPixmap(QPixmap(path));

    this->setStyleSheet("background: none;");
}

gnomonViewVolumicOverlay::~gnomonViewVolumicOverlay(void)
{

}

void gnomonViewVolumicOverlay::mousePressEvent(QMouseEvent *)
{
    emit clicked();
}


void gnomonViewVolumicOverlay::changeColor(const QColor& color)
{
    this->font->setDefaultOption("color", color);

    this->setPixmap(this->font->icon(this->icon).pixmap(24, 24));
}

void gnomonViewVolumicOverlay::changePath(const QString& path)
{
    this->setPixmap(QPixmap(path));
}

QColor gnomonViewVolumicOverlay::defaultColor(void)
{
    return this->default_color;
}

// ///////////////////////////////////////////////////////////////////
// gnomonViewVolumicInteractorImage
// ///////////////////////////////////////////////////////////////////

class gnomonViewVolumicInteractorImage : public vtkInteractorStyleImage
{
public:
    static gnomonViewVolumicInteractorImage *New(void);

public:
    virtual void OnLeftButtonDown(void) override
    {
        vtkInteractorStyleImage::OnLeftButtonDown();

        if(!this->picker)
            return;

        if(!this->picker->on)
            return;

        if(!this->image)
            return;

        int *pos = this->GetInteractor()->GetEventPosition();

        vtkSmartPointer<vtkCellPicker> picker = vtkSmartPointer<vtkCellPicker>::New();
        picker->SetTolerance(0.0005);
        picker->Pick(pos[0], pos[1], 0, this->GetDefaultRenderer());

        if(picker->GetCellId() == -1)
            return;

        double *picked = picker->GetPickPosition();

        this->points->InsertNextPoint(picked[0], picked[1], picked[2]);

        this->mesh->SetPoints(this->points);
        this->mesh->Modified();

        this->glyphs->SetInputData(this->mesh);
        this->glyphs->Update();

        this->q->render();
    }

public:
    gnomonViewVolumic *q = nullptr;
    gnomonViewVolumicOverlay *picker = nullptr;

public:
    vtkSmartPointer<vtkImageData> image = nullptr;
    vtkSmartPointer<vtkPoints> points = nullptr;
    vtkSmartPointer<vtkPolyData> mesh = nullptr;
    vtkSmartPointer<vtkGlyph3D> glyphs = nullptr;
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
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> window;
    vtkSmartPointer<vtkRenderer> renderer2D;
    vtkSmartPointer<vtkRenderer> renderer3D;

public:
    gnomonViewVolumic *q = nullptr;

public:
    vtkSmartPointer<vtkImagePlaneWidget> planeWidget[3];

public:
    vtkSmartPointer<vtkResliceImageViewer> viewer = nullptr;
    vtkSmartPointer<vtkVolume> volume = nullptr;
    vtkSmartPointer<vtkSmartVolumeMapper> volume_mapper = nullptr;
    vtkSmartPointer<vtkDataSetMapper> blender_mapper = nullptr;
    vtkSmartPointer<vtkActor> blender_actor = nullptr;

public:
    vtkSmartPointer<vtkPoints> points;
    vtkSmartPointer<vtkPolyData> mesh;
    vtkSmartPointer<vtkGlyph3D> glyphs;

public:
    vtkSmartPointer<vtkImageBlend> blender = nullptr;

public:
    gnomonViewVolumicInteractorImage *image_interactor = nullptr;

public:
    gnomonImagesSerieReaderCommand *image_reader_command_inr = nullptr;
    gnomonImagesSerieReaderCommand *image_reader_command_czi = nullptr;

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

public:
    gnomonViewVolumicList *blending_list = nullptr;

public:
    int syncing_count = 0;
    QTimer *syncing_timer = nullptr;
    bool synced = false;

public:
    dtkImagePtr image;

public:
    QSlider *slider;

public:
    QSlider *opacity;

public:
    int x = 0, c_x = 0;
    int y = 0, c_y = 0;
    int z = 0, c_z = 0;
    double lut_hue_min = 0., lut_hue_max = 0.5;
    double lut_sat_min = 1., lut_sat_max = 1.;
    double lut_val_min = 0., lut_val_max = 1.;
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

    this->export_button = new gnomonViewVolumicOverlay(fa::arrowcircleup, this);
    this->renderer2D_button = new gnomonViewVolumicOverlay(fa::square, this);
    this->renderer3D_button = new gnomonViewVolumicOverlay(fa::cube, this);
    this->renderer3D_button->changeColor(Qt::gray);
    this->renderer2D_XY = new gnomonViewVolumicOverlay(":gnomon/gnomonViewVolumic-XY.png", this);
    this->renderer2D_XY->on = true;
    this->renderer2D_XZ = new gnomonViewVolumicOverlay(":gnomon/gnomonViewVolumic-XZ-off.png", this);
    this->renderer2D_XZ->on = false;
    this->renderer2D_YZ = new gnomonViewVolumicOverlay(":gnomon/gnomonViewVolumic-YZ-off.png", this);
    this->renderer2D_YZ->on = false;
    this->picker = new gnomonViewVolumicOverlay(fa::crosshairs, this);
    this->picker->changeColor(Qt::gray);
    this->picker->on = false;
    this->blending = new gnomonViewVolumicOverlay(fa::adjust, this);
    this->blending->changeColor(Qt::gray);
    this->blending->on = false;
    this->sync = new gnomonViewVolumicOverlay(fa::unlock, this);
    this->sync->changeColor(Qt::gray);
    this->sync->on = false;

    this->blending_list = new gnomonViewVolumicList(this);
    this->blending_list->resize(200, 100);
    this->blending_list->setVisible(false);

    vtkImageData *dummy = vtkImageData::New();
    dummy->SetDimensions(1, 1, 1);
    dummy->SetSpacing(1, 1, 1);
#if VTK_MAJOR_VERSION <= 5
    dummy->SetNumberOfScalarComponents(1);
    dummy->SetScalarTypeToUnsignedChar();
#else
    dummy->AllocateScalars(VTK_UNSIGNED_CHAR,1);
#endif

    this->blender = vtkSmartPointer<vtkImageBlend>::New();

    //this->blender_mapper = vtkSmartPointer<vtkDataSetMapper>::New();
    //this->blender_mapper->SetInputConnection(this->blender->GetOutputPort());

    //this->blender_actor =  vtkSmartPointer<vtkActor>::New();
    //this->blender_actor->SetMapper(this->blender_mapper);
    //this->renderer2D->AddActor(this->blender_actor);

    this->viewer = vtkSmartPointer<vtkResliceImageViewer>::New();
    this->viewer->SetSliceOrientationToXY();
    this->viewer->SetRenderWindow(this->window);
    this->viewer->SetRenderer(this->renderer2D);
    this->viewer->SetupInteractor(this->GetInteractor());
    this->viewer->SetResliceModeToAxisAligned();
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
    }

    this->points = vtkSmartPointer<vtkPoints>::New();
    this->points->Allocate(100);

    this->mesh = vtkSmartPointer<vtkPolyData>::New();

    vtkSmartPointer<vtkSphereSource> sphere_source = vtkSmartPointer<vtkSphereSource>::New();
    sphere_source->SetRadius(1.0);
    sphere_source->SetPhiResolution(16);
    sphere_source->SetThetaResolution(16);
    sphere_source->Update();

    this->glyphs = vtkSmartPointer<vtkGlyph3D>::New();
    this->glyphs->SetSourceData(sphere_source->GetOutput());
    this->glyphs->SetInputData(this->mesh);

    vtkSmartPointer<vtkPolyDataMapper> glyph_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    glyph_mapper->SetInputConnection(this->glyphs->GetOutputPort());

    vtkSmartPointer<vtkActor> glyph_actor = vtkSmartPointer<vtkActor>::New();
    glyph_actor->SetMapper(glyph_mapper);
    glyph_actor->GetProperty()->SetColor(1.0, 0.0, 0.5);

    this->renderer2D->AddActor(glyph_actor);
    this->renderer3D->AddActor(glyph_actor);

    // ///////////////////////////////////////////////////////////////////

    this->image_interactor = gnomonViewVolumicInteractorImage::New();
    this->image_interactor->SetDefaultRenderer(this->renderer2D);
    this->image_interactor->picker = this->picker;
    this->image_interactor->points = this->points;
    this->image_interactor->mesh = this->mesh;
    this->image_interactor->glyphs = this->glyphs;

    this->GetInteractor()->SetInteractorStyle(this->image_interactor);

    // ///////////////////////////////////////////////////////////////////

    connect(this->export_button, SIGNAL(clicked()), this, SLOT(exportToManager()));

    connect(this->renderer2D_button, &gnomonViewVolumicOverlay::clicked, [this] () {

        this->renderer2D_button->setEnabled(false);
        this->renderer2D_button->changeColor(Qt::white);
        this->renderer3D_button->setEnabled(true);
        this->renderer3D_button->changeColor(Qt::gray);

        this->renderer2D_XY->setVisible(true);
        this->renderer2D_XZ->setVisible(true);
        this->renderer2D_YZ->setVisible(true);

        this->renderer3D->DrawOff();
        this->renderer3D->InteractiveOff();

        this->GetInteractor()->SetInteractorStyle(this->image_interactor);

        this->renderer2D->InteractiveOn();
        this->renderer2D->DrawOn();

        this->slider->setEnabled(true);

        this->planeWidget[0]->Off();
        this->planeWidget[1]->Off();
        this->planeWidget[2]->Off();

        if (this->renderer2D_XY->on) {
            this->viewer->SetSlice(this->c_z);
            this->planeWidget[0]->On();
            this->planeWidget[1]->On();
            this->planeWidget[2]->Off();
        }

        if (this->renderer2D_XZ->on) {
            this->viewer->SetSlice(this->c_y);
            this->planeWidget[0]->On();
            this->planeWidget[1]->Off();
            this->planeWidget[2]->On();
        }

        if (this->renderer2D_YZ->on) {
            this->viewer->SetSlice(this->c_x);
            this->planeWidget[0]->Off();
            this->planeWidget[1]->On();
            this->planeWidget[2]->On();
        }

        if (q)
            q->render();
    });

    connect(this->renderer3D_button, &gnomonViewVolumicOverlay::clicked, [this] () {

        this->renderer2D_button->setEnabled(true);
        this->renderer2D_button->changeColor(Qt::gray);
        this->renderer3D_button->setEnabled(false);
        this->renderer3D_button->changeColor(Qt::white);

        this->renderer2D_XY->setVisible(false);
        this->renderer2D_XZ->setVisible(false);
        this->renderer2D_YZ->setVisible(false);

        this->renderer2D->DrawOff();
        this->renderer2D->InteractiveOff();

        vtkSmartPointer<vtkInteractorStyleTrackballCamera> style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();

        this->GetInteractor()->SetInteractorStyle(style);

        this->renderer3D->InteractiveOn();
        this->renderer3D->DrawOn();

        this->slider->setEnabled(false);

        this->planeWidget[0]->Off();
        this->planeWidget[1]->Off();
        this->planeWidget[2]->Off();

        this->planeWidget[0]->On();
        this->planeWidget[1]->On();
        this->planeWidget[2]->On();

        if (q)
            q->render();
    });

    connect(this->picker, &gnomonViewVolumicOverlay::clicked, [=] () {

        this->picker->on = !this->picker->on;

        if (this->picker->on)
            this->picker->changeColor(Qt::white);
        else
            this->picker->changeColor(Qt::gray);

        q->setCursor(this->picker->on ? Qt::CrossCursor : Qt::ArrowCursor);
    });

    connect(this->blending, &gnomonViewVolumicOverlay::clicked, [=] () {

        this->blending->on = !this->blending->on;

        if (this->blending->on)
            this->blending->changeColor(Qt::white);
        else
            this->blending->changeColor(Qt::gray);

        this->opacity->setVisible(this->blending->on);
        this->blending_list->setVisible(this->blending->on);

        if(!this->blending->on) {
            this->blending_list->clear();

            if (this->image)
                q->setImage(this->image);

            this->blender->RemoveAllInputs();
        }
    });

    connect(this->sync, &gnomonViewVolumicOverlay::clicked, [=] () {

        this->sync->on = !this->sync->on;

        if (this->sync->on)
            emit q->linking();
        else
            emit q->unlinking();

        if (this->sync->on)
            this->sync->changeColor(Qt::white);
        else
            this->sync->changeColor(Qt::gray);

        if (this->sync->on && !this->synced) {
            this->syncing_count = 0;
            if(!this->syncing_timer)
                this->syncing_timer = new QTimer(this);
            connect(this->syncing_timer, &QTimer::timeout, [=] () {
                this->sync->changeColor(this->syncing_count++ % 2 ? Qt::white : Qt::gray);
                this->sync->update();
                if (this->syncing_count == 11) {
                    this->sync->on = false;
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
    if(!this->image)
        return;

    QWidget *parent = this->parentWidget();

    while(!dynamic_cast< QStackedWidget *>(parent))
        parent = parent->parentWidget();

    QStackedWidget *stack = dynamic_cast< QStackedWidget * >(parent);

    if(gnomonWorkspaceBrowser* workspace = dynamic_cast<gnomonWorkspaceBrowser *>(stack->currentWidget()))
        gnomonImageManager::instance()->addImage(this->image, gnomonToolBar::browser_color);
    else if(gnomonWorkspaceFusion* workspace = dynamic_cast<gnomonWorkspaceFusion *>(stack->currentWidget()))
        gnomonImageManager::instance()->addImage(this->image, gnomonToolBar::fusion_color);
    else if(gnomonWorkspaceSegmentation* workspace = dynamic_cast<gnomonWorkspaceSegmentation *>(stack->currentWidget()))
        gnomonImageManager::instance()->addImage(this->image, gnomonToolBar::segmentation_color);
    else if(gnomonWorkspacePreprocess* workspace = dynamic_cast<gnomonWorkspacePreprocess *>(stack->currentWidget()))
        gnomonImageManager::instance()->addImage(this->image, gnomonToolBar::preprocess_color);
    else
        gnomonImageManager::instance()->addImage(this->image, gnomonToolBar::registration_color);
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
    this->opacity->move(event->size().width() - 200 + 5, event->size().height() - 100 - 10 - 30);
    this->blending_list->move(event->size().width() - 200 - 10, event->size().height() - 100 - 10);

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
        break;
    case SLICE_ORIENTATION_XZ:
        this->viewer->SetSliceOrientation(vtkImageViewer2::SLICE_ORIENTATION_XZ);
        break;
    case SLICE_ORIENTATION_XY:
        this->viewer->SetSliceOrientation(vtkImageViewer2::SLICE_ORIENTATION_XY);
        break;
    default:
        break;
    };
}

// ///////////////////////////////////////////////////////////////////
// gnomonViewVolumic
// ///////////////////////////////////////////////////////////////////

gnomonViewVolumic::gnomonViewVolumic(QWidget *parent) : QFrame(parent)
{
    d = new gnomonViewVolumicPrivate;
    d->q = this;

    connect(d->renderer2D_XY, SIGNAL(clicked()), this, SLOT(switchTo2DXY()));
    connect(d->renderer2D_XZ, SIGNAL(clicked()), this, SLOT(switchTo2DXZ()));
    connect(d->renderer2D_YZ, SIGNAL(clicked()), this, SLOT(switchTo2DYZ()));

    d->image_interactor->q = this;

    d->slider = new QSlider(this);
    d->slider->setObjectName("prout");
    d->slider->setOrientation(Qt::Vertical);
    d->slider->setMinimum(0);
    d->slider->setMaximum(1);
    d->slider->setValue(0);

    connect(d->slider, SIGNAL(valueChanged(int)), this, SLOT(sliceChange(int)));

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(d->slider);
    layout->addWidget(d);

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
    if (d->image_reader_command_inr)
        delete d->image_reader_command_inr;

    if (d->image_reader_command_czi)
        delete d->image_reader_command_czi;

    delete d;
}

void gnomonViewVolumic::link(gnomonViewVolumic *other)
{
    if (d->syncing_timer)
        d->syncing_timer->stop();

    d->sync->on = true;
    d->sync->icon = fa::lock;
    d->sync->changeColor(Qt::white);

    d->synced = true;

    // ///////////////////////////////////////////////////////////////

    d->renderer2D->SetActiveCamera(other->d->renderer2D->GetActiveCamera());
    d->renderer3D->SetActiveCamera(other->d->renderer3D->GetActiveCamera());

    other->d->GetRenderWindow()->AddObserver(vtkCommand::RenderEvent, this, &gnomonViewVolumic::render);

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

    d->sync->on = false;
    d->sync->icon = fa::unlock;
    d->sync->changeColor(Qt::gray);

    d->synced = false;

    // ///////////////////////////////////////////////////////////////

    vtkSmartPointer<vtkCamera> camera2D = vtkCamera::New();
    camera2D->ShallowCopy(d->renderer2D->GetActiveCamera());

    vtkSmartPointer<vtkCamera> camera3D = vtkCamera::New();
    camera3D->ShallowCopy(d->renderer2D->GetActiveCamera());

    d->renderer2D->SetActiveCamera(camera2D);
    d->renderer3D->SetActiveCamera(camera3D);

    // ///////////////////////////////////////////////////////////////

    disconnect(other, SIGNAL(switchedTo2DXY()), this, SLOT(switchTo2DXY()));
    disconnect(other, SIGNAL(switchedTo2DXZ()), this, SLOT(switchTo2DXZ()));
    disconnect(other, SIGNAL(switchedTo2DYZ()), this, SLOT(switchTo2DYZ()));

    disconnect(other, SIGNAL(sliceChanged(int)), this, SLOT(sliceChange(int)));
}

void gnomonViewVolumic::switchTo2DXY(void)
{
    if (d->renderer2D_XY->on)
        return;

    d->renderer2D_XY->changePath(":gnomon/gnomonViewVolumic-XY.png");
    d->renderer2D_XZ->changePath(":gnomon/gnomonViewVolumic-XZ-off.png");
    d->renderer2D_YZ->changePath(":gnomon/gnomonViewVolumic-YZ-off.png");

    d->renderer2D_XY->on = true;
    d->renderer2D_XZ->on = false;
    d->renderer2D_YZ->on = false;

    d->setSliceOrientation(gnomonViewVolumicPrivate::SLICE_ORIENTATION_XY);

    d->viewer->SetSlice(d->c_z);

    d->planeWidget[0]->On();
    d->planeWidget[1]->On();
    d->planeWidget[2]->Off();

    d->slider->blockSignals(true);
    d->slider->setMaximum(d->z);
    d->slider->setValue(d->c_z);
    d->slider->blockSignals(false);

    emit switchedTo2DXY();
}

void gnomonViewVolumic::switchTo2DXZ(void)
{
    if (d->renderer2D_XZ->on)
        return;

    d->renderer2D_XY->changePath(":gnomon/gnomonViewVolumic-XY-off.png");
    d->renderer2D_XZ->changePath(":gnomon/gnomonViewVolumic-XZ.png");
    d->renderer2D_YZ->changePath(":gnomon/gnomonViewVolumic-YZ-off.png");

    d->renderer2D_XY->on = false;
    d->renderer2D_XZ->on = true;
    d->renderer2D_YZ->on = false;

    d->setSliceOrientation(gnomonViewVolumicPrivate::SLICE_ORIENTATION_XZ);

    d->viewer->SetSlice(d->c_y);

    d->planeWidget[0]->On();
    d->planeWidget[1]->Off();
    d->planeWidget[2]->On();

    d->slider->blockSignals(true);
    d->slider->setMaximum(d->y);
    d->slider->setValue(d->c_y);
    d->slider->blockSignals(false);

    emit switchedTo2DXZ();
}

void gnomonViewVolumic::switchTo2DYZ(void)
{
    if (d->renderer2D_YZ->on)
        return;

    d->renderer2D_XY->changePath(":gnomon/gnomonViewVolumic-XY-off.png");
    d->renderer2D_XZ->changePath(":gnomon/gnomonViewVolumic-XZ-off.png");
    d->renderer2D_YZ->changePath(":gnomon/gnomonViewVolumic-YZ.png");

    d->renderer2D_XY->on = false;
    d->renderer2D_XZ->on = false;
    d->renderer2D_YZ->on = true;

    d->setSliceOrientation(gnomonViewVolumicPrivate::SLICE_ORIENTATION_YZ);

    d->viewer->SetSlice(d->c_x);

    d->planeWidget[0]->Off();
    d->planeWidget[1]->On();
    d->planeWidget[2]->On();

    d->slider->blockSignals(true);
    d->slider->setMaximum(d->x);
    d->slider->setValue(d->c_x);
    d->slider->blockSignals(false);

    emit switchedTo2DYZ();
}

void gnomonViewVolumic::sliceChange(int value)
{
    d->viewer->SetSlice(value);

    if (d->renderer2D_XY->on) {
        d->planeWidget[2]->SetSliceIndex(value);
        d->c_z = value;
    }

    if (d->renderer2D_XZ->on) {
        d->planeWidget[1]->SetSliceIndex(value);
        d->c_y = value;
    }

    if (d->renderer2D_YZ->on) {
        d->planeWidget[0]->SetSliceIndex(value);
        d->c_x = value;
    }

    d->GetInteractor()->Render();

    // emit sliceChanged(value);
}

void gnomonViewVolumic::setImage(dtkImagePtr i)
{
    d->points->Reset();

    d->mesh->SetPoints(d->points);
    d->mesh->Modified();

    d->glyphs->SetInputData(d->mesh);
    d->glyphs->Update();

    d->image = i;

    // 2D

    dtkImageConverter *converter = dtkImaging::converter::pluginFactory().create("dtkVtkImageConverter");
    converter->setInput(i.data());
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

    // ///////////////////////////////////////////////////////////////////
    //
    // ///////////////////////////////////////////////////////////////////

    double bounds[2];
    image->GetPointData()->GetScalars()->GetRange(bounds);

    vtkSmartPointer<vtkLookupTable> hueLut = vtkSmartPointer<vtkLookupTable>::New();
    hueLut->SetTableRange (bounds);
    hueLut->SetHueRange (d->lut_hue_min, d->lut_hue_max);
    hueLut->SetSaturationRange (d->lut_sat_min, d->lut_sat_max);
    hueLut->SetValueRange (d->lut_val_min, d->lut_val_max);
    hueLut->Build(); //effective built

    vtkSmartPointer<vtkImageMapToColors> image_color = vtkSmartPointer<vtkImageMapToColors>::New();
    image_color->SetLookupTable(hueLut);
    image_color->SetInputData(image);
    image_color->SetOutputFormatToRGB();
    image_color->Update();

    double bb[2];
    qDebug() << "img bounds" << bounds[0] << bounds[1];
    image_color->GetOutput()->GetPointData()->GetScalars()->GetRange(bb);
    qDebug() << "img bounds" << bb[0] << bb[1];

    if (d->blending->on) {

        QString label = QString("Layer %1").arg(d->blender->GetNumberOfInputs());

        d->blending_list->addItem(label);

        vtkSmartPointer<vtkImageCast> caster = vtkSmartPointer<vtkImageCast>::New();
        //caster->SetInputData(image);
        caster->SetInputData(image_color->GetOutput());
        caster->SetOutputScalarTypeToUnsignedShort();
        caster->Update();

        double bounds_temp[2];
        caster->GetOutput()->GetPointData()->GetScalars()->GetRange(bounds_temp);
        qDebug() << "caster bounds" << bounds_temp[0] << bounds_temp[1];

        d->blender->AddInputData(caster->GetOutput());

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

    if (d->renderer2D_XY->on) {
        d->planeWidget[0]->On();
        d->planeWidget[1]->On();
        d->planeWidget[2]->Off();

        d->viewer->SetSlice(d->z/2);
        d->slider->setMaximum(d->z);
        d->slider->blockSignals(true);
        d->slider->setValue(d->z/2);
        d->slider->blockSignals(false);
    }

    if (d->renderer2D_XZ->on) {
        d->planeWidget[0]->On();
        d->planeWidget[1]->Off();
        d->planeWidget[2]->On();

        d->viewer->SetSlice(d->y/2);
        d->slider->setMaximum(d->y);
        d->slider->blockSignals(true);
        d->slider->setValue(d->y/2);
        d->slider->blockSignals(false);
    }

    if (d->renderer2D_YZ->on) {
        d->planeWidget[0]->Off();
        d->planeWidget[1]->On();
        d->planeWidget[2]->On();

        d->viewer->SetSlice(d->x/2);
        d->slider->setMaximum(d->x);
        d->slider->blockSignals(true);
        d->slider->setValue(d->x/2);
        d->slider->blockSignals(false);
    }

    if(!d->volume_mapper)
        d->volume_mapper = vtkSmartPointer<vtkSmartVolumeMapper>::New();

    d->volume_mapper->SetInputData(image);
    d->volume_mapper->SetRequestedRenderMode(vtkSmartVolumeMapper::DefaultRenderMode);
    d->volume_mapper->Modified();
    d->volume_mapper->Update();

    if(!d->volume)
        d->volume = vtkSmartPointer<vtkVolume>::New();

    vtkSmartPointer<vtkPiecewiseFunction> opacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
    opacity->AddPoint(   bounds[0],                0.00);
    opacity->AddPoint(1*(bounds[1]-bounds[0])/2/4, 0.00);
    opacity->AddPoint(   bounds[1],                1.00);

    vtkSmartPointer<vtkVolumeProperty> property = vtkSmartPointer<vtkVolumeProperty>::New();
    property->SetScalarOpacity(opacity);
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

dtkImagePtr gnomonViewVolumic::image(void)
{
    return d->image;
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
    d->slider->setValue(d->slider->value()+1);
    d->slider->setValue(d->slider->value()-1);
    d->GetInteractor()->Render();
}

void gnomonViewVolumic::onSliceChanged(int slice)
{
    d->slider->setValue(slice);
}

void gnomonViewVolumic::applyLut(double lut_hue_min, double lut_hue_max,
                                 double lut_sat_min, double lut_sat_max,
                                 double lut_val_min, double lut_val_max)
{
    d->lut_hue_min = lut_hue_min;
    d->lut_hue_max = lut_hue_max;
    d->lut_sat_min = lut_sat_min;
    d->lut_sat_max = lut_sat_max;
    d->lut_val_min = lut_val_min;
    d->lut_val_max = lut_val_max;

    double bounds[2];
    d->image_interactor->image->GetPointData()->GetScalars()->GetRange(bounds);

    vtkSmartPointer<vtkLookupTable> hueLut = vtkSmartPointer<vtkLookupTable>::New();
    hueLut->SetTableRange (bounds[0], bounds[1]);
    hueLut->SetHueRange (d->lut_hue_min, d->lut_hue_max);
    hueLut->SetSaturationRange (d->lut_sat_min, d->lut_sat_max);
    hueLut->SetValueRange (d->lut_val_min, d->lut_val_max);
    hueLut->Build(); //effective built

    if(d->blending->on) {
        //TODO
        qDebug() << Q_FUNC_INFO << "TODO";
    }
    else {
        vtkSmartPointer<vtkImageMapToColors> image_color = vtkSmartPointer<vtkImageMapToColors>::New();
        image_color->SetLookupTable(hueLut);
        image_color->SetInputData(d->image_interactor->image);
        image_color->SetOutputFormatToRGB();
        image_color->Update();
        d->viewer->SetInputData(image_color->GetOutput());
    }
    this->render();
}

void gnomonViewVolumic::onChannelChanged(const QString& channel)
{
    if(!d->image_reader_command_czi) {
        return;
    }
    dtkImage *img = d->image_reader_command_czi->at(d->image_reader_command_czi->time(), channel);
    if (!img) {
        qWarning() << Q_FUNC_INFO << "Resulting image is void.";
        return;
    }

    this->setImage(dtkImagePtr(new dtkImage(*img)));
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
        this->setImage(gnomonImageManager::instance()->get(path.remove(":").toInt()));

    } else {
        gnomonImagesSerieReaderCommand *command = nullptr;

        if(path.endsWith("inr") || path.endsWith("inr.gz") || path.endsWith("mha") || path.endsWith("tif")) {
            if(!d->image_reader_command_inr)
                d->image_reader_command_inr = new gnomonImagesSerieReaderCommand("gnomonImagesSerieReader");
            command = d->image_reader_command_inr;
        }

        if(path.endsWith("czi")) {
            if(!d->image_reader_command_czi)
                d->image_reader_command_czi = new gnomonImagesSerieReaderCommand("gnomonCziImageReader");
            command = d->image_reader_command_czi;
        }

        if(command) {
            command->setPath(path.remove("file://"));
            command->redo();
            dtkImagePtr img = dtkImagePtr(new dtkImage(*command->next()));

            if (!img) {
                qWarning() << Q_FUNC_INFO << "Resulting image is void.";
                event->ignore();
                return;
            }
            emit channelsChanged(command->channels());
            this->setImage(img);

        } else {
            qWarning() << Q_FUNC_INFO << "No reader founds for input: " << path;
        }
    }

    // ///////////////////////////////////////////////////////////////

    event->accept();
}

// ///////////////////////////////////////////////////////////////////

#include "gnomonViewVolumic.moc"

//
// gnomonViewVolumic.cpp ends here
