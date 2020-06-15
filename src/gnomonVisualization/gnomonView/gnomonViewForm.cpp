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


// TODO: Script

#include <dtkThemes>
#include <dtkWidgets>
//#include <dtkScript>
#include <dtkImagingCore>

#include <QtQuick>
#include <QtQuickWidgets>

#include <gnomonCore>
#include <gnomonComposer>
#include <gnomonWidgets>

#include <gnomonCore/gnomonCommand/gnomonCellComplex/gnomonCellComplexAdapterCommand>
#include <gnomonCore/gnomonCommand/gnomonMesh/gnomonMeshAdapterCommand>

#include "gnomonManager/gnomonFormManager.h"
#include "gnomonVisualizations/gnomonCellComplex/gnomonAbstractVisualizationCellComplex.h"
#include "gnomonVisualizations/gnomonCellImage/gnomonAbstractVisualizationCellImage.h"
#include "gnomonVisualizations/gnomonImage/gnomonAbstractVisualizationImage.h"
#include "gnomonVisualizations/gnomonMesh/gnomonAbstractVisualizationMesh.h"
#include "gnomonVisualizations/gnomonPointCloud/gnomonAbstractVisualizationPointCloud.h"

#include "gnomonInteractorStyle/gnomonInteractorStyle.h"
#include "gnomonInteractorStyle/gnomonInteractorStyleXYZ.h"

#include <vtkCamera.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkInteractorStyleImage.h>
#include <vtkPNGWriter.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkWindowToImageFilter.h>

#include <QVTKInteractor.h>
#include <QVTKOpenGLNativeWidget.h>

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class gnomonFormAdapterMenu : public QQuickWidget
{
    Q_OBJECT

public:
    gnomonFormAdapterMenu(QVariantMap, QWidget *parent = nullptr);

protected:
    void focusOutEvent(QFocusEvent *event) override
    {
        this->close();
        this->deleteLater();

        QQuickWidget::focusOutEvent(event);
    }
};

gnomonFormAdapterMenu::gnomonFormAdapterMenu(QVariantMap adapter_descs, QWidget *parent) : QQuickWidget(parent)
{
    this->engine()->addImportPath("qrc:/");

    QQmlContext *context = this->rootContext();
    context->setContextProperty("font", dtkFontAwesome::instance());
    context->setContextProperty("theme", dtkThemesEngine::instance());
    context->setContextProperty("adapter_descs", adapter_descs);

    this->setResizeMode(QQuickWidget::SizeRootObjectToView);
    this->setSource(QUrl("qrc:/gnomonFormAdapter.qml"));
    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    this->setFocus(Qt::PopupFocusReason);
}



// ///////////////////////////////////////////////////////////////////
// gnomonViewFormPrivate
// ///////////////////////////////////////////////////////////////////

class gnomonViewFormPrivate : public QVTKOpenGLNativeWidget
{
    Q_OBJECT

public:
    enum Orientation {
        SLICE_ORIENTATION_XY = 2,
        SLICE_ORIENTATION_XZ = 1,
        SLICE_ORIENTATION_YZ = 0,
        NONE = -1
    };

public:
     gnomonViewFormPrivate(QWidget *parent = Q_NULLPTR);
    ~gnomonViewFormPrivate(void);

public slots:
    void exportToManager(void);
    void saveScreenshot(void);
    void clear(void);

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
    Orientation ori = NONE;
    QMap<Orientation, vtkSmartPointer<vtkCamera> > cameras;

public:
    QMap<QString, gnomonAbstractDynamicForm *> forms;
    QMap<QString, gnomonAbstractVisualization *> formVisualization;

public:
    QMap<QString, bool> acceptForms;

public:
    gnomonOverlayButton *renderer2D_button = nullptr;
    gnomonOverlayButton *renderer3D_button = nullptr;
    gnomonOverlayButton *renderer2D_XY = nullptr;
    gnomonOverlayButton *renderer2D_XZ = nullptr;
    gnomonOverlayButton *renderer2D_YZ = nullptr;

    gnomonOverlayButton *sync = nullptr;
    gnomonOverlayButton *export_button = nullptr;
    gnomonOverlayButton *save_button = nullptr;
    gnomonOverlayButton *help_button = nullptr;

public:
    gnomonInteractorStyle *default_style = nullptr;
    gnomonInteractorStyle *xyz_style = nullptr;
    QList<gnomonInteractorStyle *> available_styles;

    gnomonInteractorStyle *style = nullptr;

    dtkWidgetsMenuBar *view_menubar = nullptr;
    dtkWidgetsMenuBar *style_menubar = nullptr;

    QMap<gnomonInteractorStyle *, dtkWidgetsMenu *> style_menus;

    QList<gnomonOverlayButton *> shortcut_keys;

public:
    int syncing_count = 0; QTimer *syncing_timer = nullptr; bool synced = false;

public:
    bool enableLink = false;
    bool enableMenus = true;

public:
    QColor export_color = QColor("#cccccc");

public:
    QSlider *slice_slider = nullptr;

public:
    QSlider *time_slider = nullptr;
    QSet<double> forms_times;

public:
    double xBounds[2] = {0,0}, yBounds[2] = {0,0}, zBounds[2] = {0,0};
    double c_x = 0, c_y = 0, c_z = 0;

public:
    double c_t = 0;

public:
    bool empty = true;

signals:
    void sliceOrientationChanged(int);

public slots:
    void configure(dtkWidgetsMenuItemDIY *parent, const QString& key);
    void addFormMenu(const QString& key);
    void refresh(void);

public:
    QMap<QString, QMap<QString, gnomonAbstractCommand *> > adapterCommands;
    QMap<QString, QMap<QString, QString> > adapterTargets;
    QMap<QString, QMap<QString, QString> > adapterDescriptions;

    gnomonAbstractDynamicForm *form_to_adapt = nullptr;

public slots:
    void adaptForm(const QString& adapter_plugin);

public:
    void updateKeys(void);
    void updateInteractorStyleMenu(void);

// /////////////////////////////////////////////////////////////////////////////
// Menu stuff
// /////////////////////////////////////////////////////////////////////////////

public:
    QPushButton *renderButton = nullptr;
    QPushButton *clearButton = nullptr;

    QMap<QString, QFormLayout *> parameterLayouts;

    QMap<QString, dtkWidgetsMenu *> formVisualizationMenus;
    QMap<QString, dtkWidgetsMenuItemDIY *> formVisualizationPaneItems;

    dtkWidgetsMenuItemDIY *paneItemButton = nullptr;

public:
    dtkWidgetsMenu *menu(void);

public:
    dtkWidgetsMenu *view_menu;
    
public:
    gnomonFormAdapterMenu *adapter_menu = nullptr;

// /////////////////////////////////////////////////////////////////////////////
};

