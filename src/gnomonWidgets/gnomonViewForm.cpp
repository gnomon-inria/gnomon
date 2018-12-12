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

#include "gnomonViewForm.h"

#include "gnomonToolBar.h"
#include "gnomonViewVolumicOverlay.h"

#include <gnomonCore/gnomonAbstractCommand>
#include <gnomonCore/gnomonImagesSerieReaderCommand>
#include <gnomonCore/gnomonMeshReaderCommand>

#include <dtkImagingCore>
#include <gnomonCore/gnomonMesh>
#include <gnomonCore/gnomonImagesSerie>

#include "gnomonOverlayPane.h"
#include "gnomonOverlayPaneItem.h"

#include "gnomonVisualizationMesh.h"
#include "gnomonVisualizationImagesSerie.h"

#include <vtkCamera.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkInteractorStyleImage.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>

#include <QVTKInteractor.h>
#include <QVTKOpenGLWidget.h>

// ///////////////////////////////////////////////////////////////////
// gnomonViewFormPrivate
// ///////////////////////////////////////////////////////////////////

class gnomonViewFormPrivate : public QVTKOpenGLWidget
{
    Q_OBJECT

public:
    enum Orientation {
        SLICE_ORIENTATION_XY = 2,
        SLICE_ORIENTATION_XZ = 1,
        SLICE_ORIENTATION_YZ = 0
    };

public:
     gnomonViewFormPrivate(QWidget *parent = Q_NULLPTR);
    ~gnomonViewFormPrivate(void);

public slots:
    void enableInteractor(void);
    void disableInteractor(void);

public:
    QSize sizeHint(void) const;

protected:
    void resizeEvent(QResizeEvent *);

public:
    Orientation orientation(void);

public:
    void setSliceOrientation(Orientation orientation);
    void updateOrientation(void);

public:
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> window;
    vtkSmartPointer<vtkRenderer> renderer2D;
    vtkSmartPointer<vtkRenderer> renderer3D;

public:
    gnomonViewForm *q = nullptr;

public:
    Orientation ori;
    QMap<Orientation, vtkSmartPointer<vtkCamera> > cameras;

public:
    gnomonVisualizationMesh *visuMesh = nullptr;
    gnomonVisualizationImagesSerie *visuImagesSerie = nullptr;

public:
    gnomonMeshReaderCommand *mesh_reader_command = nullptr;
    gnomonImagesSerieReaderCommand *image_reader_command = nullptr;

public:
    gnomonViewVolumicOverlay *renderer2D_button = nullptr;
    gnomonViewVolumicOverlay *renderer3D_button = nullptr;
    gnomonViewVolumicOverlay *renderer2D_XY = nullptr;
    gnomonViewVolumicOverlay *renderer2D_XZ = nullptr;
    gnomonViewVolumicOverlay *renderer2D_YZ = nullptr;

public:
    gnomonMesh *mesh;
    gnomonImagesSerie *images_serie;

public:
    QSlider *slice_slider;

public:
    double xBounds[2] = {0,0}, yBounds[2] = {0,0}, zBounds[2] = {0,0};
    double c_x = 0, c_y = 0, c_z = 0;

signals:
    void sliceOrientationChanged(int);
};

gnomonViewFormPrivate::gnomonViewFormPrivate(QWidget *parent) : QVTKOpenGLWidget(parent)
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

    this->renderer2D_button = new gnomonViewVolumicOverlay(fa::square, "", this);
    this->renderer2D_button->toggle(false);
    this->renderer3D_button = new gnomonViewVolumicOverlay(fa::cube, "", this);
    this->renderer2D_XY = new gnomonViewVolumicOverlay(":gnomon/gnomonViewVolumic-XY.png", ":gnomon/gnomonViewVolumic-XY-off.png", "", this);
    this->renderer2D_XY->toggle(false);
    this->renderer2D_XZ = new gnomonViewVolumicOverlay(":gnomon/gnomonViewVolumic-XZ.png", ":gnomon/gnomonViewVolumic-XZ-off.png", "", this);
    this->renderer2D_XZ->toggle(false);
    this->renderer2D_YZ = new gnomonViewVolumicOverlay(":gnomon/gnomonViewVolumic-YZ.png",  ":gnomon/gnomonViewVolumic-YZ-off.png", "", this);
    this->renderer2D_YZ->toggle(false);
}

gnomonViewFormPrivate::~gnomonViewFormPrivate(void)
{
}

void gnomonViewFormPrivate::enableInteractor(void)
{
    this->GetInteractor()->Enable();
}

void gnomonViewFormPrivate::disableInteractor(void)
{
    this->GetInteractor()->Disable();
}

