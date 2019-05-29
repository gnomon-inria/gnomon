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

#include <gnomonCore/gnomonCommand/gnomonAbstractCommand>
#include <gnomonCore/gnomonCommand/gnomonImagesSerie/gnomonImagesSerieReaderCommand>
#include <gnomonCore/gnomonCommand/gnomonCellImage/gnomonCellImageReaderCommand>
#include <gnomonCore/gnomonCommand/gnomonCellComplex/gnomonCellComplexReaderCommand>
#include <gnomonCore/gnomonCommand/gnomonImage/gnomonImageReaderCommand>
#include <gnomonCore/gnomonCommand/gnomonMesh/gnomonMeshReaderCommand>

#include <dtkImagingCore>
#include <dtkScript>

#include <gnomonCore>
#include <gnomonWidgets>

#include "gnomonVisualizations/gnomonCellComplex/gnomonAbstractVisualizationCellComplex.h"
#include "gnomonVisualizations/gnomonCellImage/gnomonAbstractVisualizationCellImage.h"
#include "gnomonVisualizations/gnomonImage/gnomonAbstractVisualizationImage.h"
#include "gnomonVisualizations/gnomonImagesSerie/gnomonAbstractVisualizationImagesSerie.h"
#include "gnomonVisualizations/gnomonMesh/gnomonAbstractVisualizationMesh.h"
#include "gnomonVisualizations/gnomonPointCloud/gnomonAbstractVisualizationPointCloud.h"

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
    void updateTimeSlider(void);

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
    QMap<QString, gnomonAbstractDynamicForm *> forms;
    QMap<QString, gnomonAbstractVisualization *> formVisualization;
    QMap<QString, gnomonAbstractCommand *> formReaderCommand;

public:
    gnomonOverlayButton *renderer2D_button = nullptr;
    gnomonOverlayButton *renderer3D_button = nullptr;
    gnomonOverlayButton *renderer2D_XY = nullptr;
    gnomonOverlayButton *renderer2D_XZ = nullptr;
    gnomonOverlayButton *renderer2D_YZ = nullptr;

    gnomonOverlayButton *sync = nullptr;
    gnomonOverlayButton *export_button = nullptr;

public:
    int syncing_count = 0;
    QTimer *syncing_timer = nullptr;
    bool synced = false;

public:
    bool acceptCellComplex = true;

public:
    QColor export_color = QColor("#cccccc");

public:
    QSlider *slice_slider = nullptr;

public:
    QSlider *time_slider = nullptr;
    QSet<double> forms_times;

public:
    gnomonOverlayPaneItem *paneItemButton = nullptr;
    QPushButton *renderButton = nullptr;
    QPushButton *clearButton = nullptr;

    QMap<QString, QFormLayout *> parameterLayouts;
    QMap<QString, gnomonOverlayPaneItem *> formVisualizationPaneItems;

    gnomonOverlayPane *formVisualizationPane = nullptr;

public:
    gnomonOverlayPane *infoPane = nullptr;

public:
    double xBounds[2] = {0,0}, yBounds[2] = {0,0}, zBounds[2] = {0,0};
    double c_x = 0, c_y = 0, c_z = 0;

public:
    double c_t = 0;

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

    this->renderer2D_button = new gnomonOverlayButton(fa::square, "", this);
    this->renderer2D_button->toggle(false);
    this->renderer3D_button = new gnomonOverlayButton(fa::cube, "", this);
    this->renderer2D_XY = new gnomonOverlayButton(":gnomon/gnomonButton-XY.png", ":gnomon/gnomonButton-XY-off.png", "", this);
    this->renderer2D_XY->toggle(false);
    this->renderer2D_XZ = new gnomonOverlayButton(":gnomon/gnomonButton-XZ.png", ":gnomon/gnomonButton-XZ-off.png", "", this);
    this->renderer2D_XZ->toggle(false);
    this->renderer2D_YZ = new gnomonOverlayButton(":gnomon/gnomonButton-YZ.png",  ":gnomon/gnomonButton-YZ-off.png", "", this);
    this->renderer2D_YZ->toggle(false);

    this->sync = new gnomonOverlayButton(fa::unlock, "", this);
    this->sync->toggle(false);

    this->export_button = new gnomonOverlayButton(fa::arrowcircleup, "", this);
}