gnomonViewFormPrivate::gnomonViewFormPrivate(QWidget *parent) : QVTKOpenGLNativeWidget(parent)
{
    QColor background_color = dtkThemesEngine::instance()->color("@bgalt");

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
    this->export_button->toggle(true);

    this->save_button = new gnomonOverlayButton(fa::save, "", this);
    this->save_button->toggle(true);

    this->help_button = new gnomonOverlayButton(fa::questioncircle, "", this);
    this->help_button->toggle(false);

    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    this->default_style = new gnomonInteractorStyle();
    this->xyz_style = new gnomonInteractorStyleXYZ();

    this->available_styles.push_back(this->default_style);
    this->available_styles.push_back(this->xyz_style);

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

    static int count = 0;
//    this->view_menu = new dtkWidgetsMenu(fa::image, "View " + QString::number(count++));
    this->view_menu = new dtkWidgetsMenu(fa::cubes, "3D Form Viewer", this);

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

    connect(dtkThemesEngine::instance(), &dtkThemesEngine::changed, [=] (void) -> void
    {
        QColor bg = dtkThemesEngine::instance()->color("@bgalt");

        this->renderer2D->SetBackground(bg.redF(), bg.greenF(), bg.blueF());
        this->renderer3D->SetBackground(bg.redF(), bg.greenF(), bg.blueF());
        this->GetInteractor()->Render();
    });
}

gnomonViewFormPrivate::~gnomonViewFormPrivate(void)
{
    // the mother-ViewForm is called in the FormManager to display detailed views of the stored items.
    // it can be destroyed at runtime, so we need to destroy its fields.
    delete this->default_style;
    delete this->xyz_style;
    // the rest is deleted by parent-relationship.

}

void gnomonViewFormPrivate::exportToManager(void)
{
    for (const auto& key : this->forms.keys()) {
        gnomonFormManager::instance()->addForm(this->forms[key], this->export_color, this->formVisualization[key], this->renderer3D->GetActiveCamera());
        q->emit exportedForm(this->forms[key]);
    }
}

void gnomonViewFormPrivate::saveScreenshot(void)
{
    QSettings settings("inria", "gnomon");
    settings.beginGroup("General");
    QString path = settings.value("last_saved_file", QDir::homePath()).toString();
    settings.endGroup();

    QString export_file_path;
    export_file_path = QFileDialog::getSaveFileName(this, tr("Save screenshot"), path, tr("PNG Image (*.png)"));

    this->GetRenderWindow()->SetAlphaBitPlanes(1);

    vtkSmartPointer<vtkWindowToImageFilter> screenshooter = vtkWindowToImageFilter::New();
    screenshooter->SetInput(this->GetRenderWindow());
    screenshooter->SetInputBufferTypeToRGBA(); //also record the alpha (transparency) channel
    screenshooter->ReadFrontBufferOff();
    screenshooter->Update();

    vtkSmartPointer<vtkPNGWriter> writer = vtkPNGWriter::New();
    writer->SetFileName(export_file_path.toStdString().c_str());
    writer->SetInputConnection(screenshooter->GetOutputPort());
    writer->Update();
    writer->Write();
}

QSize gnomonViewFormPrivate::sizeHint(void) const
{
    return QSize(1200, 800);
}

void gnomonViewFormPrivate::resizeEvent(QResizeEvent *event)
{
    static int l_margin;
    if (this->enableMenus) {
        l_margin = 38;
    } else {
        l_margin = 0;
    }
    static int r_margin = 0;

    this->renderer2D_button->move(l_margin + 10, 10);
    this->renderer3D_button->move(l_margin + 50, 10);
    this->renderer2D_XY->move(l_margin + 10,  50);
    this->renderer2D_XZ->move(l_margin + 10,  90);
    this->renderer2D_YZ->move(l_margin + 10, 130);

    this->export_button->move(event->size().width() - r_margin - 40, 10);
    this->save_button->move(event->size().width() - r_margin - 80, 10);
    if (this->enableLink) {
        this->sync->setVisible(true);
        this->sync->move(event->size().width() - r_margin - 120, 10);
        this->help_button->move(event->size().width() - r_margin - 160, 10);
    } else {
        this->sync->setVisible(false);
        this->help_button->move(event->size().width() - r_margin - 120, 10);
    }
    for(int i_key=0; i_key<this->shortcut_keys.size(); i_key++) {
        this->shortcut_keys[i_key]->move(event->size().width() - r_margin - 240, 50 + 40*i_key);
    }

    if (this->view_menubar) {
        this->view_menubar->setFixedHeight(event->size().height());
        if (this->enableMenus) {
            this->view_menubar->show();
        } else {
            this->view_menubar->hide();
        }
    }

    if (this->style_menubar){
        this->style_menubar->setFixedHeight(32*(1+this->available_styles.size())+32);
        this->style_menubar->move(QPoint(0, 32*(1+this->formVisualizationMenus.size())+32));
        if (this->enableMenus) {
            this->style_menubar->show();
        } else {
            this->style_menubar->hide();
        }
    }



    QVTKOpenGLNativeWidget::resizeEvent(event);
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

            default:
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

void gnomonViewFormPrivate::clear(void)
{
    for (const auto& key : this->formVisualization.keys()) {

        qDebug() << Q_FUNC_INFO << key;

        this->formVisualization[key]->disconnect();
        this->formVisualization[key]->clearConnections();
        this->formVisualization[key]->clear();
        delete this->formVisualization[key];
        this->parameterLayouts[key]->disconnect();
        delete this->parameterLayouts[key];

        this->formVisualizationMenus[key]->removeItem(this->formVisualizationPaneItems[key]);

        this->formVisualizationPaneItems[key]->disconnect();
        this->formVisualizationPaneItems[key]->clear();
        delete this->formVisualizationPaneItems[key];

        this->view_menubar->removeMenu(this->formVisualizationMenus[key]);

        this->formVisualizationMenus[key]->disconnect();
        this->formVisualizationMenus[key]->clear();
        delete this->formVisualizationMenus[key];
    }

    this->formVisualization.clear();
    this->forms.clear();
    this->parameterLayouts.clear();
    this->formVisualizationMenus.clear();
    this->formVisualizationPaneItems.clear();

    this->updateInteractorStyleMenu();

    this->empty = true;

    this->refresh();
    this->view_menubar->touch();

    q->render();
}

dtkWidgetsMenu *gnomonViewFormPrivate::menu(void)
{
    if(!this->renderButton)
    {
        this->renderButton = new QPushButton("Render");

        connect(this->renderButton, &QPushButton::clicked, [=] ()
        {
            for (const auto& key : this->formVisualization.keys()) {
                gnomonAbstractVisualization *v = this->formVisualization[key];
                if(v) {
                    dtkApp->window()->setCursor(Qt::BusyCursor);
                    v->update();
                    dtkApp->window()->setCursor(Qt::ArrowCursor);
                }
            }
        });
    }

    if(!this->clearButton)
    {
        this->clearButton = new QPushButton("Clear");

        connect(this->clearButton, &QPushButton::clicked, [=] ()
        {
            this->clear();
        });
     }

    this->renderButton->setCheckable(true);
    this->clearButton->setCheckable(true);

    static int count = 0;

    if(!this->paneItemButton) {
        this->paneItemButton = new dtkWidgetsMenuItemDIY("View controls" + QString::number(count));
        this->paneItemButton->setShowTitle(false);
    }

    this->paneItemButton->addWidget(this->renderButton);
    this->paneItemButton->addWidget(this->clearButton);

    this->refresh();

    return this->view_menu;
}

void gnomonViewFormPrivate::configure(dtkWidgetsMenuItemDIY *parent, const QString& key)
{
    if (this->formVisualization.contains(key)) {

        gnomonAbstractVisualization *v = this->formVisualization[key];

        if(v) {
             if ((this->parameterLayouts.contains(key)) && (this->parameterLayouts[key])) {
                 for(int row = 0, max_row = this->parameterLayouts[key]->count(); row < max_row; ++row) {
                     QLayoutItem *forDeletion = this->parameterLayouts[key]->takeAt(0);
                     forDeletion->widget()->disconnect();
                     delete forDeletion->widget();
                     delete forDeletion;
                 }
             } else {

                this->parameterLayouts[key] = new QFormLayout;
                this->parameterLayouts[key]->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);

                parent->addLayout(this->parameterLayouts[key]);
            }

            QMap<QString, gnomonCoreParameter *> parameters = v->parameters();

            for(QMap<QString, gnomonCoreParameter*>::iterator it = parameters.begin(), it_end = parameters.end(); it != it_end; ++it) {
                QWidget *widget = gnomonWidgetsParameter::widget(it.value(), 0);
                if (widget) {
                    this->parameterLayouts[key]->addRow(it.key(), widget);
                }
            }
        }
    }

    this->refresh();
}

