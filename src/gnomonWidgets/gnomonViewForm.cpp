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

// #include "gnomonToolBar.h"
#include "gnomonViewVolumicOverlay.h"
#include "gnomonFormManager.h"

#include <gnomonCore/gnomonAbstractCommand>
#include <gnomonCore/gnomonImagesSerieReaderCommand>
#include <gnomonCore/gnomonCellImageReaderCommand>
#include <gnomonCore/gnomonCellComplexReaderCommand>
#include <gnomonCore/gnomonMeshReaderCommand>

#include <dtkImagingCore>
#include <dtkScript>

#include <gnomonCore/gnomonAbstractForm>
#include <gnomonCore/gnomonMesh>
#include <gnomonCore/gnomonCellComplex>
#include <gnomonCore/gnomonCellImage>
#include <gnomonCore/gnomonImagesSerie>

#include <gnomonCore/gnomonCoreParameter.h>
#include "gnomonWidgetsParameter.h"

#include "gnomonOverlayPane.h"
#include "gnomonOverlayPaneItem.h"

#include "gnomonVisualizationMesh.h"
#include "gnomonVisualizationCellComplex.h"
#include "gnomonVisualizationCellImage.h"
#include "gnomonVisualizationCellImageMarchingCubes.h"
#include "gnomonVisualizationImagesSerie.h"
#include "gnomonVisualizationImagesSerieChannelBlending.h"

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
    void exportToManager(void);

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
    QMap<QString, gnomonAbstractForm *> forms;
    QMap<QString, gnomonAbstractVisualization *> formVisualization;
    QMap<QString, gnomonAbstractCommand *> formReaderCommand;

public:
    gnomonViewVolumicOverlay *renderer2D_button = nullptr;
    gnomonViewVolumicOverlay *renderer3D_button = nullptr;
    gnomonViewVolumicOverlay *renderer2D_XY = nullptr;
    gnomonViewVolumicOverlay *renderer2D_XZ = nullptr;
    gnomonViewVolumicOverlay *renderer2D_YZ = nullptr;

    gnomonViewVolumicOverlay *sync = nullptr;
    gnomonViewVolumicOverlay *export_button = nullptr;

public:
    int syncing_count = 0;
    QTimer *syncing_timer = nullptr;
    bool synced = false;

public:
    bool acceptCellComplex = true;

public:
    QColor export_color = QColor("#cccccc");

public:
    QSlider *slice_slider;

public:
    gnomonOverlayPaneItem *paneItemButton = nullptr;
    QPushButton *renderButton = nullptr;

    QMap<QString, QFormLayout *> parameterLayouts;
    QMap<QString, gnomonOverlayPaneItem *> formVisualizationPaneItems;

    gnomonOverlayPane *formVisualizationPane = nullptr;

public:
    double xBounds[2] = {0,0}, yBounds[2] = {0,0}, zBounds[2] = {0,0};
    double c_x = 0, c_y = 0, c_z = 0;

signals:
    void sliceOrientationChanged(int);

public:
    gnomonOverlayPane *pane(QWidget *parent);

public slots:
    void configure(QWidget *parent, const QString& key);
    void refresh(void);
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

    this->sync = new gnomonViewVolumicOverlay(fa::unlock, "", this);
    this->sync->toggle(false);

    this->export_button = new gnomonViewVolumicOverlay(fa::arrowcircleup, "", this);
}

gnomonViewFormPrivate::~gnomonViewFormPrivate(void)
{
}