gnomonViewFormPrivate::~gnomonViewFormPrivate(void)
{
}

void gnomonViewFormPrivate::exportToManager(void)
{
    for (const auto& key : this->forms.keys()) {
        gnomonFormManager::instance()->addForm(this->forms[key]->current(), this->export_color, this->formVisualization[key]);
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
        qDebug()<<Q_FUNC_INFO<<"Updating camera"<<ori;
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

    if(!this->clearButton) {
        this->clearButton = new QPushButton("Clear", parent);
    }

    this->renderButton->setCheckable(true);
    this->clearButton->setCheckable(true);

    if(!this->paneItemButton) {
        this->paneItemButton = new gnomonOverlayPaneItem(parent);
    }
    this->paneItemButton->setTitle("View Form");
    this->paneItemButton->addWidget(this->renderButton);
    this->paneItemButton->addWidget(this->clearButton);
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
            qDebug()<<Q_FUNC_INFO<<key;
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

            if (!this->formVisualizationPaneItems.contains(key)) {
                this->refresh();
            }
            this->formVisualizationPaneItems[key]->addLayout(this->parameterLayouts[key]);

            qDebug()<<Q_FUNC_INFO<<v;
            QMap<QString, gnomonCoreParameter *> parameters = v->parameters();
            qDebug()<<Q_FUNC_INFO<<parameters;
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

    for (const auto& key : this->formVisualization.keys()) {

        if ((!this->formVisualizationPaneItems.contains(key))||(!this->formVisualizationPaneItems[key])) {
            this->formVisualizationPaneItems[key] = new gnomonOverlayPaneItem((QWidget *) q->parent());
            this->formVisualizationPaneItems[key]->setTitle(key+" Visualization");

            QComboBox *combo_box = new QComboBox(this);
            QStringList combo_box_keys = {};
            if (key == "gnomonCellComplex") {
                combo_box_keys = gnomonVisualization::visualizationCellComplex::pluginFactory().keys();
            } else if (key == "gnomonCellImage") {
                combo_box_keys = gnomonVisualization::visualizationCellImage::pluginFactory().keys();
            } else if (key == "gnomonImage") {
                combo_box_keys = gnomonVisualization::visualizationImage::pluginFactory().keys();
            } else if (key == "gnomonImagesSerie") {
                combo_box_keys = gnomonVisualization::visualizationImagesSerie::pluginFactory().keys();
            } else if (key == "gnomonMesh") {
                combo_box_keys = gnomonVisualization::visualizationMesh::pluginFactory().keys();
            } else if (key == "gnomonPointCloud") {
                combo_box_keys = gnomonVisualization::visualizationPointCloud::pluginFactory().keys();
            }
            for (auto it = combo_box_keys.begin(), it_end = combo_box_keys.end(); it != it_end; ++it) {
                combo_box->addItem(*it);
            }
            combo_box->model()->sort(0);

            QObject::connect(combo_box, &QComboBox::currentTextChanged, [=] (const QString& visu) {
                q->switchTo3D();
                qDebug()<<"Visualization changed"<<visu;
                if (this->formVisualization[key]) {
                    delete this->formVisualization[key];
                    this->formVisualization[key] = nullptr;
                }
                if (key == "gnomonCellComplex") {
                    this->formVisualization[key] = gnomonVisualization::visualizationCellComplex::pluginFactory().create(visu);
                    this->formVisualization[key]->setView(q);
                    gnomonAbstractVisualizationCellComplex *formVisualizationCellComplex = (gnomonAbstractVisualizationCellComplex *)this->formVisualization[key];
                    gnomonCellComplexSeries *cellComplex = (gnomonCellComplexSeries *)this->forms[key];
                    formVisualizationCellComplex->setCellComplex(cellComplex);
                    formVisualizationCellComplex->update();
                } else if (key == "gnomonCellImage") {
                    this->formVisualization[key] = gnomonVisualization::visualizationCellImage::pluginFactory().create(visu);
                    this->formVisualization[key]->setView(q);
                    gnomonAbstractVisualizationCellImage *formVisualizationCellImage = (gnomonAbstractVisualizationCellImage *)this->formVisualization[key];
                    gnomonCellImageSeries *cellImage = (gnomonCellImageSeries *)this->forms[key];
                    formVisualizationCellImage->setCellImage(cellImage);
                    formVisualizationCellImage->update();
                } else if (key == "gnomonImage") {
                    this->formVisualization[key] = gnomonVisualization::visualizationImage::pluginFactory().create(visu);
                    this->formVisualization[key]->setView(q);
                    gnomonAbstractVisualizationImage *formVisualizationImage = (gnomonAbstractVisualizationImage *)this->formVisualization[key];
                    gnomonImageSeries *image = (gnomonImageSeries *)this->forms[key];
                    formVisualizationImage->setImage(image);
                    formVisualizationImage->update();
                } else if (key == "gnomonImagesSerie") {
                    this->formVisualization[key] = gnomonVisualization::visualizationImagesSerie::pluginFactory().create(visu);
                    this->formVisualization[key]->setView(q);
                    gnomonAbstractVisualizationImagesSerie *formVisualizationImagesSerie = (gnomonAbstractVisualizationImagesSerie *)this->formVisualization[key];
                    gnomonImagesSerie *imagesSerie = (gnomonImagesSerie *)this->forms[key];
                    formVisualizationImagesSerie->setImagesSerie(imagesSerie);
                    formVisualizationImagesSerie->update();
                } else if (key == "gnomonMesh") {
                    this->formVisualization[key] = gnomonVisualization::visualizationMesh::pluginFactory().create(visu);
                    this->formVisualization[key]->setView(q);
                    gnomonAbstractVisualizationMesh *formVisualizationMesh = (gnomonAbstractVisualizationMesh *)this->formVisualization[key];
                    gnomonMeshSeries *mesh = (gnomonMeshSeries *)this->forms[key];
                    formVisualizationMesh->setMesh((gnomonMeshSeries *)mesh->current());
                    formVisualizationMesh->update();
                } else if (key == "gnomonPointCloud") {
                    this->formVisualization[key] = gnomonVisualization::visualizationPointCloud::pluginFactory().create(visu);
                    this->formVisualization[key]->setView(q);
                    gnomonAbstractVisualizationPointCloud *formVisualizationPointCloud = (gnomonAbstractVisualizationPointCloud *)this->formVisualization[key];
                    gnomonPointCloudSeries *pointCloud = (gnomonPointCloudSeries *)this->forms[key];
                    formVisualizationPointCloud->setPointCloud(pointCloud);
                    formVisualizationPointCloud->update();
                }
                this->configure((QWidget *) q->parent(), key);
            });

            this->formVisualizationPaneItems[key]->addWidget(combo_box);
            this->formVisualizationPaneItems[key]->toggle();
        }

        this->formVisualizationPane->addWidget(this->formVisualizationPaneItems[key]);
    }
    this->formVisualizationPane->addWidget(this->paneItemButton);
}

void gnomonViewFormPrivate::updateTimeSlider(void)
{
    qDebug()<<Q_FUNC_INFO<<"Times :"<<forms_times;

    if(this->forms_times.size() < 2) {
        this->time_slider->setVisible(false);
        return;
    }
    this->time_slider->setVisible(true);


    this->time_slider->setOrientation(Qt::Horizontal);
    this->time_slider->setMinimum(0);
    this->time_slider->setMaximum(this->forms_times.size()-1);
}

// ///////////////////////////////////////////////////////////////////
// gnomonViewForm
// ///////////////////////////////////////////////////////////////////

gnomonViewForm::gnomonViewForm(QWidget *parent) : QFrame(parent)
{
    d = new gnomonViewFormPrivate;
    d->q = this;

    int stat;
    dtkScriptInterpreterPython::instance()->interpret("import gnomonVisualizationCellComplex", &stat);
    dtkScriptInterpreterPython::instance()->interpret("import gnomonVisualizationPointCloud", &stat);

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

    d->infoPane = new gnomonOverlayPane(parent);
    d->infoPane->toggle();

    connect(d->slice_slider, SIGNAL(valueChanged(int)), this, SLOT(sliceChange(int)));

    connect(d, &gnomonViewFormPrivate::sliceOrientationChanged, this, &gnomonViewForm::sliceOrientationChanged);

    d->time_slider = new QSlider(this);
    d->time_slider->setObjectName("Time Point");
    d->time_slider->setOrientation(Qt::Horizontal);
    d->time_slider->setMinimum(0);
    d->time_slider->setPageStep(1);
    d->time_slider->setMaximum(1);
    d->time_slider->setValue(0);
    d->time_slider->setEnabled(true);
    d->time_slider->setVisible(true);
    d->time_slider->setTickPosition(QSlider::TicksAbove);

    connect(d->time_slider, SIGNAL(valueChanged(int)), this, SLOT(timeIndexChange(int)));
//    connect(d->time_slider, &QSlider::valueChanged, [=] (int value) {
//        this->timeChange((double) value);
//    });

    QGridLayout *layout  = new QGridLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(d->slice_slider, 0, 0, 1, 1);
    layout->addWidget(d->infoPane, 0, 1, 1, 1);
    layout->addWidget(d, 0, 2, 1, 1);
    layout->addWidget(d->pane(parent), 0, 3, 1, 1);
    layout->addWidget(d->time_slider, 1, 0, 1, 3);

    connect(d->sync, &gnomonOverlayButton::iconClicked, [=] () {
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

    connect(d->clearButton, &QPushButton::clicked, [=] () {
        d->formVisualizationPane->clearLayout();

        for (const auto& key : d->formVisualization.keys()) {
            d->formVisualization[key]->disconnect();
            d->formVisualization[key]->clear();
            delete d->formVisualization[key];
            d->parameterLayouts[key]->disconnect();
            delete d->parameterLayouts[key];
            d->formVisualizationPaneItems[key]->disconnect();
            delete d->formVisualizationPaneItems[key];
        }
        d->formVisualization.clear();
        d->forms.clear();
        d->parameterLayouts.clear();
        d->formVisualizationPaneItems.clear();

        d->formVisualizationPane->addWidget(d->paneItemButton);

        this->render();
    });

    this->setAcceptDrops(true);
    this->switchTo2D();
    this->switchTo2DXY();
    this->switchTo3D();
    d->updateOrientation();
    d->updateTimeSlider();
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

    qDebug()<<Q_FUNC_INFO;

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
    emit sliceOrientationChanged(gnomonViewFormPrivate::SLICE_ORIENTATION_XY);

    d->slice_slider->blockSignals(true);
    d->slice_slider->setMinimum(d->zBounds[0]);
    d->slice_slider->setMaximum(d->zBounds[1]);
    d->slice_slider->blockSignals(false);
    d->slice_slider->setValue(d->c_z);
    emit sliceChanged(d->c_z);

    d->setSliceOrientation(gnomonViewFormPrivate::SLICE_ORIENTATION_XY);

    d->renderer2D_XY->toggle(true);
    d->renderer2D_XZ->toggle(false);
    d->renderer2D_YZ->toggle(false);

    emit switchedTo2DXY();
}

void gnomonViewForm::switchTo2DXZ(void)
{
    emit sliceOrientationChanged(gnomonViewFormPrivate::SLICE_ORIENTATION_XZ);

    d->slice_slider->blockSignals(true);
    d->slice_slider->setMinimum(d->yBounds[0]);
    d->slice_slider->setMaximum(d->yBounds[1]);
    d->slice_slider->blockSignals(false);
    d->slice_slider->setValue(d->c_y);
    emit sliceChanged(d->c_y);

    d->setSliceOrientation(gnomonViewFormPrivate::SLICE_ORIENTATION_XZ);

    d->renderer2D_XY->toggle(false);
    d->renderer2D_XZ->toggle(true);
    d->renderer2D_YZ->toggle(false);

    emit switchedTo2DXZ();
}

void gnomonViewForm::switchTo2DYZ(void)
{
    emit sliceOrientationChanged(gnomonViewFormPrivate::SLICE_ORIENTATION_YZ);

    d->slice_slider->blockSignals(true);
    d->slice_slider->setMinimum(d->xBounds[0]);
    d->slice_slider->setMaximum(d->xBounds[1]);
    d->slice_slider->blockSignals(false);
    d->slice_slider->setValue(d->c_x);
    emit sliceChanged(d->c_x);

    d->setSliceOrientation(gnomonViewFormPrivate::SLICE_ORIENTATION_YZ);

    d->renderer2D_XY->toggle(false);
    d->renderer2D_XZ->toggle(false);
    d->renderer2D_YZ->toggle(true);

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


void gnomonViewForm::timeIndexChange(int value)
{

    QList<double> sorted_times = QList<double>::fromSet(d->forms_times);
    qSort(sorted_times);
    qDebug()<<sorted_times;

    double time = sorted_times[value];

    bool valueChanged = false;
    if (d->c_t != time) {
        valueChanged = true;
        d->c_t = time;
    }

    d->time_slider->blockSignals(true);
    d->time_slider->setValue(value);
    d->time_slider->setToolTip(QString("current time: %1").arg(time));
    d->time_slider->blockSignals(false);

    if (valueChanged) {
        qDebug()<<Q_FUNC_INFO<<"Time Changed !"<<time;
        emit timeChanged(time);
    }

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
        return d->forms[name]->current();
    } else {
        return nullptr;
    }
}

void gnomonViewForm::setForm(const QString& name, gnomonAbstractDynamicForm *form, gnomonAbstractVisualization *visualization)
{
    qDebug()<<Q_FUNC_INFO<<name<<form;
    if (gnomonImagesSerie *images_serie = dynamic_cast<gnomonImagesSerie *>(form)) {
        this->setImagesSerie(images_serie, visualization);
    } else if (gnomonCellImageSeries *cellImage = dynamic_cast<gnomonCellImageSeries *>(form)) {
        this->setCellImage(cellImage);
    } else if (gnomonCellComplexSeries *cellComplex = dynamic_cast<gnomonCellComplexSeries *>(form)) {
        this->setCellComplex(cellComplex);
    } else if (gnomonImageSeries *image = dynamic_cast<gnomonImageSeries *>(form)) {
        this->setImage(image);
    } else if (gnomonMeshSeries *mesh = dynamic_cast<gnomonMeshSeries *>(form)) {
        this->setMesh(mesh);
    } else if (gnomonPointCloudSeries *pointCloud = dynamic_cast<gnomonPointCloudSeries *>(form)) {
        this->setPointCloud(pointCloud);
    }

    d->forms_times.clear();
    for (const auto& key : d->forms.keys()) {
        for(auto time : d->forms[key]->times()) {
            d->forms_times.insert(time);
        }
    }
    d->updateTimeSlider();

    return;
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
//    d->forms["gnomonImagesSerie"] = images_serie;

    // bool enable_slider = images_serie->times().count() > 1;
    // d->time_slider->setVisible(enable_slider);

    qDebug()<<Q_FUNC_INFO<<gnomonVisualization::visualizationImagesSerie::pluginFactory().keys();
    QString key = gnomonVisualization::visualizationImagesSerie::pluginFactory().keys()[0];

    if ((!d->formVisualization.contains("gnomonImagesSerie"))||(!d->formVisualization["gnomonImagesSerie"]))
    {
        d->formVisualization["gnomonImagesSerie"] = gnomonVisualization::visualizationImagesSerie::pluginFactory().create(key);
        d->formVisualization["gnomonImagesSerie"]->setView(this);
    }

    gnomonAbstractVisualizationImagesSerie *formVisualizationImagesSerie = (gnomonAbstractVisualizationImagesSerie *)d->formVisualization["gnomonImagesSerie"];
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

gnomonCellImageSeries *gnomonViewForm::cellImage(void)
{
    if (d->forms.contains("gnomonCellImage")) {
        return dynamic_cast<gnomonCellImageSeries *>(d->forms["gnomonCellImage"]);
    } else {
        return nullptr;
    }
}

void gnomonViewForm::setCellImage(gnomonCellImageSeries* cellImage, gnomonAbstractVisualization *visualization)
{
    d->forms["gnomonCellImage"] = cellImage;

    qDebug()<<Q_FUNC_INFO<<gnomonVisualization::visualizationCellImage::pluginFactory().keys();
    QString key = gnomonVisualization::visualizationCellImage::pluginFactory().keys()[0];

    if ((!d->formVisualization.contains("gnomonCellImage"))||(!d->formVisualization["gnomonCellImage"]))
    {
        d->formVisualization["gnomonCellImage"] = gnomonVisualization::visualizationCellImage::pluginFactory().create(key);
        d->formVisualization["gnomonCellImage"]->setView(this);
    }

    gnomonAbstractVisualizationCellImage *formVisualizationCellImage = (gnomonAbstractVisualizationCellImage *)d->formVisualization["gnomonCellImage"];
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

gnomonCellComplexSeries *gnomonViewForm::cellComplex(void)
{
    if (d->forms.contains("gnomonCellComplex")) {
        return dynamic_cast<gnomonCellComplexSeries *>(d->forms["gnomonCellComplex"]);
    } else {
        return nullptr;
    }
}

void gnomonViewForm::setCellComplex(gnomonCellComplexSeries *cellComplex, gnomonAbstractVisualization *visualization)
{
    d->forms["gnomonCellComplex"] = cellComplex;

    qDebug()<<Q_FUNC_INFO<<gnomonVisualization::visualizationCellComplex::pluginFactory().keys();
    QString key = gnomonVisualization::visualizationCellComplex::pluginFactory().keys()[0];
//    QString key = "gnomonVisualizationCellComplexTriangularMesh";

    if ((!d->formVisualization.contains("gnomonCellComplex"))||(!d->formVisualization["gnomonCellComplex"]))
    {
//        d->formVisualization["gnomonCellComplex"] = new gnomonVisualizationCellComplex();
        d->formVisualization["gnomonCellComplex"] = gnomonVisualization::visualizationCellComplex::pluginFactory().create(key);
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


gnomonImageSeries *gnomonViewForm::image(void)
{
    if (d->forms.contains("gnomonImage")) {
        return dynamic_cast<gnomonImageSeries *>(d->forms["gnomonImage"]);
    } else {
        return nullptr;
    }
}

void gnomonViewForm::setImage(gnomonImageSeries* image, gnomonAbstractVisualization *visualization)
{
    d->forms["gnomonImage"] = image;

    qDebug()<<Q_FUNC_INFO<<gnomonVisualization::visualizationImage::pluginFactory().keys();
    QString key = gnomonVisualization::visualizationImage::pluginFactory().keys()[0];

    if ((!d->formVisualization.contains("gnomonImage"))||(!d->formVisualization["gnomonImage"]))
    {
        d->formVisualization["gnomonImage"] = gnomonVisualization::visualizationImage::pluginFactory().create(key);
        d->formVisualization["gnomonImage"]->setView(this);
    }

    gnomonAbstractVisualizationImage *formVisualizationImage = (gnomonAbstractVisualizationImage *)d->formVisualization["gnomonImage"];

    qDebug()<<Q_FUNC_INFO<<formVisualizationImage;
    formVisualizationImage->setImage(image);
    if (visualization) {
        formVisualizationImage->setParameters(visualization->parameters());
    }
    qDebug()<<Q_FUNC_INFO<<"Set Image OK";
    formVisualizationImage->update();
    qDebug()<<Q_FUNC_INFO<<"Update OK";

    if (d->renderer3D_button->isToggled()) {
        d->renderer3D_button->toggle(false);
        this->switchTo3D();
    }
    else if (d->renderer2D_button->isToggled()) {
        d->renderer2D_button->toggle(false);
        this->switchTo2D();
    }

    emit formAdded("gnomonImage");
}

gnomonMeshSeries *gnomonViewForm::mesh(void)
{
    if (d->forms.contains("gnomonMesh")) {
        return dynamic_cast<gnomonMeshSeries *>(d->forms["gnomonMesh"]);
    } else {
        return nullptr;
    }
}

void gnomonViewForm::setMesh(gnomonMeshSeries *mesh, gnomonAbstractVisualization *visualization)
{
    d->forms["gnomonMesh"] = mesh;

    qDebug()<<Q_FUNC_INFO<<gnomonVisualization::visualizationMesh::pluginFactory().keys();
    QString key = gnomonVisualization::visualizationMesh::pluginFactory().keys()[0];

    if ((!d->formVisualization.contains("gnomonMesh"))||(!d->formVisualization["gnomonMesh"]))
    {
        d->formVisualization["gnomonMesh"] = gnomonVisualization::visualizationMesh::pluginFactory().create(key);
        d->formVisualization["gnomonMesh"]->setView(this);
    }

    gnomonAbstractVisualizationMesh *formVisualizationMesh = (gnomonAbstractVisualizationMesh *)d->formVisualization["gnomonMesh"];
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

gnomonPointCloud *gnomonViewForm::pointCloud(void)
{
    if (d->forms.contains("gnomonPointCloud")) {
        return dynamic_cast<gnomonPointCloud *>(d->forms["gnomonPointCloud"]);
    } else {
        return nullptr;
    }
}

void gnomonViewForm::setPointCloud(gnomonPointCloudSeries *pointCloud, gnomonAbstractVisualization *visualization)
{
    d->forms["gnomonPointCloud"] = pointCloud;

    qDebug()<<Q_FUNC_INFO<<gnomonVisualization::visualizationPointCloud::pluginFactory().keys();
    QString key = gnomonVisualization::visualizationPointCloud::pluginFactory().keys()[0];

    if ((!d->formVisualization.contains("gnomonPointCloud"))||(!d->formVisualization["gnomonPointCloud"]))
    {
        d->formVisualization["gnomonPointCloud"] = gnomonVisualization::visualizationPointCloud::pluginFactory().create(key);
        d->formVisualization["gnomonPointCloud"]->setView(this);
    }

    gnomonAbstractVisualizationPointCloud *formVisualizationPointCloud = (gnomonAbstractVisualizationPointCloud *)d->formVisualization["gnomonPointCloud"];
    formVisualizationPointCloud->setPointCloud(pointCloud);
    if (visualization) {
        formVisualizationPointCloud->setParameters(visualization->parameters());
    }
    formVisualizationPointCloud->update();

    if (d->renderer3D_button->isToggled()) {
        d->renderer3D_button->toggle(false);
        this->switchTo3D();
    }
    else if (d->renderer2D_button->isToggled()) {
        d->renderer2D_button->toggle(false);
        this->switchTo2D();
    }

    emit formAdded("gnomonPointCloud");
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
//    d->renderer3D->ResetCamera();

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

gnomonOverlayPane *gnomonViewForm::infoPane(void)
{
    return d->infoPane;
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

void gnomonViewForm::onTimeChanged(double time)
{
    QList<double> sorted_times = QList<double>::fromSet(d->forms_times);

    if (sorted_times.contains(time)) {
        qSort(sorted_times);
        int value = sorted_times.indexOf(time);
        d->time_slider->setValue(value);
    }
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
//        this->setForm("formManager", form, gnomonFormManager::instance()->getVisualization(path.remove(":").toInt()));
        // gnomonImagesSerie * images_serie = gnomonImageManager::instance()->get(path.remove(":").toInt());
        // emit channelsChanged(images_serie->channels());
        // this->setImagesSerie(images_serie);
    } else {
        if((path.endsWith("inr") || path.endsWith("inr.gz") || path.endsWith("mha") || path.endsWith("mha.gz")  || path.endsWith("tif"))&&(path.contains("seg",Qt::CaseInsensitive))) {
            if ((!d->formReaderCommand.contains("gnomonCellImage"))||(!d->formReaderCommand["gnomonCellImage"]))
                d->formReaderCommand["gnomonCellImage"] = new gnomonCellImageReaderCommand("gnomonCellImageReaderPropertySpatialImage");
            gnomonCellImageReaderCommand *cellImageCommand = (gnomonCellImageReaderCommand *) d->formReaderCommand["gnomonCellImage"];
            cellImageCommand->setPath(path.remove("file://"));
            cellImageCommand->redo();

            gnomonCellImageSeries * cellImage = (gnomonCellImageSeries *) cellImageCommand->cellImage()->clone();
            if (!cellImage) {
                qWarning() << Q_FUNC_INFO << "Resulting cell image is void.";
                event->ignore();
                return;
            }
            this->setForm("gnomonCellImage",(gnomonTimeSeries<gnomonAbstractForm> *) cellImage);

        } else if(path.endsWith("inr") || path.endsWith("inr.gz") || path.endsWith("mha") || path.endsWith("mha.gz") || path.endsWith("tif") || (path.endsWith("czi"))) {
            if ((!d->formReaderCommand.contains("gnomonImage"))||(!d->formReaderCommand["gnomonImage"]))
                d->formReaderCommand["gnomonImage"] = new gnomonImageReaderCommand("gnomonImageReader");
            gnomonImageReaderCommand *imageCommand = (gnomonImageReaderCommand *) d->formReaderCommand["gnomonImage"];
            imageCommand->setPath(path.remove("file://"));
            imageCommand->redo();

            gnomonImageSeries * images_serie = (gnomonImageSeries *) imageCommand->image()->clone();
            if (!images_serie) {
                qWarning() << Q_FUNC_INFO << "Resulting image series is void.";
                event->ignore();
                return;
            }
            // emit channelsChanged(images_serie->channels());
            // emit timeChanged(images_serie->time());
            this->setForm("gnomonImage",images_serie);

        } else if((path.endsWith("ply")) and (d->acceptCellComplex)) {
            if ((!d->formReaderCommand.contains("gnomonCellComplex"))||(!d->formReaderCommand["gnomonCellComplex"]))
                d->formReaderCommand["gnomonCellComplex"] = new gnomonCellComplexReaderCommand("gnomonCellComplexReaderPropertyTopomesh");
            gnomonCellComplexReaderCommand *cellComplexCommand = (gnomonCellComplexReaderCommand *) d->formReaderCommand["gnomonCellComplex"];
            qDebug()<<Q_FUNC_INFO<<path.remove("file://");
            cellComplexCommand->setPath(path.remove("file://"));
            cellComplexCommand->redo();
            qDebug()<<Q_FUNC_INFO<<cellComplexCommand;

            gnomonCellComplexSeries *cellComplex = (gnomonCellComplexSeries *) cellComplexCommand->cellComplex()->clone();
            if (!cellComplex) {
                qWarning() << Q_FUNC_INFO << "Resulting cellComplex is void.";
                event->ignore();
                return;
            }
            this->setForm("gnomonCellComplex",(gnomonTimeSeries<gnomonAbstractForm> *) cellComplex);
        } else if(path.endsWith("ply")) {
            if ((!d->formReaderCommand.contains("gnomonMesh"))||(!d->formReaderCommand["gnomonMesh"]))
                d->formReaderCommand["gnomonMesh"] = new gnomonMeshReaderCommand("gnomonMeshReaderPropertyTopomesh");
            gnomonMeshReaderCommand *meshCommand = (gnomonMeshReaderCommand *) d->formReaderCommand["gnomonMesh"];
            meshCommand->setPath(path.remove("file://"));
            meshCommand->redo();

            gnomonMeshSeries *mesh = (gnomonMeshSeries *) meshCommand->mesh()->clone();
            if (!mesh) {
                qWarning() << Q_FUNC_INFO << "Resulting mesh is void.";
                event->ignore();
                return;
            }
            this->setForm("gnomonMesh",(gnomonTimeSeries<gnomonAbstractForm> *) mesh);
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