void gnomonViewFormPrivate::addFormMenu(const QString& key)
{
    if ((!this->formVisualizationPaneItems.contains(key))||(!this->formVisualizationPaneItems[key]))
    {
        int icon = fa::circlethin;
        if (key == "gnomonCellComplex") {
//            icon = fa::bordernone;
            icon = fa::image;
        } else if (key == "gnomonCellImage") {
//            icon = fa::borderall;
            icon = fa::image;
        } else if (key == "gnomonImage") {
            icon = fa::image;
        } else if (key == "gnomonMesh") {
//            icon = fa::dice-d20;
            icon = fa::image;
        } else if (key == "gnomonPointCloud") {
//            icon = fa::braille;
            icon = fa::image;
        }

        QString menu_name = key;
        menu_name.remove("gnomon");
        this->formVisualizationMenus[key] = new dtkWidgetsMenu(icon, menu_name);
        this->formVisualizationPaneItems[key] = new dtkWidgetsMenuItemDIY(key);

        this->formVisualizationPaneItems[key]->setShowTitle(false);
        this->formVisualizationMenus[key]->addItem(this->formVisualizationPaneItems[key]);

        QComboBox *combo_box = new QComboBox;

        QWidget *contents = new QWidget;

        QStringList combo_box_keys = {};

        if (key == "gnomonCellComplex") {
            combo_box_keys = gnomonVisualization::visualizationCellComplex::pluginFactory().keys();
        } else if (key == "gnomonCellImage") {
            combo_box_keys = gnomonVisualization::visualizationCellImage::pluginFactory().keys();
        } else if (key == "gnomonImage") {
            combo_box_keys = gnomonVisualization::visualizationImage::pluginFactory().keys();
        } else if (key == "gnomonMesh") {
            combo_box_keys = gnomonVisualization::visualizationMesh::pluginFactory().keys();
        } else if (key == "gnomonPointCloud") {
            combo_box_keys = gnomonVisualization::visualizationPointCloud::pluginFactory().keys();
        }
        for (auto it = combo_box_keys.begin(), it_end = combo_box_keys.end(); it != it_end; ++it) {
            combo_box->addItem(*it);
        }
        combo_box->model()->sort(0);

        connect(combo_box, &QComboBox::currentTextChanged, [=] (const QString& visu)
        {
            q->switchTo3D();

            if (this->formVisualization[key]) {
                this->formVisualization[key]->clear();
                // TODO:
                delete this->formVisualization[key];
                this->formVisualization[key] = nullptr;
            }
            if (key == "gnomonCellComplex") {
                this->formVisualization[key] = gnomonVisualization::visualizationCellComplex::pluginFactory().create(visu);
                this->formVisualization[key]->setView(q);
                gnomonAbstractVisualizationCellComplex *formVisualizationCellComplex = (gnomonAbstractVisualizationCellComplex *)this->formVisualization[key];
                dtkApp->window()->setCursor(Qt::BusyCursor);
                gnomonCellComplexSeries *cellComplex = (gnomonCellComplexSeries *)this->forms[key];
                formVisualizationCellComplex->setCellComplex(cellComplex);
                formVisualizationCellComplex->update();
                dtkApp->window()->setCursor(Qt::ArrowCursor);
            } else if (key == "gnomonCellImage") {
                this->formVisualization[key] = gnomonVisualization::visualizationCellImage::pluginFactory().create(visu);
                this->formVisualization[key]->setView(q);
                gnomonAbstractVisualizationCellImage *formVisualizationCellImage = (gnomonAbstractVisualizationCellImage *)this->formVisualization[key];
                dtkApp->window()->setCursor(Qt::BusyCursor);
                gnomonCellImageSeries *cellImage = (gnomonCellImageSeries *)this->forms[key];
                formVisualizationCellImage->setCellImage(cellImage);
                formVisualizationCellImage->update();
                dtkApp->window()->setCursor(Qt::ArrowCursor);
            } else if (key == "gnomonImage") {
                this->formVisualization[key] = gnomonVisualization::visualizationImage::pluginFactory().create(visu);
                this->formVisualization[key]->setView(q);
                gnomonAbstractVisualizationImage *formVisualizationImage = (gnomonAbstractVisualizationImage *)this->formVisualization[key];
                dtkApp->window()->setCursor(Qt::BusyCursor);
                gnomonImageSeries *image = (gnomonImageSeries *)this->forms[key];
                formVisualizationImage->setImage(image);
                formVisualizationImage->update();
                dtkApp->window()->setCursor(Qt::ArrowCursor);
            } else if (key == "gnomonMesh") {
                this->formVisualization[key] = gnomonVisualization::visualizationMesh::pluginFactory().create(visu);
                this->formVisualization[key]->setView(q);
                gnomonAbstractVisualizationMesh *formVisualizationMesh = (gnomonAbstractVisualizationMesh *)this->formVisualization[key];
                gnomonMeshSeries *mesh = (gnomonMeshSeries *)this->forms[key];
                formVisualizationMesh->setMesh(mesh);
                dtkApp->window()->setCursor(Qt::BusyCursor);
                formVisualizationMesh->update();
                dtkApp->window()->setCursor(Qt::ArrowCursor);
            } else if (key == "gnomonPointCloud") {
                this->formVisualization[key] = gnomonVisualization::visualizationPointCloud::pluginFactory().create(visu);
                this->formVisualization[key]->setView(q);
                gnomonAbstractVisualizationPointCloud *formVisualizationPointCloud = (gnomonAbstractVisualizationPointCloud *)this->formVisualization[key];
                dtkApp->window()->setCursor(Qt::BusyCursor);
                gnomonPointCloudSeries *pointCloud = (gnomonPointCloudSeries *)this->forms[key];
                formVisualizationPointCloud->setPointCloud(pointCloud);
                formVisualizationPointCloud->update();
                dtkApp->window()->setCursor(Qt::ArrowCursor);
            }

            this->configure(formVisualizationPaneItems[key], key);
            this->updateInteractorStyleMenu();
        });

        this->formVisualizationPaneItems[key]->addWidget(combo_box);
        this->formVisualizationPaneItems[key]->addWidget(contents);

        this->view_menubar->addMenu(this->formVisualizationMenus[key]);
        this->view_menubar->touch();
    }

}

