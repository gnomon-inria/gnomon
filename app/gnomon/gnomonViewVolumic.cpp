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
#include <vtkImageViewer2.h>
#include <vtkImageMapToColors.h>
#include <vtkImageMapToWindowLevelColors.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkInteractorStyleImage.h>
#include <vtkPolyDataMapper.h>
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
    this->renderer2D_XZ = new gnomonViewVolumicOverlay(":gnomon/gnomonViewVolumic-XZ-off.png", this);
    this->renderer2D_YZ = new gnomonViewVolumicOverlay(":gnomon/gnomonViewVolumic-YZ-off.png", this);

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

        vtkSmartPointer<QVTKInteractor> interactor = vtkSmartPointer<QVTKInteractor>::New();
        interactor->SetRenderWindow(this->RenderWindow);

        vtkSmartPointer<vtkInteractorStyleImage> style = vtkSmartPointer<vtkInteractorStyleImage>::New();
        interactor->SetInteractorStyle(style);

        this->renderer2D->InteractiveOn();
        this->renderer2D->DrawOn();

        this->slider->setValue(this->slider->value());

        this->slider->setValue(this->slider->value() + 1);
        this->slider->setValue(this->slider->value() - 1);
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

        vtkSmartPointer<QVTKInteractor> interactor = vtkSmartPointer<QVTKInteractor>::New();
        interactor->SetRenderWindow(this->RenderWindow);

        vtkSmartPointer<vtkInteractorStyleTrackballCamera> style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
        interactor->SetInteractorStyle(style);

        this->renderer3D->InteractiveOn();
        this->renderer3D->DrawOn();

        this->slider->setValue(this->slider->value() + 1);
        this->slider->setValue(this->slider->value() - 1);
    });

    connect(this->renderer2D_XY, &gnomonViewVolumicOverlay::clicked, [this] () {

        this->renderer2D_XY->changePath(":gnomon/gnomonViewVolumic-XY.png");
        this->renderer2D_XZ->changePath(":gnomon/gnomonViewVolumic-XZ-off.png");
        this->renderer2D_YZ->changePath(":gnomon/gnomonViewVolumic-YZ-off.png");
    });

    connect(this->renderer2D_XZ, &gnomonViewVolumicOverlay::clicked, [this] () {

        this->renderer2D_XY->changePath(":gnomon/gnomonViewVolumic-XY-off.png");
        this->renderer2D_XZ->changePath(":gnomon/gnomonViewVolumic-XZ.png");
        this->renderer2D_YZ->changePath(":gnomon/gnomonViewVolumic-YZ-off.png");
    });

    connect(this->renderer2D_YZ, &gnomonViewVolumicOverlay::clicked, [this] () {

        this->renderer2D_XY->changePath(":gnomon/gnomonViewVolumic-XY-off.png");
        this->renderer2D_XZ->changePath(":gnomon/gnomonViewVolumic-XZ-off.png");
        this->renderer2D_YZ->changePath(":gnomon/gnomonViewVolumic-YZ.png");
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

    vtkImageData *dummy = vtkImageData::New();
    dummy->SetDimensions(1, 1, 1);
    dummy->SetSpacing(1, 1, 1);
#if VTK_MAJOR_VERSION <= 5
    dummy->SetNumberOfScalarComponents(1);
    dummy->SetScalarTypeToUnsignedChar();
#else
    dummy->AllocateScalars(VTK_UNSIGNED_CHAR,1);
#endif

    d->viewer = vtkSmartPointer<vtkResliceImageViewer>::New();
    d->viewer->SetSliceOrientationToXY();
    d->viewer->SetRenderWindow(d->window);
    d->viewer->SetRenderer(d->renderer2D);
    d->viewer->SetupInteractor(d->GetInteractor());
    d->viewer->SetResliceModeToAxisAligned();
    d->viewer->GetWindowLevel()->SetOutputFormatToRGB();
    d->viewer->SetInputData(dummy);

    connect(d->slider, &QSlider::valueChanged, [=] (int value) {
        d->viewer->SetSlice(value);
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

    int z = image->GetDimensions()[2];

    d->viewer->SetSlice(z/2);
    d->viewer->SetInputData(image);

    d->slider->setMaximum(z);
    d->slider->blockSignals(true);
    d->slider->setValue(z/2);
    d->slider->blockSignals(false);

    // 3D

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

    d->GetInteractor()->Render();
}

dtkImage *gnomonViewVolumic::image(void)
{
    return d->image;
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