QSize gnomonViewFormPrivate::sizeHint(void) const
{
    return QSize(800, 600);
}

void gnomonViewFormPrivate::resizeEvent(QResizeEvent *event)
{
    this->renderer2D_button->move(10, 10);
    this->renderer3D_button->move(50, 10);
    this->renderer2D_XY->move(10,  50);
    this->renderer2D_XZ->move(10,  90);
    this->renderer2D_YZ->move(10, 130);

    QVTKOpenGLWidget::resizeEvent(event);
}

gnomonViewFormPrivate::Orientation gnomonViewFormPrivate::orientation(void)
{
    return this->ori;
}

void gnomonViewFormPrivate::setSliceOrientation(Orientation orientation)
{
    this->ori = orientation;
    this->updateOrientation();
}


void gnomonViewFormPrivate::updateOrientation(void)
{
    qDebug()<<"Update camera"<<this->ori<<this->cameras.keys();
    if(!this->cameras.contains(this->ori)) {
        vtkSmartPointer<vtkCamera> cam = vtkCamera::New();
        cam->ParallelProjectionOn();
        cam->SetParallelScale(1);
        cam->SetFocalPoint((xBounds[0]+xBounds[1])/2,(yBounds[0]+yBounds[1])/2,(zBounds[0]+zBounds[1])/2);
        qDebug()<<"  --> X:"<<xBounds[0]<<xBounds[1];
        qDebug()<<"  --> Y:"<<yBounds[0]<<yBounds[1];
        qDebug()<<"  --> Z:"<<zBounds[0]<<zBounds[1];
        qDebug()<<"  --> FocalPoint:  "<<(xBounds[0]+xBounds[1])/2<<(yBounds[0]+yBounds[1])/2<<(zBounds[0]+zBounds[1])/2;
    
     
        switch(this->ori)
        {
            case SLICE_ORIENTATION_XY:
                cam->SetPosition((xBounds[0]+xBounds[1])/2,(yBounds[0]+yBounds[1])/2,zBounds[1]);
                cam->SetViewUp(0,1,0);
                cam->SetClippingRange((zBounds[1] - zBounds[0]) - 3.0, (zBounds[1] - zBounds[0]) + 3.0);
                qDebug()<<"  --> Position:    "<<cam->GetPosition()[0]<<cam->GetPosition()[1]<<cam->GetPosition()[2];
                break;

            case SLICE_ORIENTATION_XZ:
                cam->SetPosition((xBounds[0]+xBounds[1])/2,yBounds[0],(zBounds[0]+zBounds[1])/2);
                cam->SetViewUp(0,0,1);
                cam->SetClippingRange((yBounds[1] - yBounds[0]) - 3.0, (yBounds[1] - yBounds[0]) + 3.0);
                qDebug()<<"  --> Position:    "<<cam->GetPosition()[0]<<cam->GetPosition()[1]<<cam->GetPosition()[2];
                break;

            case SLICE_ORIENTATION_YZ:
                cam->SetPosition(xBounds[1],(yBounds[0]+yBounds[1])/2,(zBounds[0]+zBounds[1])/2);
                cam->SetViewUp(0,0,1);
                cam->SetClippingRange((xBounds[1] - xBounds[0]) - 3.0, (xBounds[1] - xBounds[0]) + 3.0);
                qDebug()<<"  --> Position:    "<<cam->GetPosition()[0]<<cam->GetPosition()[1]<<cam->GetPosition()[2];
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

    double foc[3];
    this->renderer2D->GetActiveCamera()->GetFocalPoint(foc);
    qDebug()<<"  --> FocalPoint:  "<<foc[0]<<foc[1]<<foc[2];

    double pos[3];
    this->renderer2D->GetActiveCamera()->GetPosition(pos);
    qDebug()<<"  --> Position:    "<<pos[0]<<pos[1]<<pos[2];

    this->GetInteractor()->Render();
      
}

// ///////////////////////////////////////////////////////////////////
// gnomonViewForm
// ///////////////////////////////////////////////////////////////////


gnomonViewForm::gnomonViewForm(QWidget *parent) : QFrame(parent)
{
    d = new gnomonViewFormPrivate;
    d->q = this;

    connect(d->renderer2D_button, SIGNAL(iconClicked()), this, SLOT(switchTo2D()));
    connect(d->renderer3D_button, SIGNAL(iconClicked()), this, SLOT(switchTo3D()));
    connect(d->renderer2D_XY, SIGNAL(iconClicked()), this, SLOT(switchTo2DXY()));
    connect(d->renderer2D_XZ, SIGNAL(iconClicked()), this, SLOT(switchTo2DXZ()));
    connect(d->renderer2D_YZ, SIGNAL(iconClicked()), this, SLOT(switchTo2DYZ()));

    d->slice_slider = new QSlider(this);
    d->slice_slider->setObjectName("Slice Position");
    d->slice_slider->setOrientation(Qt::Vertical);
    d->slice_slider->setMinimum(0);
    d->slice_slider->setMaximum(1);
    d->slice_slider->setValue(0);

    gnomonOverlayPane *pane = new gnomonOverlayPane(parent);

    QFormLayout *parameterLayout = new QFormLayout;
    gnomonOverlayPaneItem *paneItemParameterLayout = new gnomonOverlayPaneItem(parent);
    paneItemParameterLayout->setTitle("Visualization");
    paneItemParameterLayout->addLayout(parameterLayout);
    paneItemParameterLayout->toggle();

    QPushButton *button = new QPushButton("Render",parent);
    button->setCheckable(true);
    gnomonOverlayPaneItem *paneItemButton = new gnomonOverlayPaneItem(parent);
    paneItemButton->setTitle("Visualization");
    paneItemButton->addWidget(button);
    paneItemButton->toggle();

    pane->addWidget(paneItemParameterLayout);
    pane->addWidget(paneItemButton);
    pane->toggle();

    connect(d->slice_slider, SIGNAL(valueChanged(int)), this, SLOT(sliceChange(int)));

    connect(d, &gnomonViewFormPrivate::sliceOrientationChanged, this, &gnomonViewForm::sliceOrientationChanged);

    QGridLayout *layout  = new QGridLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(d->slice_slider, 0, 0, 1, 1);
    layout->addWidget(d, 0, 1, 1, 1);
    layout->addWidget(pane, 0, 2, 1, 1);

    this->setAcceptDrops(true);
    this->switchTo2D(); 
    this->switchTo2DXY();
    d->updateOrientation(); 
}

gnomonViewForm::~gnomonViewForm(void)
{
    delete d;
}

void gnomonViewForm::switchTo3D(void)
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

    emit switchedTo3D();
}

void gnomonViewForm::switchTo2D(void)
{
    if (d->renderer2D_button->isToggled()) return;

    qDebug()<<"switchTo2D";

    d->renderer2D_button->toggle(true);
    d->renderer2D_button->setEnabled(false);

    d->renderer3D_button->toggle(false);
    d->renderer3D_button->setEnabled(true);

    d->renderer2D_XY->setVisible(true);
    d->renderer2D_XY->toggle(false);
    d->renderer2D_XZ->setVisible(true);
    d->renderer2D_XZ->toggle(false);
    d->renderer2D_YZ->setVisible(true);
    d->renderer2D_YZ->toggle(false);

    d->renderer3D->DrawOff();
    d->renderer3D->InteractiveOff();

    vtkSmartPointer<vtkInteractorStyleImage> style = vtkSmartPointer<vtkInteractorStyleImage>::New();
    d->GetInteractor()->SetInteractorStyle(style);

    d->renderer2D->InteractiveOn();
    d->renderer2D->DrawOn();
    d->slice_slider->setEnabled(true);

    emit switchedTo2D();

    switch(d->ori) {
        case gnomonViewFormPrivate::SLICE_ORIENTATION_XY:
            this->switchTo2DXY();
            break;

        case gnomonViewFormPrivate::SLICE_ORIENTATION_XZ:
            this->switchTo2DXZ();
            break;

        case gnomonViewFormPrivate::SLICE_ORIENTATION_YZ:
            this->switchTo2DYZ();
            break;
    }
}

void gnomonViewForm::switchTo2DXY(void)
{
    if (d->renderer2D_XY->isToggled()) return;
    
    qDebug()<<"switchTo2D_XY";

    d->renderer2D_XY->toggle(true);
    d->renderer2D_XZ->toggle(false);
    d->renderer2D_YZ->toggle(false);

    d->setSliceOrientation(gnomonViewFormPrivate::SLICE_ORIENTATION_XY);
    emit sliceOrientationChanged(gnomonViewFormPrivate::SLICE_ORIENTATION_XY);

    d->slice_slider->blockSignals(true);
    d->slice_slider->setMinimum(d->zBounds[0]);
    d->slice_slider->setMaximum(d->zBounds[1]);
    d->slice_slider->blockSignals(false);
    d->slice_slider->setValue(d->c_z);

    emit switchedTo2DXY();
}

void gnomonViewForm::switchTo2DXZ(void)
{
    if (d->renderer2D_XZ->isToggled()) return;

    qDebug()<<"switchTo2D_XZ";

    d->renderer2D_XY->toggle(false);
    d->renderer2D_XZ->toggle(true);
    d->renderer2D_YZ->toggle(false);

    d->setSliceOrientation(gnomonViewFormPrivate::SLICE_ORIENTATION_XZ);
    emit sliceOrientationChanged(gnomonViewFormPrivate::SLICE_ORIENTATION_XZ);

    d->slice_slider->blockSignals(true);
    d->slice_slider->setMinimum(d->yBounds[0]);
    d->slice_slider->setMaximum(d->yBounds[1]);
    d->slice_slider->blockSignals(false);
    d->slice_slider->setValue(d->c_y);

    emit switchedTo2DXZ();
}

void gnomonViewForm::switchTo2DYZ(void)
{
    if (d->renderer2D_YZ->isToggled()) return;

    qDebug()<<"switchTo2D_YZ";

    d->renderer2D_XY->toggle(false);
    d->renderer2D_XZ->toggle(false);
    d->renderer2D_YZ->toggle(true);

    d->setSliceOrientation(gnomonViewFormPrivate::SLICE_ORIENTATION_YZ);
    emit sliceOrientationChanged(gnomonViewFormPrivate::SLICE_ORIENTATION_YZ);

    d->slice_slider->blockSignals(true);
    d->slice_slider->setMinimum(d->xBounds[0]);
    d->slice_slider->setMaximum(d->xBounds[1]);
    d->slice_slider->blockSignals(false);
    d->slice_slider->setValue(d->c_x);

    emit switchedTo2DYZ();
}

void gnomonViewForm::sliceChange(int value)
{
    bool valueChanged = false;

    if (d->renderer2D_XY->isToggled()) {
        if (d->c_z != value) {
            d->c_z = value;
            valueChanged = true;
        }
    }

    if (d->renderer2D_XZ->isToggled()) {
        if (d->c_y != value) {
            d->c_y = value;
            valueChanged = true;
        }
    }

    if (d->renderer2D_YZ->isToggled()) {
        if (d->c_x != value){
            d->c_x = value;
            valueChanged = true;
        }
    }

    d->slice_slider->blockSignals(true);
    d->slice_slider->setValue(value);
    d->slice_slider->blockSignals(false);

    if (valueChanged)
        emit sliceChanged(value);

    d->GetInteractor()->Render();
}


gnomonImagesSerie *gnomonViewForm::imagesSerie(void)
{
    return d->images_serie;
}

void gnomonViewForm::setImagesSerie(gnomonImagesSerie* images_serie)
{
    d->images_serie = images_serie;
    qDebug()<<"Images Serie"<<images_serie->channel();
    // d->last_channel_toggled = images_serie->channel();

    // bool enable_slider = images_serie->times().count() > 1;

    // d->time_slider->setVisible(enable_slider);

    // if(d->images_serie->image())
    //     setImage(d->images_serie->image(), source);

    if (!d->visuImagesSerie)
        d->visuImagesSerie = new gnomonVisualizationImagesSerie(this);
    d->visuImagesSerie->setImagesSerie(images_serie);
    d->visuImagesSerie->setParameter("alpha",0.5);

    if (d->renderer3D_button->isToggled()) {
        d->renderer3D_button->toggle(false);
        this->switchTo3D();
    }
    else if (d->renderer2D_button->isToggled()) {
        d->renderer2D_button->toggle(false);
        this->switchTo2D();
    }
}

gnomonMesh *gnomonViewForm::mesh(void)
{
    return d->mesh;
}

void gnomonViewForm::setMesh(gnomonMesh *mesh)
{
    d->mesh = mesh;

    if (!d->visuMesh)
        d->visuMesh = new gnomonVisualizationMesh(this);
    d->visuMesh->setMesh(mesh);
    d->visuMesh->setParameter("alpha",0.5);

    if (d->renderer3D_button->isToggled()) {
        d->renderer3D_button->toggle(false);
        this->switchTo3D();
    }
    else if (d->renderer2D_button->isToggled()) {
        d->renderer2D_button->toggle(false);
        this->switchTo2D();
    }
}

void gnomonViewForm::setBounds(double bounds[6])
{   
    d->xBounds[0] = bounds[0];
    d->xBounds[1] = bounds[1];
    d->yBounds[0] = bounds[2];
    d->yBounds[1] = bounds[3];
    d->zBounds[0] = bounds[4];
    d->zBounds[1] = bounds[5];

    d->c_x = (d->xBounds[0]+d->xBounds[1])/2;
    d->c_y = (d->yBounds[0]+d->yBounds[1])/2;
    d->c_z = (d->zBounds[0]+d->zBounds[1])/2;

    d->cameras.clear();

    if (d->renderer2D_XY->isToggled()) {
        d->slice_slider->blockSignals(true);
        d->slice_slider->setMinimum(d->zBounds[0]);
        d->slice_slider->setMaximum(d->zBounds[1]);
        d->slice_slider->setValue(d->c_z);
        d->slice_slider->blockSignals(false);
    }

    if (d->renderer2D_XZ->isToggled()) {
        d->slice_slider->blockSignals(true);
        d->slice_slider->setMinimum(d->yBounds[0]);
        d->slice_slider->setMaximum(d->yBounds[1]);
        d->slice_slider->setValue(d->c_y);
        d->slice_slider->blockSignals(false);
    }

    if (d->renderer2D_YZ->isToggled()) {
        d->slice_slider->blockSignals(true);
        d->slice_slider->setMinimum(d->xBounds[0]);
        d->slice_slider->setMaximum(d->xBounds[1]);
        d->slice_slider->setValue(d->c_x);
        d->slice_slider->blockSignals(false);
    }

    d->renderer2D->ResetCamera();
    d->renderer3D->ResetCamera();

}

vtkRenderWindowInteractor *gnomonViewForm::interactor(void)
{
    return d->GetInteractor();
}

vtkRenderer *gnomonViewForm::renderer2D(void)
{
    return d->renderer2D;
}

vtkRenderer *gnomonViewForm::renderer3D(void)
{
    return d->renderer3D;
}

int gnomonViewForm::orientation(void)
{
    return d->ori;
}

void gnomonViewForm::render(void)
{
    d->renderer2D->ResetCameraClippingRange();
    d->GetInteractor()->Render();
}

void gnomonViewForm::onSliceChanged(int slice)
{
    d->slice_slider->setValue(slice);
}

void gnomonViewForm::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasText()) {
        event->accept();
        return;
    }

    event->ignore();
}