void gnomonViewFormPrivate::refresh(void)
{
    this->view_menu->removeItem(this->paneItemButton);

    for (const auto& menu : this->view_menubar->menus()) {
        this->view_menubar->removeMenu(menu);
    }

    for (const auto& key : this->formVisualizationMenus.keys()) {
        this->view_menubar->addMenu(this->formVisualizationMenus[key]);
    }

    this->view_menu->addItem(this->paneItemButton);

    this->view_menubar->addMenu(this->view_menu);

    this->resizeEvent(new QResizeEvent(this->size(), QSize()));

    this->view_menubar->touch();
}

void gnomonViewFormPrivate::updateTimeSlider(void)
{
    this->forms_times.clear();

    for (const auto& key : this->forms.keys()) {
        for(auto time : this->forms[key]->times()) {
            this->forms_times.insert(time);
        }
    }

    if(this->forms_times.size() < 2) {
        this->time_slider->setVisible(false);
        return;
    }

    this->time_slider->setVisible(true);
    this->time_slider->setOrientation(Qt::Horizontal);
    this->time_slider->setMinimum(0);
    this->time_slider->setMaximum(this->forms_times.size()-1);
}

void gnomonViewFormPrivate::updateKeys(void)
{
    for (int i_key=0;i_key<this->shortcut_keys.size();i_key++) {
        delete this->shortcut_keys[i_key];
    }
    this->shortcut_keys.clear();

    QMap<int, QString> keymap = this->style->keyMap();
    for(const auto& key : keymap.keys()) {
        QChar key_char;
        if (QKeySequence(key).toString().size()==1) {
            key_char = QKeySequence(key).toString().at(0);
        } else {
            if (key == Qt::Key_Shift) {
                key_char = QChar(0x21E7);
            } else if (key == Qt::Key_Alt) {
                key_char = QChar(0x2325);
            }  else if (key == Qt::Key_Control) {
                key_char = QChar(0x2318);
            } else if (key == -Qt::LeftButton) { //Mouse click
                key_char = QChar(0x2196);
            } else if (key == -2*Qt::LeftButton) { //Mouse double click
                key_char = QChar(0x21b8);
            } else if (key == -3*Qt::LeftButton) { //Mouse scroll
                key_char = QChar(0x2195);
            } else {
                key_char = ' ';
            }
        }
        gnomonOverlayButton *shortcut_key = new gnomonOverlayButton(key_char, keymap[key], this);
        shortcut_key->setFixedWidth(240);
        shortcut_key->setVisible(this->help_button->isToggled());
        this->shortcut_keys.push_back(shortcut_key);
    }
    this->resizeEvent(new QResizeEvent(this->size(), QSize()));
}


void gnomonViewFormPrivate::updateInteractorStyleMenu(void)
{
    this->available_styles.clear();
    this->available_styles.push_back(this->default_style);
    this->available_styles.push_back(this->xyz_style);
    for(const auto& visu : this->formVisualization.values()) {
        if(gnomonInteractorStyle *style = visu->interactorStyle()) {
            this->available_styles.append(style);
        }
    }

    for (const auto& menu : this->style_menus.values()) {
        this->style_menubar->removeMenu(menu);
    }
    this->style_menubar->disconnect();
    this->style_menus.clear();
    for (const auto& style : this->available_styles) {
        this->style_menus[style] = this->style_menubar->addMenu(style->icon(), style->description());
    }
    connect(this->style_menubar, &dtkWidgetsMenuBar::clicked, [=] (int i_style)
    {
        gnomonInteractorStyle* style = this->available_styles[i_style];
        this->q->setInteractorStyle(style);
    });
    this->refresh();
    this->style_menubar->touch();
}

void gnomonViewFormPrivate::adaptForm(const QString& adapter_plugin)
{
    gnomonAbstractDynamicForm *form = this->form_to_adapt;

    if (gnomonMeshSeries* mesh = dynamic_cast<gnomonMeshSeries *>(form)) {
        gnomonMeshAdapterCommand *meshCommand = dynamic_cast<gnomonMeshAdapterCommand *>(this->adapterCommands["gnomonMesh"][adapter_plugin]);

        meshCommand->setInput(mesh);
        meshCommand->redo();
        gnomonAbstractDynamicForm *adaptedMesh = meshCommand->output();
        if (adaptedMesh) {
            gnomonPipeline::instance()->addAdapter(meshCommand);
            q->setForm("adaptedMesh",adaptedMesh);
        }
    } else if (gnomonCellComplexSeries* cellComplex = dynamic_cast<gnomonCellComplexSeries *>(form)) {
        gnomonCellComplexAdapterCommand *cellComplexCommand = dynamic_cast<gnomonCellComplexAdapterCommand *>(this->adapterCommands["gnomonCellComplex"][adapter_plugin]);

        cellComplexCommand->setInput(cellComplex);
        cellComplexCommand->redo();
        gnomonAbstractDynamicForm *adaptedCellComplex = cellComplexCommand->output();
        if (adaptedCellComplex) {
            gnomonPipeline::instance()->addAdapter(cellComplexCommand);
            q->setForm("adaptedCellComplex",adaptedCellComplex);
        }
    }

    if (this->adapter_menu) {
        this->adapter_menu->close();
    }
}

// ///////////////////////////////////////////////////////////////////
// gnomonViewForm
// ///////////////////////////////////////////////////////////////////

