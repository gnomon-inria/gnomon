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
#include <vtkInteractorStyleImage.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkResliceImageViewer.h>
#include <vtkSmartPointer.h>

#include <QVTKOpenGLWidget.h>

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

signals:
    void clicked(void);

protected:
    void mousePressEvent(QMouseEvent *);

private:
    gnomonFontAwesome *font;
};

gnomonViewVolumicOverlay::gnomonViewVolumicOverlay(fa::icon icon, QWidget *parent) : QLabel(parent)
{
    this->font = new gnomonFontAwesome(this);
    this->font->initFontAwesome();
    this->font->setDefaultOption("color", QColor("#ffffff"));

    this->setPixmap(this->font->icon(icon).pixmap(32, 32));

    this->setStyleSheet("background: none;");
}

gnomonViewVolumicOverlay::~gnomonViewVolumicOverlay(void)
{

}

void gnomonViewVolumicOverlay::mousePressEvent(QMouseEvent *)
{
    emit clicked();
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
    vtkSmartPointer<vtkRenderer> renderer;

public:
    gnomonViewVolumic *q = nullptr;

public:
    vtkSmartPointer<vtkResliceImageViewer> viewer = nullptr;

public:
    gnomonImagesSerieReaderCommand *image_reader_command = nullptr;

public:
    gnomonViewVolumicOverlay *export_button;

public:
    dtkImage *image = nullptr;

public:
    QSlider *slider;
};

gnomonViewVolumicPrivate::gnomonViewVolumicPrivate(QWidget *parent) : QVTKOpenGLWidget(parent)
{
    QColor background_color = QColor(GNOMON_STYLE_BACKGROUNDCOLOR);

    this->renderer = vtkSmartPointer<vtkRenderer>::New();
    this->renderer->SetBackground(background_color.redF(), background_color.greenF(), background_color.blueF());

    this->window = vtkGenericOpenGLRenderWindow::New();
    this->window->AddRenderer(this->renderer);

    this->SetRenderWindow(this->window);
    this->setEnableHiDPI(true);

    this->export_button = new gnomonViewVolumicOverlay(fa::arrowcircleup, this);

    connect(this->export_button, SIGNAL(clicked()), this, SLOT(exportToManager()));
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

    gnomonImageManager::instance()->addImage(this->image);
}

QSize gnomonViewVolumicPrivate::sizeHint(void) const
{
    return QSize(800, 600);
}

void gnomonViewVolumicPrivate::resizeEvent(QResizeEvent *event)
{
    this->export_button->move(event->size().width() - 40, 10);
}

// ///////////////////////////////////////////////////////////////////
// gnomonViewVolumic
// ///////////////////////////////////////////////////////////////////

gnomonViewVolumic::gnomonViewVolumic(QWidget *parent) : QFrame(parent)
{
    d = new gnomonViewVolumicPrivate;
    d->q = this;

    d->image_reader_command = new gnomonImagesSerieReaderCommand("gnomonImagesSerieReader");

    Q_ASSERT(d->image_reader_command);

    d->slider = new QSlider(this);
    d->slider->setObjectName("prout");
    d->slider->setOrientation(Qt::Vertical);
    d->slider->setMinimum(0);
    d->slider->setMaximum(1);
    d->slider->setValue(0);

    d->viewer = vtkSmartPointer<vtkResliceImageViewer>::New();
    d->viewer->SetSliceOrientationToXY();
    d->viewer->SetRenderWindow(d->window);
    d->viewer->SetRenderer(d->renderer);
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
    if (d->image_reader_command)
        delete d->image_reader_command;

    delete d;
}

void gnomonViewVolumic::setImage(dtkImage *i)
{
    dtkImageConverter *converter = dtkImaging::converter::pluginFactory().create("dtkVtkImageConverter");
    converter->setInput(i);
    converter->convert();

    vtkImageData *image = static_cast<vtkImageData *>(converter->output());

    delete converter;

    double bounds[6]; image->GetBounds(bounds);

    d->viewer->SetSlice((bounds[5] - bounds[4]) / 2);
    d->viewer->SetInputData(image);

    d->slider->setMaximum(bounds[5] - bounds[4]);
    d->slider->blockSignals(true);
    d->slider->setValue((bounds[5] - bounds[4]) / 2);
    d->slider->blockSignals(false);

    d->renderer->ResetCamera();

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

        d->image_reader_command->setPath(path.remove("file://"));
        d->image_reader_command->redo();

        d->image = d->image_reader_command->next();

        if (!d->image) {
            qDebug() << Q_FUNC_INFO << "Resulting image is void.";
            event->ignore();
            return;
        }

        this->setImage(d->image);
    }

    // ///////////////////////////////////////////////////////////////

    event->accept();
}

// ///////////////////////////////////////////////////////////////////

#include "gnomonViewVolumic.moc"

//
// gnomonViewVolumic.cpp ends here
