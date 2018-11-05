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

#include "gnomonViewVolumic.h"

#include <gnomonStyle>

#include <gnomonAbstractImageSeriesReader.h>

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

public:
    QSize sizeHint(void) const;

public:
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> window;
    vtkSmartPointer<vtkRenderer> renderer;

public:
    gnomonViewVolumic *q = nullptr;

public:
    vtkSmartPointer<vtkResliceImageViewer> viewer = nullptr;

public:
    gnomonAbstractImageSeriesReader *image_reader = nullptr;

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

QSize gnomonViewVolumicPrivate::sizeHint(void) const
{
    return QSize(800, 600);
}

// ///////////////////////////////////////////////////////////////////
// gnomonViewVolumic
// ///////////////////////////////////////////////////////////////////

gnomonViewVolumic::gnomonViewVolumic(QWidget *parent) : QFrame(parent)
{
    d = new gnomonViewVolumicPrivate;
    d->q = this;

    d->image_reader = dtkImaging::imageSeriesReader::pluginFactory().create("gnomonImageSeriesReader");
    if(!d->image_reader) {
        qCritical() << Q_FUNC_INFO << "imageSeriesReader Plugin could not be created";
    }

    d->slider = new QSlider(this);
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
    delete d;
}

void gnomonViewVolumic::setImage(vtkSmartPointer<vtkImageData> image)
{
    d->viewer->SetInputData(image);

    d->renderer->ResetCamera();
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

    qDebug() << Q_FUNC_INFO << "Importing" << path;

    d->image_reader->setPath(path);
    d->view->setImage(d->image_reader->next());

    event->accept();
}

// ///////////////////////////////////////////////////////////////////

#include "gnomonViewVolumic.moc"

//
// gnomonViewVolumic.cpp ends here