gnomonViewForm::gnomonViewForm(QWidget *parent) : QFrame(parent)
{
    d = new gnomonViewFormPrivate;
    d->q = this;

    loadPluginGroup("visualizationCellComplex");
    loadPluginGroup("visualizationCellImage");
    loadPluginGroup("visualizationImage");
    loadPluginGroup("visualizationMesh");
    loadPluginGroup("visualizationPointCloud");

    d->acceptForms["gnomonCellComplex"] = false;
    d->acceptForms["gnomonCellImage"] = false;
    d->acceptForms["gnomonImage"] = false;
    d->acceptForms["gnomonMesh"] = false;
    d->acceptForms["gnomonPointCloud"] = false;

    for (const auto& form : d->acceptForms.keys()) {
        if (form=="gnomonMesh") {
            loadPluginGroup("meshAdapter");
            qDebug()<<Q_FUNC_INFO<<gnomonCore::meshAdapter::pluginFactory().keys();
            for (const auto& key : gnomonCore::meshAdapter::pluginFactory().keys())
            {
                gnomonAbstractMeshAdapter *adapter = dynamic_cast<gnomonAbstractMeshAdapter *>(gnomonCore::meshAdapter::pluginFactory().create(key));
                if (!d->adapterCommands.contains(form))
                {
                    QMap<QString, QString> empty_target;
                    d->adapterTargets[form] = empty_target;
                    QMap<QString, QString> empty_desc;
                    d->adapterDescriptions[form] = empty_desc;
                    QMap<QString, gnomonAbstractCommand *> empty_list;
                    d->adapterCommands[form] = empty_list;
                }
                d->adapterTargets[form][key] = adapter->target();
                d->adapterDescriptions[form][key] = adapter->documentation().split("\n")[1];
                d->adapterCommands[form][key] = new gnomonMeshAdapterCommand(key);
                delete adapter;
            }
        } else if (form=="gnomonCellComplex") {
            loadPluginGroup("cellComplexAdapter");
            qDebug()<<Q_FUNC_INFO<<gnomonCore::cellComplexAdapter::pluginFactory().keys();
            for (const auto& key : gnomonCore::cellComplexAdapter::pluginFactory().keys())
            {
                gnomonAbstractCellComplexAdapter *adapter = dynamic_cast<gnomonAbstractCellComplexAdapter *>(gnomonCore::cellComplexAdapter::pluginFactory().create(key));
                if (!d->adapterCommands.contains(form))
                {
                    QMap<QString, QString> empty_target;
                    d->adapterTargets[form] = empty_target;
                    QMap<QString, QString> empty_desc;
                    d->adapterDescriptions[form] = empty_desc;
                    QMap<QString, gnomonAbstractCommand *> empty_list;
                    d->adapterCommands[form] = empty_list;
                }
                d->adapterTargets[form][key] = adapter->target();
                d->adapterDescriptions[form][key] = adapter->documentation().split("\n")[1];
                d->adapterCommands[form][key] = new gnomonCellComplexAdapterCommand(key);
                delete adapter;
            }
        } 
    }

    connect(d->renderer2D_button, SIGNAL(iconClicked()), this, SLOT(switchTo2D()));
    connect(d->renderer3D_button, SIGNAL(iconClicked()), this, SLOT(switchTo3D()));
    connect(d->renderer2D_XY, SIGNAL(iconClicked()), this, SLOT(switchTo2DXY()));
    connect(d->renderer2D_XZ, SIGNAL(iconClicked()), this, SLOT(switchTo2DXZ()));
    connect(d->renderer2D_YZ, SIGNAL(iconClicked()), this, SLOT(switchTo2DYZ()));

    connect(d->export_button, &gnomonOverlayButton::iconClicked, [=] ()
    {
        if (d->export_button->isToggled()) {
            d->exportToManager();
        }
    });

    connect(d->save_button,  &gnomonOverlayButton::iconClicked, [=] ()
    {
        if (d->save_button->isToggled()) {
            d->saveScreenshot();
        }
    });

    connect(d->help_button, &gnomonOverlayButton::iconClicked, [=] ()
    {
        d->help_button->toggle(!d->help_button->isToggled());
        for(int i_key=0; i_key<d->shortcut_keys.size(); i_key++) {
            d->shortcut_keys[i_key]->setVisible(d->help_button->isToggled());
        }
    });

    d->slice_slider = new QSlider(this);
    d->slice_slider->setObjectName("Slice Position");
    d->slice_slider->setOrientation(Qt::Vertical);
    d->slice_slider->setMinimum(0);
    d->slice_slider->setMaximum(1);
    d->slice_slider->setValue(0);

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

    d->view_menubar = new dtkWidgetsMenuBar(d);
    d->view_menubar->hide();
    d->view_menubar->setInteractive(false);
    d->view_menubar->setWidth(32);
    d->view_menubar->setMargins(6);
    d->view_menubar->addMenu(d->menu());
    d->view_menubar->touch();

    d->style_menubar = new dtkWidgetsMenuBar(d);
    d->style_menubar->hide();
    d->style_menubar->setInteractive(false);
    d->style_menubar->setStandalone(true);
    d->style_menubar->setWidth(32);
    d->style_menubar->setMargins(6);
    d->style_menubar->setObjectName("RHS");

    d->updateInteractorStyleMenu();

    this->setInteractorStyle(d->default_style);

    QGridLayout *layout  = new QGridLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(d->slice_slider, 0, 1, 1, 1);
    layout->addWidget(d, 0, 3, 1, 1);
    layout->addWidget(d->time_slider, 1, 1, 1, 3);

    static int count = 0;

//    d->view_item = new dtkWidgetsMenuItemDIY("View parameters" + QString::number(count++));
//    d->view_item->setShowTitle(false);

    connect(d->sync, &gnomonOverlayButton::iconClicked, [=] ()
    {
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
        d->addFormMenu(key);
        d->configure(d->formVisualizationPaneItems[key], key);
        d->updateInteractorStyleMenu();
        d->updateTimeSlider();
        if (d->empty) {
            d->renderer3D->ResetCamera();
        }
        d->empty = false;
        this->render();
    });

    this->setAcceptDrops(false);

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

    d->renderer3D->InteractiveOn();
    d->renderer3D->DrawOn();

    emit switchedTo3D();

    d->slice_slider->setEnabled(false);
    d->slice_slider->setVisible(false);
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

    d->renderer2D->InteractiveOn();
    d->renderer2D->DrawOn();


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

        default:
            break;
    }
    
    emit switchedTo2D();

    d->slice_slider->setVisible(true);
    d->slice_slider->setEnabled(true);
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

    bool hasChanged = d->ori != gnomonViewFormPrivate::SLICE_ORIENTATION_XY;
    d->setSliceOrientation(gnomonViewFormPrivate::SLICE_ORIENTATION_XY);

    d->renderer2D_XY->toggle(true);
    d->renderer2D_XZ->toggle(false);
    d->renderer2D_YZ->toggle(false);

    if (hasChanged)
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

    bool hasChanged = d->ori != gnomonViewFormPrivate::SLICE_ORIENTATION_XZ;
    d->setSliceOrientation(gnomonViewFormPrivate::SLICE_ORIENTATION_XZ);

    d->renderer2D_XY->toggle(false);
    d->renderer2D_XZ->toggle(true);
    d->renderer2D_YZ->toggle(false);

    if (hasChanged)
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

    bool hasChanged = d->ori != gnomonViewFormPrivate::SLICE_ORIENTATION_YZ;
    d->setSliceOrientation(gnomonViewFormPrivate::SLICE_ORIENTATION_YZ);

    d->renderer2D_XY->toggle(false);
    d->renderer2D_XZ->toggle(false);
    d->renderer2D_YZ->toggle(true);

    if (hasChanged)
        emit switchedTo2DYZ();
}