void gnomonViewFormPrivate::exportToManager(void)
{
    for (const auto& key : this->forms.keys()) {
        gnomonFormManager::instance()->addForm(this->forms[key], this->export_color, this->formVisualization[key]);
    }
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

    this->sync->move(event->size().width() - 80, 10);
    this->export_button->move(event->size().width() - 40, 10);

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
    if(!this->cameras.contains(this->ori)) {
        vtkSmartPointer<vtkCamera> cam = vtkCamera::New();
        cam->ParallelProjectionOn();
        cam->SetParallelScale(1);
        cam->SetFocalPoint((xBounds[0]+xBounds[1])/2,(yBounds[0]+yBounds[1])/2,(zBounds[0]+zBounds[1])/2);

        switch(this->ori)
        {
            case SLICE_ORIENTATION_XY:
                cam->SetPosition((xBounds[0]+xBounds[1])/2,(yBounds[0]+yBounds[1])/2,zBounds[1]);
                cam->SetViewUp(0,1,0);
                cam->SetClippingRange((zBounds[1] - zBounds[0]) - 3.0, (zBounds[1] - zBounds[0]) + 3.0);
                break;

            case SLICE_ORIENTATION_XZ:
                cam->SetPosition((xBounds[0]+xBounds[1])/2,yBounds[0],(zBounds[0]+zBounds[1])/2);
                cam->SetViewUp(0,0,1);
                cam->SetClippingRange((yBounds[1] - yBounds[0]) - 3.0, (yBounds[1] - yBounds[0]) + 3.0);
                break;

            case SLICE_ORIENTATION_YZ:
                cam->SetPosition(xBounds[1],(yBounds[0]+yBounds[1])/2,(zBounds[0]+zBounds[1])/2);
                cam->SetViewUp(0,0,1);
                cam->SetClippingRange((xBounds[1] - xBounds[0]) - 3.0, (xBounds[1] - xBounds[0]) + 3.0);
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
    this->GetInteractor()->Render();
}

gnomonOverlayPane *gnomonViewFormPrivate::pane(QWidget *parent)
{
    if(!this->formVisualizationPane) {
        this->formVisualizationPane = new gnomonOverlayPane(parent);
    }

    if(!this->renderButton) {
        this->renderButton = new QPushButton("Render",parent);
    }
    this->renderButton->setCheckable(true);

    if(!this->paneItemButton) {
        this->paneItemButton = new gnomonOverlayPaneItem(parent);
    }
    this->paneItemButton->setTitle("View Form");
    this->paneItemButton->addWidget(this->renderButton);
    this->paneItemButton->toggle();

    this->refresh();
    this->formVisualizationPane->toggle();

    return this->formVisualizationPane;
}

void gnomonViewFormPrivate::configure(QWidget *parent, const QString& key)
{
    if (this->formVisualization.contains(key)) {
        gnomonAbstractVisualization *v = this->formVisualization[key];
        if(v) {
            if ((this->parameterLayouts.contains(key))&&(this->parameterLayouts[key])) {
                for(int row = 0, max_row = this->parameterLayouts[key]->count(); row < max_row; ++row) {
                    QLayoutItem *forDeletion = this->parameterLayouts[key]->takeAt(0);
                    forDeletion->widget()->disconnect();
                    delete forDeletion->widget();
                    delete forDeletion;
                }
            } else {
                this->parameterLayouts[key] = new QFormLayout;
                this->parameterLayouts[key]->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
            }

            if ((!this->parameterLayouts.contains(key))||(!this->formVisualizationPaneItems[key])) {
                this->formVisualizationPaneItems[key] = new gnomonOverlayPaneItem(parent);
                this->formVisualizationPaneItems[key]->setTitle(key+" Visualization");
                this->formVisualizationPaneItems[key]->addLayout(this->parameterLayouts[key]);
                this->formVisualizationPaneItems[key]->toggle();
            }

            QMap<QString, gnomonCoreParameter *> parameters = v->parameters();
            for(QMap<QString, gnomonCoreParameter*>::iterator it = parameters.begin(), it_end = parameters.end(); it != it_end; ++it) {
                QWidget *widget = gnomonWidgetsParameter::widget(it.value(), parent);
                if (widget) {
                    this->parameterLayouts[key]->addRow(it.key(), widget);
                }
            }
        }
    }

    this->refresh();
}

void gnomonViewFormPrivate::refresh(void)
{
    this->formVisualizationPane->clearLayout();

    for (const auto& key : this->formVisualizationPaneItems.keys()) {
        if (this->formVisualizationPaneItems[key]) {
            this->formVisualizationPane->addWidget(this->formVisualizationPaneItems[key]);
        }
    }
    this->formVisualizationPane->addWidget(this->paneItemButton);
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

    connect(d->export_button, SIGNAL(iconClicked()), d, SLOT(exportToManager()));

    d->slice_slider = new QSlider(this);
    d->slice_slider->setObjectName("Slice Position");
    d->slice_slider->setOrientation(Qt::Vertical);
    d->slice_slider->setMinimum(0);
    d->slice_slider->setMaximum(1);
    d->slice_slider->setValue(0);

    connect(d->slice_slider, SIGNAL(valueChanged(int)), this, SLOT(sliceChange(int)));

    connect(d, &gnomonViewFormPrivate::sliceOrientationChanged, this, &gnomonViewForm::sliceOrientationChanged);

    QGridLayout *layout  = new QGridLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(d->slice_slider, 0, 0, 1, 1);
    layout->addWidget(d, 0, 1, 1, 1);
    layout->addWidget(d->pane(parent), 0, 2, 1, 1);

    connect(d->sync, &gnomonViewVolumicOverlay::iconClicked, [=] () {
        d->sync->toggle(!d->sync->isToggled());

        if (d->sync->isToggled())
            emit linking();
        else
            emit unlinking();

        if (d->sync->isToggled() && !d->synced) {
            d->syncing_count = 0;
            if(!d->syncing_timer)
                d->syncing_timer = new QTimer(d);
            connect(d->syncing_timer, &QTimer::timeout, [=] () {
                    d->sync->changeColor((d->syncing_count++ % 2) ? Qt::gray : Qt::white);
                    d->sync->update();
                    if (d->syncing_count == 11) {
                        d->sync->toggle(false);
                        d->syncing_timer->stop();
                        d->syncing_timer->disconnect();
                        delete d->syncing_timer;
                        d->syncing_timer = nullptr;
                        emit unlinking();
                    }
                });
            d->syncing_timer->start(500);
        }
    });

    connect(this, &gnomonViewForm::formAdded, [=] (const QString& key) {
        d->configure((QWidget *)this->parent(), key);
    });

    connect(d->renderButton, &QPushButton::clicked, [=] () {
        for (const auto& key : d->formVisualization.keys()) {
            gnomonAbstractVisualization *v = d->formVisualization[key];
            if(v) {
                v->update();
            }
        }
    });

    this->setAcceptDrops(true);
    this->switchTo2D();
    this->switchTo2DXY();
    this->switchTo3D();
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

void gnomonViewForm::link(gnomonViewForm *other)
{
    if (d->syncing_timer)
        d->syncing_timer->stop();

    d->sync->toggle(true);
    d->sync->changeIcon(fa::lock);

    d->synced = true;

    // ///////////////////////////////////////////////////////////////

    d->renderer2D->SetActiveCamera(other->d->renderer2D->GetActiveCamera());
    d->renderer3D->SetActiveCamera(other->d->renderer3D->GetActiveCamera());

    other->d->GetRenderWindow()->AddObserver(vtkCommand::RenderEvent, this, &gnomonViewForm::render);

    connect(other, SIGNAL(switchedTo3D()), this, SLOT(switchTo3D()));
    connect(other, SIGNAL(switchedTo2D()), this, SLOT(switchTo2D()));
    connect(other, SIGNAL(switchedTo2DXY()), this, SLOT(switchTo2DXY()));
    connect(other, SIGNAL(switchedTo2DXZ()), this, SLOT(switchTo2DXZ()));
    connect(other, SIGNAL(switchedTo2DYZ()), this, SLOT(switchTo2DYZ()));
    connect(other, SIGNAL(sliceChanged(int)), this, SLOT(sliceChange(int)));
}

void gnomonViewForm::unlink(gnomonViewForm *other)
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


void gnomonViewForm::setExportColor(const QColor& color)
{
    d->export_color = color;
}

void gnomonViewForm::toggleVisualizationPane(void)
{
    d->formVisualizationPane->toggle();
}

gnomonAbstractForm *gnomonViewForm::form(const QString& name)
{
    if (d->forms.contains(name)) {
        return d->forms[name];
    } else {
        return nullptr;
    }
}


void gnomonViewForm::setForm(const QString& name, gnomonAbstractForm *form, gnomonAbstractVisualization *visualization)
{
    if (gnomonImagesSerie *images_serie = dynamic_cast<gnomonImagesSerie *>(form)) {
        return this->setImagesSerie(images_serie, visualization);
    }
    if (gnomonCellImage *cellImage = dynamic_cast<gnomonCellImage *>(form)) {
        return this->setCellImage(cellImage);
    }
    if (gnomonCellComplex *cellComplex = dynamic_cast<gnomonCellComplex *>(form)) {
        return this->setCellComplex(cellComplex);
    }
    if (gnomonMesh *mesh = dynamic_cast<gnomonMesh *>(form)) {
        return this->setMesh(mesh);
    }
}

gnomonImagesSerie *gnomonViewForm::imagesSerie(void)
{
    if (d->forms.contains("gnomonImagesSerie")) {
        return dynamic_cast<gnomonImagesSerie *>(d->forms["gnomonImagesSerie"]);
    } else {
        return nullptr;
    }
}

void gnomonViewForm::setImagesSerie(gnomonImagesSerie* images_serie, gnomonAbstractVisualization *visualization)
{
    d->forms["gnomonImagesSerie"] = images_serie;

    // bool enable_slider = images_serie->times().count() > 1;
    // d->time_slider->setVisible(enable_slider);

    if ((!d->formVisualization.contains("gnomonImagesSerie"))||(!d->formVisualization["gnomonImagesSerie"])) {
        // d->formVisualization["gnomonImagesSerie"] = new gnomonVisualizationImagesSerie(this);
        d->formVisualization["gnomonImagesSerie"] = new gnomonVisualizationImagesSerieChannelBlending();
        d->formVisualization["gnomonImagesSerie"]->setView(this);
    }
    gnomonVisualizationImagesSerieChannelBlending *formVisualizationImagesSerie = (gnomonVisualizationImagesSerieChannelBlending *)d->formVisualization["gnomonImagesSerie"];
    // gnomonVisualizationImagesSerie *formVisualizationImagesSerie = (gnomonVisualizationImagesSerie *)d->formVisualization["gnomonImagesSerie"];
    formVisualizationImagesSerie->setImagesSerie(images_serie);
    if (visualization) {
        formVisualizationImagesSerie->setParameters(visualization->parameters());
    }
    formVisualizationImagesSerie->update();

    if (d->renderer3D_button->isToggled()) {
        d->renderer3D_button->toggle(false);
        this->switchTo3D();
    }
    else if (d->renderer2D_button->isToggled()) {
        d->renderer2D_button->toggle(false);
        this->switchTo2D();
    }

    emit formAdded("gnomonImagesSerie");
}

gnomonCellImage *gnomonViewForm::cellImage(void)
{
    if (d->forms.contains("gnomonCellImage")) {
        return dynamic_cast<gnomonCellImage *>(d->forms["gnomonCellImage"]);
    } else {
        return nullptr;
    }
}

void gnomonViewForm::setCellImage(gnomonCellImage* cellImage, gnomonAbstractVisualization *visualization)
{
    d->forms["gnomonCellImage"] = cellImage;

    if ((!d->formVisualization.contains("gnomonCellImage"))||(!d->formVisualization["gnomonCellImage"])) {
//        d->formVisualization["gnomonCellImage"] = new gnomonVisualizationCellImage(this);
        d->formVisualization["gnomonCellImage"] = new gnomonVisualizationCellImageMarchingCubes();
        d->formVisualization["gnomonCellImage"]->setView(this);
    }
//    gnomonVisualizationCellImage *formVisualizationCellImage = (gnomonVisualizationCellImage *)d->formVisualization["gnomonCellImage"];
    gnomonVisualizationCellImageMarchingCubes *formVisualizationCellImage = (gnomonVisualizationCellImageMarchingCubes *)d->formVisualization["gnomonCellImage"];
    formVisualizationCellImage->setCellImage(cellImage);
    if (visualization) {
        formVisualizationCellImage->setParameters(visualization->parameters());
    }
    formVisualizationCellImage->update();

    if (d->renderer3D_button->isToggled()) {
        d->renderer3D_button->toggle(false);
        this->switchTo3D();
    }
    else if (d->renderer2D_button->isToggled()) {
        d->renderer2D_button->toggle(false);
        this->switchTo2D();
    }

    emit formAdded("gnomonCellImage");
}

gnomonCellComplex *gnomonViewForm::cellComplex(void)
{
    if (d->forms.contains("gnomonCellComplex")) {
        return dynamic_cast<gnomonCellComplex *>(d->forms["gnomonCellComplex"]);
    } else {
        return nullptr;
    }
}

void gnomonViewForm::setCellComplex(gnomonCellComplex *cellComplex, gnomonAbstractVisualization *visualization)
{
    d->forms["gnomonCellComplex"] = cellComplex;

    if ((!d->formVisualization.contains("gnomonCellComplex"))||(!d->formVisualization["gnomonCellComplex"]))
    {
        d->formVisualization["gnomonCellComplex"] = new gnomonVisualizationCellComplex();
        d->formVisualization["gnomonCellComplex"]->setView(this);
    }
    gnomonAbstractVisualizationCellComplex *formVisualizationCellComplex = (gnomonAbstractVisualizationCellComplex *)d->formVisualization["gnomonCellComplex"];
    formVisualizationCellComplex->setCellComplex(cellComplex);
    if (visualization) {
        formVisualizationCellComplex->setParameters(visualization->parameters());
    }
    formVisualizationCellComplex->update();

    if (d->renderer3D_button->isToggled()) {
        d->renderer3D_button->toggle(false);
        this->switchTo3D();
    }
    else if (d->renderer2D_button->isToggled()) {
        d->renderer2D_button->toggle(false);
        this->switchTo2D();
    }

    emit formAdded("gnomonCellComplex");
}

gnomonMesh *gnomonViewForm::mesh(void)
{
    if (d->forms.contains("gnomonMesh")) {
        return dynamic_cast<gnomonMesh *>(d->forms["gnomonMesh"]);
    } else {
        return nullptr;
    }
}

void gnomonViewForm::setMesh(gnomonMesh *mesh, gnomonAbstractVisualization *visualization)
{
    d->forms["gnomonMesh"] = mesh;

    if ((!d->formVisualization.contains("gnomonMesh"))||(!d->formVisualization["gnomonMesh"]))
    {
        d->formVisualization["gnomonMesh"] = new gnomonVisualizationMesh();
        d->formVisualization["gnomonMesh"]->setView(this);
    }
    gnomonVisualizationMesh *formVisualizationMesh = (gnomonVisualizationMesh *)d->formVisualization["gnomonMesh"];
    formVisualizationMesh->setMesh(mesh);
    if (visualization) {
        formVisualizationMesh->setParameters(visualization->parameters());
    }
    formVisualizationMesh->update();

    if (d->renderer3D_button->isToggled()) {
        d->renderer3D_button->toggle(false);
        this->switchTo3D();
    }
    else if (d->renderer2D_button->isToggled()) {
        d->renderer2D_button->toggle(false);
        this->switchTo2D();
    }

    emit formAdded("gnomonMesh");
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

void gnomonViewForm::setAcceptCellComplex(bool accept)
{
    d->acceptCellComplex = accept;
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
        gnomonAbstractForm *form = gnomonFormManager::instance()->get(path.remove(":").toInt());
        this->setForm("formManager", form, gnomonFormManager::instance()->getVisualization(path.remove(":").toInt()));
        // gnomonImagesSerie * images_serie = gnomonImageManager::instance()->get(path.remove(":").toInt());
        // emit channelsChanged(images_serie->channels());
        // this->setImagesSerie(images_serie);
    } else {
        if((path.endsWith("inr") || path.endsWith("inr.gz") || path.endsWith("tif"))&&(path.contains("seg",Qt::CaseInsensitive))) {
            if ((!d->formReaderCommand.contains("gnomonCellImage"))||(!d->formReaderCommand["gnomonCellImage"]))
                d->formReaderCommand["gnomonCellImage"] = new gnomonCellImageReaderCommand("gnomonCellImageReaderPropertySpatialImage");
            gnomonCellImageReaderCommand *cellImageCommand = (gnomonCellImageReaderCommand *) d->formReaderCommand["gnomonCellImage"];
            cellImageCommand->setPath(path.remove("file://"));
            cellImageCommand->redo();

            gnomonCellImage * cellImage = (gnomonCellImage *) cellImageCommand->cellImage()->clone();
            if (!cellImage) {
                qWarning() << Q_FUNC_INFO << "Resulting cell image is void.";
                event->ignore();
                return;
            }
            this->setForm("gnomonCellImage",cellImage);

        } else if(path.endsWith("inr") || path.endsWith("inr.gz") || path.endsWith("mha") || path.endsWith("tif") || (path.endsWith("czi"))) {
            if ((!d->formReaderCommand.contains("gnomonImagesSerie"))||(!d->formReaderCommand["gnomonImagesSerie"]))
                d->formReaderCommand["gnomonImagesSerie"] = new gnomonImagesSerieReaderCommand("gnomonImagesSerieReader");
            gnomonImagesSerieReaderCommand *imageCommand = (gnomonImagesSerieReaderCommand *) d->formReaderCommand["gnomonImagesSerie"];
            imageCommand->setPath(path.remove("file://"));
            imageCommand->redo();

            gnomonImagesSerie * images_serie = imageCommand->imagesSerie()->copy();
            if (!images_serie) {
                qWarning() << Q_FUNC_INFO << "Resulting image series is void.";
                event->ignore();
                return;
            }
            // emit channelsChanged(images_serie->channels());
            // emit timeChanged(images_serie->time());
            this->setForm("gnomonImagesSerie",images_serie);

        } else if((path.endsWith("ply")) and (d->acceptCellComplex)) {
            if ((!d->formReaderCommand.contains("gnomonCellComplex"))||(!d->formReaderCommand["gnomonCellComplex"]))
                d->formReaderCommand["gnomonCellComplex"] = new gnomonCellComplexReaderCommand("gnomonCellComplexReaderPropertyTopomesh");
            gnomonCellComplexReaderCommand *cellComplexCommand = (gnomonCellComplexReaderCommand *) d->formReaderCommand["gnomonCellComplex"];
            qDebug()<<Q_FUNC_INFO<<path.remove("file://");
            cellComplexCommand->setPath(path.remove("file://"));
            cellComplexCommand->redo();
            qDebug()<<Q_FUNC_INFO<<cellComplexCommand;

            gnomonCellComplex *cellComplex = (gnomonCellComplex *) cellComplexCommand->cellComplex()->clone();
            if (!cellComplex) {
                qWarning() << Q_FUNC_INFO << "Resulting cellComplex is void.";
                event->ignore();
                return;
            }
            this->setForm("gnomonCellComplex",cellComplex);
        } else if(path.endsWith("ply")) {
            if ((!d->formReaderCommand.contains("gnomonMesh"))||(!d->formReaderCommand["gnomonMesh"]))
                d->formReaderCommand["gnomonMesh"] = new gnomonMeshReaderCommand("gnomonMeshReaderPropertyTopomesh");
            gnomonMeshReaderCommand *meshCommand = (gnomonMeshReaderCommand *) d->formReaderCommand["gnomonMesh"];
            meshCommand->setPath(path.remove("file://"));
            meshCommand->redo();

            gnomonMesh *mesh = (gnomonMesh *) meshCommand->mesh()->clone();
            if (!mesh) {
                qWarning() << Q_FUNC_INFO << "Resulting mesh is void.";
                event->ignore();
                return;
            }
            this->setForm("gnomonMesh",mesh);
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
