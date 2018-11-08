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

#include "gnomonWorkspaceBrowser.h"
#include "gnomonWorkspaceFusion.h"
#include "gnomonWorkspaceSegmentation.h"
#include "gnomonWorkspacePreprocess.h"

#include <gnomonStyle>
#include <gnomonFonts>

#include <gnomonImagesSerieReaderCommand.h>

#include <vtkActor.h>
#include <vtkContourFilter.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkImageData.h>
#include <vtkImageViewer2.h>
#include <vtkImageMapToColors.h>
#include <vtkImageMapToWindowLevelColors.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkResliceImageViewer.h>
#include <vtkSmartPointer.h>
#include <vtkSmartVolumeMapper.h>
#include <vtkVolume.h>
#include <QVTKOpenGLWidget.h>

#include <QVTKInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkInteractorStyleImage.h>

#include <dtkImagingCore>

// ///////////////////////////////////////////////////////////////////
// gnomonViewVolumicOverlay
// ///////////////////////////////////////////////////////////////////

class gnomonViewVolumicOverlay : public QLabel
{
    Q_OBJECT

public:
     gnomonViewVolumicOverlay(fa::icon, QWidget *parent = nullptr);
    ~gnomonViewVolumicOverlay(void);

    void changeColor(const QColor&);
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

    connect(this->export_button, SIGNAL(clicked()), this, SLOT(exportToManager()));

    connect(this->renderer2D_button, &gnomonViewVolumicOverlay::clicked, [this] () {
            this->renderer2D_button->setEnabled(false);
            this->renderer2D_button->changeColor(Qt::white);
            this->renderer3D_button->setEnabled(true);
            this->renderer3D_button->changeColor(Qt::gray);

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
    { parent = parent->parentWidget(); }

    QStackedWidget *stack = dynamic_cast< QStackedWidget * >(parent);
    if(gnomonWorkspaceBrowser* workspace = dynamic_cast<gnomonWorkspaceBrowser *>(stack->currentWidget()))
    { gnomonImageManager::instance()->addImage(this->image, Qt::black); }
    else if(gnomonWorkspaceFusion* workspace = dynamic_cast<gnomonWorkspaceFusion *>(stack->currentWidget()))
    { gnomonImageManager::instance()->addImage(this->image, Qt::red); }
    else if(gnomonWorkspaceSegmentation* workspace = dynamic_cast<gnomonWorkspaceSegmentation *>(stack->currentWidget()))
    { gnomonImageManager::instance()->addImage(this->image, Qt::green); }
    else if(gnomonWorkspacePreprocess* workspace = dynamic_cast<gnomonWorkspacePreprocess *>(stack->currentWidget()))
    { gnomonImageManager::instance()->addImage(this->image, Qt::blue); }
    else
    { gnomonImageManager::instance()->addImage(this->image, Qt::white); }

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

    d->viewer = vtkSmartPointer<vtkResliceImageViewer>::New();
    d->viewer->SetSliceOrientationToXY();
    d->viewer->SetRenderWindow(d->window);
    d->viewer->SetRenderer(d->renderer2D);
    d->viewer->SetupInteractor(d->GetInteractor());
    d->viewer->SetResliceModeToAxisAligned();
    d->viewer->GetWindowLevel()->SetOutputFormatToRGB();

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