void gnomonViewForm::sliceChange(int value)
{
    bool valueChanged = false;

    switch(d->ori)
    {
        case gnomonViewFormPrivate::SLICE_ORIENTATION_XY:
            if (d->c_z != value) {
                d->c_z = value;
                valueChanged = true;
            }
            break;
        case gnomonViewFormPrivate::SLICE_ORIENTATION_XZ:
            if (d->c_y != value) {
                d->c_y = value;
                valueChanged = true;
            }
            break;
        case gnomonViewFormPrivate::SLICE_ORIENTATION_YZ:
            if (d->c_x != value){
                d->c_x = value;
                valueChanged = true;
            }
            break;
        default:
            break;
    };

    d->slice_slider->blockSignals(true);
    d->slice_slider->setValue(value);
    d->slice_slider->blockSignals(false);

    if (valueChanged)
        emit sliceChanged(value);

    d->GetInteractor()->Render();
}


void gnomonViewForm::timeIndexChange(int value)
{
    QList<double> sorted_times = this->times();

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
        emit timeChanged(time);
    }

    d->GetInteractor()->Render();
}

QList<double> gnomonViewForm::times(void)
{
    QList<double> sorted_times = QList<double>::fromSet(d->forms_times);
    qSort(sorted_times);

    return sorted_times;
}

void gnomonViewForm::link(gnomonViewForm *other)
{
    if (d->syncing_timer)
        d->syncing_timer->stop();

    d->sync->toggle(true);
    d->sync->changeIcon(fa::lock);

    d->synced = true;

    // ///////////////////////////////////////////////////////////////

//    d->renderer2D->SetActiveCamera(other->d->renderer2D->GetActiveCamera());
    d->renderer3D->SetActiveCamera(other->d->renderer3D->GetActiveCamera());

    other->d->GetRenderWindow()->AddObserver(vtkCommand::RenderEvent, this, &gnomonViewForm::render);

    connect(other, SIGNAL(switchedTo3D()), this, SLOT(switchTo3D()));
    connect(other, &gnomonViewForm::switchedTo2D, [=] () {
        this->switchTo2D();
        d->renderer2D->SetActiveCamera(other->d->renderer2D->GetActiveCamera());
    });
    connect(other, &gnomonViewForm::switchedTo2DXY, [=] () {
        this->switchTo2DXY();
        d->renderer2D->SetActiveCamera(other->d->renderer2D->GetActiveCamera());
    });
    connect(other, &gnomonViewForm::switchedTo2DXZ, [=] () {
        this->switchTo2DXZ();
        d->renderer2D->SetActiveCamera(other->d->renderer2D->GetActiveCamera());
    });
    connect(other, &gnomonViewForm::switchedTo2DYZ, [=] () {
        this->switchTo2DYZ();
        d->renderer2D->SetActiveCamera(other->d->renderer2D->GetActiveCamera());
    });
//    connect(other, &gnomonViewForm::switchedTo2DXZ()), this, SLOT(switchTo2DXZ()));
//    connect(other, &gnomonViewForm::switchedTo2DYZ()), this, SLOT(switchTo2DYZ()));
    connect(other, SIGNAL(sliceChanged(int)), this, SLOT(sliceChange(int)));
    connect(other, SIGNAL(timeChanged(double)), this, SLOT(onTimeChanged(double)));
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
    d->renderer2D->SetActiveCamera(camera2D);

    vtkSmartPointer<vtkCamera> camera3D = vtkCamera::New();
    camera3D->ShallowCopy(d->renderer3D->GetActiveCamera());
    d->renderer3D->SetActiveCamera(camera3D);

    // ///////////////////////////////////////////////////////////////

    disconnect(other, SIGNAL(switchedTo3D()), this, SLOT(switchTo3D()));
    disconnect(other, SIGNAL(switchedTo2D()), this, SLOT(switchTo2D()));
    disconnect(other, SIGNAL(switchedTo2DXY()), this, SLOT(switchTo2DXY()));
    disconnect(other, SIGNAL(switchedTo2DXZ()), this, SLOT(switchTo2DXZ()));
    disconnect(other, SIGNAL(switchedTo2DYZ()), this, SLOT(switchTo2DYZ()));
    disconnect(other, SIGNAL(sliceChanged(int)), this, SLOT(sliceChange(int)));
    disconnect(other, SIGNAL(timeChanged(double)), this, SLOT(onTimeChanged(double)));
}


void gnomonViewForm::setExportColor(const QColor& color)
{
    d->export_color = color;
}

// void gnomonViewForm::toggleVisualizationPane(void)
// {
//     d->formVisualizationPane->toggle();
// }

gnomonAbstractDynamicForm *gnomonViewForm::form(const QString& name)
{
    if (d->forms.contains(name)) {
        return d->forms[name];
    } else {
        return nullptr;
    }
}

void gnomonViewForm::setForm(const QString& name, gnomonAbstractDynamicForm *form, gnomonAbstractVisualization *visualization)
{
    if (gnomonCellImageSeries *cellImage = dynamic_cast<gnomonCellImageSeries *>(form)) {
        if (d->acceptForms["gnomonCellImage"]) {
            this->setCellImage(cellImage, visualization);
        }
    } else if (gnomonCellComplexSeries *cellComplex = dynamic_cast<gnomonCellComplexSeries *>(form)) {
        if (d->acceptForms["gnomonCellComplex"]) {
            this->setCellComplex(cellComplex, visualization);
        } else {
            this->setAdaptedForm("gnomonCellComplex", cellComplex, visualization);
        }
    } else if (gnomonImageSeries *image = dynamic_cast<gnomonImageSeries *>(form)) {
        if (d->acceptForms["gnomonImage"]) {
            this->setImage(image, visualization);
        }
    } else if (gnomonMeshSeries *mesh = dynamic_cast<gnomonMeshSeries *>(form)) {
        if (d->acceptForms["gnomonMesh"]) {
            this->setMesh(mesh, visualization);
        } else {
            this->setAdaptedForm("gnomonMesh", mesh, visualization);
        }
    } else if (gnomonPointCloudSeries *pointCloud = dynamic_cast<gnomonPointCloudSeries *>(form)) {
        if (d->acceptForms["gnomonPointCloud"]) {
            this->setPointCloud(pointCloud, visualization);
        }
    }
    return;
}