void gnomonViewForm::dragLeaveEvent(QDragLeaveEvent *event)
{
    event->accept();
}

void gnomonViewForm::dragMoveEvent(QDragMoveEvent *event)
{
    event->accept();
}

void gnomonViewForm::dropEvent(QDropEvent *event)
{
    QString path = event->mimeData()->text();

    if(path.startsWith(":")) {
        // gnomonImagesSerie * images_serie = gnomonImageManager::instance()->get(path.remove(":").toInt());
        // emit channelsChanged(images_serie->channels());
        // this->setImagesSerie(images_serie);
    } else {

        gnomonImagesSerieReaderCommand *imageCommand = nullptr;
        gnomonMeshReaderCommand *meshCommand = nullptr;

        if(path.endsWith("inr") || path.endsWith("inr.gz") || path.endsWith("mha") || path.endsWith("tif") || (path.endsWith("czi"))) {
            if(!d->image_reader_command)
                d->image_reader_command = new gnomonImagesSerieReaderCommand("gnomonImagesSerieReader");
            imageCommand = d->image_reader_command;
        }

        if(path.endsWith("ply")) {
            if(!d->mesh_reader_command)
                d->mesh_reader_command = new gnomonMeshReaderCommand("gnomonMeshReaderPropertyTopomesh");
            meshCommand = d->mesh_reader_command;
        }


        if(imageCommand) {
            imageCommand->setPath(path.remove("file://"));
            imageCommand->redo();
            gnomonImagesSerie * images_serie = imageCommand->imagesSerie()->copy();
            if (!images_serie) {
                qWarning() << Q_FUNC_INFO << "Resulting image series is void.";
                event->ignore();
                return;
            }
            qDebug()<<"Add image";
            // emit channelsChanged(images_serie->channels());
            // emit timeChanged(images_serie->time());
            // this->switchTo3D();
            this->setImagesSerie(images_serie);
        }
        else if(meshCommand)
        {
            meshCommand->setPath(path.remove("file://"));
            meshCommand->redo();

            gnomonMesh *mesh = (gnomonMesh *) meshCommand->mesh()->clone();
            if (!mesh) {
                qWarning() << Q_FUNC_INFO << "Resulting mesh is void.";
                event->ignore();
                return;
            }
            qDebug()<<"Add mesh";
            // this->switchTo3D();
            this->setMesh(mesh);
        } else {
            qWarning() << Q_FUNC_INFO << "No reader founds for input: " << path;
        }
    }


    // ///////////////////////////////////////////////////////////////

    event->accept();
}

// ///////////////////////////////////////////////////////////////////

#include "gnomonViewForm.moc"

//
// gnomonViewForm.cpp ends here
