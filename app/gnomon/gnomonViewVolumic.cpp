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
#include <vtkContourFilter.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkImageData.h>
#include <vtkImagePlaneWidget.h>
#include <vtkImageViewer2.h>
#include <vtkImageMapToColors.h>
#include <vtkImageMapToWindowLevelColors.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkInteractorStyleImage.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkResliceImageViewer.h>
#include <vtkSmartPointer.h>
#include <vtkSmartVolumeMapper.h>
#include <vtkVolume.h>
#include <QVTKInteractor.h>
#include <QVTKOpenGLWidget.h>

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

private:
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

public:
    dtkImage *image = nullptr;

public:
    QSlider *slider;

public:
    int x = 0, c_x = 0;
    int y = 0, c_y = 0;
    int z = 0, c_z = 0;
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

    vtkImageData *dummy = vtkImageData::New();
    dummy->SetDimensions(1, 1, 1);
    dummy->SetSpacing(1, 1, 1);
#if VTK_MAJOR_VERSION <= 5
    dummy->SetNumberOfScalarComponents(1);
    dummy->SetScalarTypeToUnsignedChar();
#else
    dummy->AllocateScalars(VTK_UNSIGNED_CHAR,1);
#endif

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

        vtkSmartPointer<vtkInteractorStyleImage> style = vtkSmartPointer<vtkInteractorStyleImage>::New();

        this->GetInteractor()->SetInteractorStyle(style);

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

        q->render();
    });

    connect(this->renderer2D_XY, &gnomonViewVolumicOverlay::clicked, [=] () {

        this->renderer2D_XY->changePath(":gnomon/gnomonViewVolumic-XY.png");
        this->renderer2D_XZ->changePath(":gnomon/gnomonViewVolumic-XZ-off.png");
        this->renderer2D_YZ->changePath(":gnomon/gnomonViewVolumic-YZ-off.png");

        this->renderer2D_XY->on = true;
        this->renderer2D_XZ->on = false;
        this->renderer2D_YZ->on = false;

        this->setSliceOrientation(SLICE_ORIENTATION_XY);

        this->viewer->SetSlice(this->c_z);

        this->planeWidget[0]->On();
        this->planeWidget[1]->On();
        this->planeWidget[2]->Off();

        this->slider->blockSignals(true);
        this->slider->setMaximum(this->z);
        this->slider->setValue(this->c_z);
        this->slider->blockSignals(false);
    });

    connect(this->renderer2D_XZ, &gnomonViewVolumicOverlay::clicked, [=] () {

        this->renderer2D_XY->changePath(":gnomon/gnomonViewVolumic-XY-off.png");
        this->renderer2D_XZ->changePath(":gnomon/gnomonViewVolumic-XZ.png");
        this->renderer2D_YZ->changePath(":gnomon/gnomonViewVolumic-YZ-off.png");

        this->renderer2D_XY->on = false;
        this->renderer2D_XZ->on = true;
        this->renderer2D_YZ->on = false;

        this->setSliceOrientation(SLICE_ORIENTATION_XZ);

        this->viewer->SetSlice(this->c_y);

        this->planeWidget[0]->On();
        this->planeWidget[1]->Off();
        this->planeWidget[2]->On();

        this->slider->blockSignals(true);
        this->slider->setMaximum(this->y);
        this->slider->setValue(this->c_y);
        this->slider->blockSignals(false);
    });

    connect(this->renderer2D_YZ, &gnomonViewVolumicOverlay::clicked, [=] () {

        this->renderer2D_XY->changePath(":gnomon/gnomonViewVolumic-XY-off.png");
        this->renderer2D_XZ->changePath(":gnomon/gnomonViewVolumic-XZ-off.png");
        this->renderer2D_YZ->changePath(":gnomon/gnomonViewVolumic-YZ.png");

        this->renderer2D_XY->on = false;
        this->renderer2D_XZ->on = false;
        this->renderer2D_YZ->on = true;

        this->setSliceOrientation(SLICE_ORIENTATION_YZ);

        this->viewer->SetSlice(this->c_x);

        this->planeWidget[0]->Off();
        this->planeWidget[1]->On();
        this->planeWidget[2]->On();

        this->slider->blockSignals(true);
        this->slider->setMaximum(this->x);
        this->slider->setValue(this->c_x);
        this->slider->blockSignals(false);
    });
}

gnomonViewVolumicPrivate::~gnomonViewVolumicPrivate(void)
{

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

    d->slider = new QSlider(this);
    d->slider->setObjectName("prout");
    d->slider->setOrientation(Qt::Vertical);
    d->slider->setMinimum(0);
    d->slider->setMaximum(1);
    d->slider->setValue(0);

    connect(d->slider, &QSlider::valueChanged, [=] (int value) {

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
    });

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(d->slider);
    layout->addWidget(d);

    this->setAcceptDrops(true);
}

gnomonViewVolumic::~gnomonViewVolumic(void)
{
    if (d->image_reader_command_inr)
        delete d->image_reader_command_inr;

    if (d->image_reader_command_czi)
        delete d->image_reader_command_czi;

    delete d;
}

void gnomonViewVolumic::setImage(dtkImage *i)
{
    d->image = i;

    // 2D

    dtkImageConverter *converter = dtkImaging::converter::pluginFactory().create("dtkVtkImageConverter");
    converter->setInput(i);
    converter->convert();

    vtkImageData *image = static_cast<vtkImageData *>(converter->output());

    delete converter;

    d->x = image->GetDimensions()[0];
    d->y = image->GetDimensions()[1];
    d->z = image->GetDimensions()[2];

    d->c_x = d->x/2;
    d->c_y = d->y/2;
    d->c_z = d->z/2;

    d->viewer->SetInputData(image);

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

    d->volume->SetMapper(d->volume_mapper);
    // d->volume->SetProperty(d->volume_property);
    d->volume->Modified();
    d->volume->Update();

    d->renderer3D->AddActor(d->volume);

    d->renderer2D->ResetCamera();
    d->renderer3D->ResetCamera();

    this->render();
}

dtkImage *gnomonViewVolumic::image(void)
{
    return d->image;
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
    this->setImage(img);
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

        if(path.endsWith("inr") || path.endsWith("inr.gz")) {
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
            dtkImage *img = command->next();

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