void gnomonViewForm::setAdaptedForm(const QString& name, gnomonAbstractDynamicForm *form, gnomonAbstractVisualization *visualization)
{
    qDebug()<<Q_FUNC_INFO<<d->adapterCommands.keys();
    if (d->adapterCommands.contains(name)) {
        QVariantMap adapter_descs;
        for (const auto &key : d->adapterCommands[name].keys()) {
            if (d->acceptForms[d->adapterTargets[name][key]]) {
                adapter_descs[key] = d->adapterDescriptions[name][key];
            }
        }
        if (adapter_descs.size() > 0) {
            d->form_to_adapt = form;
            d->adapter_menu = new gnomonFormAdapterMenu(adapter_descs);
            d->adapter_menu->setAttribute(Qt::WA_DeleteOnClose, true);
            d->adapter_menu->resize(dtkApp->window()->width() * 1/3, dtkApp->window()->height() - 40);
            d->adapter_menu->move(dtkApp->window()->frameGeometry().topLeft() + QPoint(86,0));
            d->adapter_menu->show();

            QObject *context = d->adapter_menu->rootObject();
            connect(context, SIGNAL(clicked(const QString&)), d, SLOT(adaptForm(const QString&)));

            QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(d->adapter_menu);
        }
    }
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

    QString key = gnomonVisualization::visualizationCellImage::pluginFactory().keys()[0];

    if ((!d->formVisualization.contains("gnomonCellImage"))||(!d->formVisualization["gnomonCellImage"]))
    {
        d->formVisualization["gnomonCellImage"] = gnomonVisualization::visualizationCellImage::pluginFactory().create(key);
        d->formVisualization["gnomonCellImage"]->setView(this);
    }

    gnomonAbstractVisualizationCellImage *formVisualizationCellImage = (gnomonAbstractVisualizationCellImage *)d->formVisualization["gnomonCellImage"];
    dtkApp->window()->setCursor(Qt::BusyCursor);
    formVisualizationCellImage->setCellImage(cellImage);
    if (visualization) {
        formVisualizationCellImage->setParameters(visualization->parameters());
    }
    formVisualizationCellImage->update();
    dtkApp->window()->setCursor(Qt::ArrowCursor);

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

    QString key = gnomonVisualization::visualizationCellComplex::pluginFactory().keys()[0];
//    QString key = "gnomonVisualizationCellComplexTriangularMesh";

    if ((!d->formVisualization.contains("gnomonCellComplex"))||(!d->formVisualization["gnomonCellComplex"]))
    {
//        d->formVisualization["gnomonCellComplex"] = new gnomonVisualizationCellComplex();
        d->formVisualization["gnomonCellComplex"] = gnomonVisualization::visualizationCellComplex::pluginFactory().create(key);
        d->formVisualization["gnomonCellComplex"]->setView(this);
    }
    gnomonAbstractVisualizationCellComplex *formVisualizationCellComplex = (gnomonAbstractVisualizationCellComplex *)d->formVisualization["gnomonCellComplex"];
    dtkApp->window()->setCursor(Qt::BusyCursor);
    formVisualizationCellComplex->setCellComplex(cellComplex);
    if (visualization) {
        formVisualizationCellComplex->setParameters(visualization->parameters());
    }
    formVisualizationCellComplex->update();
    dtkApp->window()->setCursor(Qt::ArrowCursor);

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

    qDebug() << gnomonVisualization::visualizationImage::pluginFactory().keys();
    QString key = gnomonVisualization::visualizationImage::pluginFactory().keys()[0];

    if ((!d->formVisualization.contains("gnomonImage"))||(!d->formVisualization["gnomonImage"]))
        {
            d->formVisualization["gnomonImage"] = gnomonVisualization::visualizationImage::pluginFactory().create(key);
            d->formVisualization["gnomonImage"]->setView(this);
        }

    gnomonAbstractVisualizationImage *formVisualizationImage = (gnomonAbstractVisualizationImage *)d->formVisualization["gnomonImage"];
    dtkApp->window()->setCursor(Qt::BusyCursor);
    formVisualizationImage->setImage(image);
    if (visualization) {
        formVisualizationImage->setParameters(visualization->parameters());
    }
    formVisualizationImage->update();
    dtkApp->window()->setCursor(Qt::ArrowCursor);

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

    QString key = gnomonVisualization::visualizationMesh::pluginFactory().keys()[0];

    if ((!d->formVisualization.contains("gnomonMesh"))||(!d->formVisualization["gnomonMesh"]))
    {
        d->formVisualization["gnomonMesh"] = gnomonVisualization::visualizationMesh::pluginFactory().create(key);
        d->formVisualization["gnomonMesh"]->setView(this);
    }

    gnomonAbstractVisualizationMesh *formVisualizationMesh = (gnomonAbstractVisualizationMesh *)d->formVisualization["gnomonMesh"];
    dtkApp->window()->setCursor(Qt::BusyCursor);
    formVisualizationMesh->setMesh(mesh);
    if (visualization) {
        formVisualizationMesh->setParameters(visualization->parameters());
    }
    formVisualizationMesh->update();
    dtkApp->window()->setCursor(Qt::ArrowCursor);

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

gnomonPointCloudSeries *gnomonViewForm::pointCloud(void)
{
    if (d->forms.contains("gnomonPointCloud")) {
        return dynamic_cast<gnomonPointCloudSeries *>(d->forms["gnomonPointCloud"]);
    } else {
        return nullptr;
    }
}

void gnomonViewForm::setPointCloud(gnomonPointCloudSeries *pointCloud, gnomonAbstractVisualization *visualization)
{
    d->forms["gnomonPointCloud"] = pointCloud;

    QString key = gnomonVisualization::visualizationPointCloud::pluginFactory().keys()[0];

    if ((!d->formVisualization.contains("gnomonPointCloud"))||(!d->formVisualization["gnomonPointCloud"]))
    {
        d->formVisualization["gnomonPointCloud"] = gnomonVisualization::visualizationPointCloud::pluginFactory().create(key);
        d->formVisualization["gnomonPointCloud"]->setView(this);
    }

    gnomonAbstractVisualizationPointCloud *formVisualizationPointCloud = (gnomonAbstractVisualizationPointCloud *)d->formVisualization["gnomonPointCloud"];
    dtkApp->window()->setCursor(Qt::BusyCursor);
    formVisualizationPointCloud->setPointCloud(pointCloud);
    if (visualization) {
        formVisualizationPointCloud->setParameters(visualization->parameters());
    }
    formVisualizationPointCloud->update();
    dtkApp->window()->setCursor(Qt::ArrowCursor);

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
    bool changed = false;

    if (bounds[0] != d->xBounds[0]) {
        d->xBounds[0] = bounds[0];
        changed = true;
    }
    if (bounds[1] != d->xBounds[1]) {
        d->xBounds[1] = bounds[1];
        changed = true;
    }
    if (bounds[2] != d->yBounds[0]) {
        d->yBounds[0] = bounds[2];
        changed = true;
    }
    if (bounds[3] != d->yBounds[1]) {
        d->yBounds[1] = bounds[3];
        changed = true;
    }
    if (bounds[4] != d->zBounds[0]) {
        d->zBounds[0] = bounds[4];
        changed = true;
    }
    if (bounds[5] != d->zBounds[1]) {
        d->zBounds[1] = bounds[5];
        changed = true;
    }

    if (changed) {

        d->c_x = (d->xBounds[0]+d->xBounds[1])/2;
        d->c_y = (d->yBounds[0]+d->yBounds[1])/2;
        d->c_z = (d->zBounds[0]+d->zBounds[1])/2;

        d->cameras.clear();

        switch(d->ori)
        {
            case gnomonViewFormPrivate::SLICE_ORIENTATION_XY:
                d->slice_slider->blockSignals(true);
                d->slice_slider->setMinimum(d->zBounds[0]);
                d->slice_slider->setMaximum(d->zBounds[1]);
                d->slice_slider->setValue(d->c_z);
                d->slice_slider->blockSignals(false);
                break;
            case gnomonViewFormPrivate::SLICE_ORIENTATION_XZ:
                d->slice_slider->blockSignals(true);
                d->slice_slider->setMinimum(d->yBounds[0]);
                d->slice_slider->setMaximum(d->yBounds[1]);
                d->slice_slider->setValue(d->c_y);
                d->slice_slider->blockSignals(false);
                break;
            case gnomonViewFormPrivate::SLICE_ORIENTATION_YZ:
                d->slice_slider->blockSignals(true);
                d->slice_slider->setMinimum(d->xBounds[0]);
                d->slice_slider->setMaximum(d->xBounds[1]);
                d->slice_slider->setValue(d->c_x);
                d->slice_slider->blockSignals(false);
                break;
            default:
                break;
        };

        d->renderer2D->ResetCamera();
//        d->renderer3D->ResetCamera();
    }

}

void gnomonViewForm::setBounds(double xMin, double xMax, double yMin, double yMax, double zMin, double zMax)
{
    double bounds[6];
    bounds[0] = xMin;
    bounds[1] = xMax;
    bounds[2] = yMin;
    bounds[3] = yMax;
    bounds[4] = zMin;
    bounds[5] = zMax;
    this->setBounds(bounds);
}

void gnomonViewForm::getBounds(double bounds[6])
{
    bounds[0] = d->xBounds[0];
    bounds[1] = d->xBounds[1];
    bounds[2] = d->yBounds[0];
    bounds[3] = d->yBounds[1];
    bounds[4] = d->zBounds[0];
    bounds[5] = d->zBounds[1];
}

void gnomonViewForm::setCamera(vtkCamera *cam)
{
    vtkSmartPointer<vtkCamera> camera3D = d->renderer3D->GetActiveCamera();
//    camera3D->DeepCopy(cam);
    camera3D->SetFocalPoint(cam->GetFocalPoint());
    camera3D->SetViewUp(cam->GetViewUp());
    camera3D->SetPosition(cam->GetPosition());
}

void gnomonViewForm::setAcceptForm(const QString& name, bool accept)
{
    if (d->acceptForms.contains(name)) {
        d->acceptForms[name] = accept;
    }
}


void gnomonViewForm::setEnableLinking(bool enable)
{
    d->enableLink = enable;
    d->refresh();
}

void gnomonViewForm::setEnableMenus(bool enable)
{
    d->enableMenus = enable;
    d->refresh();
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

dtkWidgetsMenu *gnomonViewForm::menu(void)
{
    return d->menu();
}

dtkWidgetsMenuBar *gnomonViewForm::menubar(void)
{
    return d->view_menubar;
}

int gnomonViewForm::orientation(void)
{
    return d->ori;
}

void gnomonViewForm::render(void)
{
//    d->renderer2D->ResetCameraClippingRange();
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

void gnomonViewForm::setInputView(bool input)
{
    this->setAcceptDrops(input);
    if (input) {
        d->export_button->changeIcon(fa::arrowcircledown);
        d->export_button->toggle(false);
        d->export_button->activate(false);
    } else {
        d->export_button->changeIcon(fa::arrowcircleup);
        d->export_button->toggle(true);
        d->export_button->activate(true);
    }
}

void gnomonViewForm::setInteractorStyle(gnomonInteractorStyle *style)
{
    gnomonInteractorStyle *new_style;
    if (style) {
        new_style = style;
    } else {
        new_style = d->default_style;
    }
    if(d->style) {
        d->style->disable();
    }
    d->style = new_style;
    this->interactor()->SetInteractorStyle(d->style);
    d->style->setView(this);
    if (d->renderer3D_button->isToggled()) {
        d->style->setMode("3D");
        d->style->SetDefaultRenderer(this->renderer3D());
    } else {
        d->style->setMode("2D");
        d->style->SetDefaultRenderer(this->renderer2D());
    }
    this->interactor()->Enable();
    d->updateKeys();
}

void gnomonViewForm::updateShortcutKeys(void)
{
    d->updateKeys();
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

    if (path.startsWith(":")) {
        int form_index = path.remove(":").toInt();
        gnomonAbstractDynamicForm *form = gnomonFormManager::instance()->get(form_index);
        if (d->empty) {
            if (vtkCamera *cam = gnomonFormManager::instance()->getCamera(form_index)) {
                this->setCamera(cam);
            }
        }
        this->setForm("formManager", form, gnomonFormManager::instance()->getVisualization(form_index));

        event->accept();
    } else if (path.startsWith("file://")) {
        emit fileDropped(path);
        event->accept();
    }
    // ///////////////////////////////////////////////////////////////

}

void gnomonViewForm::resizeEvent(QResizeEvent *event)
{
      d->style_menubar->setFixedHeight(32*(1+d->available_styles.size())+32);
      d->style_menubar->move(QPoint(0, 32*(1+d->formVisualizationMenus.size())+32));
}

// ///////////////////////////////////////////////////////////////////

#include "gnomonViewForm.moc"

//
// gnomonViewForm.cpp ends here
