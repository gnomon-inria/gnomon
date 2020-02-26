//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#include <gnomonMainWindow.h>

#include <gnomonCore>
#include <gnomonVisualization>
#include <gnomonWidgets>
#include <gnomonWorkspace>

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class gnomonMainWindowPrivate
{
public:
    void setdw(void);
    void setup(void);

public:
    gnomonWidgetsWorkspaceBar *workspace_bar = nullptr;

// /////////////////////////////////////////////////////////////////////////////
// Top level - World
// /////////////////////////////////////////////////////////////////////////////

public:
    gnomonFormManager *manager;

// /////////////////////////////////////////////////////////////////////////////
// Inner level - Workspaces
// /////////////////////////////////////////////////////////////////////////////

public:
    QStackedWidget *stack;

// /////////////////////////////////////////////////////////////////////////////
// Bottom level - Workflow
// /////////////////////////////////////////////////////////////////////////////

// public:
//    gnomonComposerWidget *workflow;

public:
    gnomonMainWindow *q;
};

void gnomonMainWindowPrivate::setup(void)
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "inria", "gnomon");
    q->move(settings.value("position").toPoint());
    q->resize(settings.value("size",QSize(1024,320)).toSize());
}

void gnomonMainWindowPrivate::setdw(void)
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "inria", "gnomon");
    settings.setValue("position", q->pos());
    settings.setValue("size", q->size());
}

template <typename T> dtkWidgetsWorkspace *creator(void)
{
    return new T;
}

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

gnomonMainWindow::gnomonMainWindow(QWidget *parent) : dtkWidgetsMainWindow(parent)
{
    d = new gnomonMainWindowPrivate;
    d->q = this;

    dtk::widgets::workspace::pluginFactory().record( "Image Preprocessing", creator<gnomonWorkspacePreprocess>);
    dtk::widgets::workspace::pluginFactory().record( "Form Browser", creator<gnomonWorkspaceBrowser>);
    dtk::widgets::workspace::pluginFactory().record( "Python Form Algorithm", creator<gnomonWorkspacePythonAlgorithm>);
    dtk::widgets::workspace::pluginFactory().record( "Cell Image Meshing", creator<gnomonWorkspaceCellComplexFromCellImage>);
    dtk::widgets::workspace::pluginFactory().record( "Cell Complex Constructor", creator<gnomonWorkspaceCellComplexConstructor>);
    dtk::widgets::workspace::pluginFactory().record( "Cell Image Morpho Filter", creator<gnomonWorkspaceCellImageFilter>);
    dtk::widgets::workspace::pluginFactory().record( "Cell Image Analysis", creator<gnomonWorkspaceCellImageQuantification>);
    dtk::widgets::workspace::pluginFactory().record( "Cell Image Tracking", creator<gnomonWorkspaceCellImageTracking>);
    dtk::widgets::workspace::pluginFactory().record( "Image Fusion", creator<gnomonWorkspaceFusion>);
    dtk::widgets::workspace::pluginFactory().record( "LPy", creator<gnomonWorkspaceLSystemSimulator>);
    dtk::widgets::workspace::pluginFactory().record( "Image Surface Meshing", creator<gnomonWorkspaceMeshFromImage>);
    dtk::widgets::workspace::pluginFactory().record( "Mesh Constructor", creator<gnomonWorkspaceMeshConstructor>);
    dtk::widgets::workspace::pluginFactory().record( "Mesh Processing", creator<gnomonWorkspaceMeshFilter>);
    dtk::widgets::workspace::pluginFactory().record( "Image Cell Detection", creator<gnomonWorkspacePointCloudFromImage>);
    dtk::widgets::workspace::pluginFactory().record( "Point Cloud Analysis", creator<gnomonWorkspacePointCloudQuantification>);
    dtk::widgets::workspace::pluginFactory().record( "Python Simulation", creator<gnomonWorkspacePythonSimulator>);
    dtk::widgets::workspace::pluginFactory().record( "Image Registration", creator<gnomonWorkspaceRegistration>);
    dtk::widgets::workspace::pluginFactory().record( "Image Segmentation", creator<gnomonWorkspaceSegmentation>);
    dtk::widgets::workspace::pluginFactory().record( "FEM Simulation", creator<gnomonWorkspaceSimulation>);
    dtk::widgets::workspace::pluginFactory().record( "Tree Analysis", creator<gnomonWorkspaceTreeAnalysis>);
    dtk::widgets::workspace::pluginFactory().record( "Tree Constructor", creator<gnomonWorkspaceTreeConstructor>);
    dtk::widgets::workspace::pluginFactory().record( "Form Translation", creator<gnomonWorkspaceTreeFromLString>);
    dtk::widgets::workspace::pluginFactory().record( "Tree to LString", creator<gnomonWorkspaceLStringFromTree>);
    dtk::widgets::workspace::pluginFactory().record( "PlantScan3D", creator<gnomonWorkspacePlantScan3D>);

    dtkApp->setWindow(this);

    d->stack = new QStackedWidget(this);

    const QString workspace_desc = "Duis risus felis, suscipit ut libero id, sollicitudin sagittis est. Etiam blandit feugiat est id luctus. Quisque a ante erat.";

    QWidget *central = new QWidget(this);

    d->workspace_bar = new gnomonWidgetsWorkspaceBar(this);
    d->workspace_bar->setStack(d->stack);
    d->workspace_bar->setDynamic(true);

    const QString general_package_desc = "Workspaces of general interest for form manipulation.";
    const QString browser_desc = QString("Loads forms in Gnomon by reading them from the file system.");
    const QString branchg_desc = QString("Uhhhh.");


    //d->workspace_bar->addWorkspaceInMenu("General", general_package_desc, "Form Browser", "Loads forms of different file formats into the application", "Form Browser", gnomonWorkspaceBrowser::color);
    if (!gnomonWorkspacePythonAlgorithm::isEmpty()) {
        d->workspace_bar->addWorkspaceInMenu("General", general_package_desc, "Python Form Algorithm", "Run a custom algorithm written in Python on any type of forms", "Python Form Algorithm", gnomonWorkspacePythonAlgorithm::color);
    }
    //const QString python_model_desc = QString("Runs a dynamical system model on forms based on a Python code.");
    //d->workspace_bar->addWorkspaceInMenu("General", general_package_desc, "Python Simulation", python_model_desc, "Python Simulation");
    //const QString converter_desc = QString("Casts a form of one type into another type.");
    //d->workspace_bar->addWorkspaceInMenu("General", general_package_desc, "Form Converter", "Provides various plugins to convert forms into other related forms", "Form Converter", gnomonWorkspaceTreeFromLString::color);
    // d->workspace_bar->addWorkspaceInMenu("General", general_package_desc, "Tree to LString", workspace_desc, "Tree to LString");

    const QString tissue_image_package_desc = QString("Workspaces dedicated to the analysis of 3D microscopy images of tissues.");
    if (!gnomonWorkspaceCellImageQuantification::isEmpty()) {
        d->workspace_bar->addWorkspaceInMenu("Tissue Image Forms", tissue_image_package_desc, "Cell Image Analysis", "Computes cell-level quantitative measures on multicellular segmented tissues", "Cell Image Analysis", gnomonWorkspaceCellImageQuantification::color);
    }
    if (!gnomonWorkspaceCellImageFilter::isEmpty()) {
        d->workspace_bar->addWorkspaceInMenu("Tissue Image Forms", tissue_image_package_desc, "Cell Image Morpho Filter", "Processes labelled images through various classsical morphological filters", "Cell Image Morpho Filter", gnomonWorkspaceCellImageFilter::color);
    }
    if (!gnomonWorkspacePointCloudFromImage::isEmpty()) {
        d->workspace_bar->addWorkspaceInMenu("Tissue Image Forms", tissue_image_package_desc, "Image Cell Detection", "Detects cells in a 3D intensity image of tissue as a 3D point cloud.", "Image Cell Detection", gnomonWorkspacePointCloudFromImage::color);
    }
    if (!gnomonWorkspaceFusion::isEmpty()) {
        d->workspace_bar->addWorkspaceInMenu("Tissue Image Forms", tissue_image_package_desc, "Image Fusion", "Fuses images that have been taken of a 3D object from different angles", "Image Fusion", gnomonWorkspaceFusion::color);
    }
    if (!gnomonWorkspacePreprocess::isEmpty()) {
        d->workspace_bar->addWorkspaceInMenu("Tissue Image Forms", tissue_image_package_desc, "Image Preprocessing",   "Provides various plugins to pre-process intensity images (noise, signal enhancement,...)", "Image Preprocessing", gnomonWorkspacePreprocess::color);
    }
    if (!gnomonWorkspaceSegmentation::isEmpty()) {
        d->workspace_bar->addWorkspaceInMenu("Tissue Image Forms", tissue_image_package_desc, "Image Segmentation",    "Performs segmentation of 3D intensity images at cellular resolution.", "Image Segmentation", gnomonWorkspaceSegmentation::color);
    }
    if (!gnomonWorkspaceRegistration::isEmpty()) {
        d->workspace_bar->addWorkspaceInMenu("Tissue Image Forms", tissue_image_package_desc, "Image Registration",  "Provides rigid, affine and non-linear plugins to optimally register close images together", "Image Registration", gnomonWorkspaceRegistration::color);
    }
    if (!gnomonWorkspaceCellImageTracking::isEmpty()) {
        d->workspace_bar->addWorkspaceInMenu("Tissue Image Forms", tissue_image_package_desc, "Cell Image Tracking", "Estimates cell lineaging based on a segmented image series and its original image series", "Cell Image Tracking", gnomonWorkspaceCellImageTracking::color);
    }
    if (!gnomonWorkspacePointCloudQuantification::isEmpty()) {
        d->workspace_bar->addWorkspaceInMenu("Tissue Image Forms", tissue_image_package_desc, "Point Cloud Analysis",  "Computes quantitative measures on each point of a point cloud", "Point Cloud Analysis", gnomonWorkspacePointCloudQuantification::color);
    }

    const QString tissue_mesh_package_desc = QString("Workspaces dedicated to the simulation of 3D geometrical representations of tissues.");
    if (!gnomonWorkspaceCellComplexFromCellImage::isEmpty()) {
        d->workspace_bar->addWorkspaceInMenu("Tissue Mesh Forms", tissue_mesh_package_desc, "Cell Image Meshing", "Provides plugins to mesh the cells of a multicellular segmented tisue", "Cell Image Meshing", gnomonWorkspaceCellComplexFromCellImage::color);
    }
    if (!gnomonWorkspaceCellComplexConstructor::isEmpty()) {
        d->workspace_bar->addWorkspaceInMenu("Tissue Mesh Forms", tissue_mesh_package_desc, "Cell Complex Constructor", "Create a virtual tissue represented by a 2D or 3D cell complex", "Cell Complex Constructor", gnomonWorkspaceCellComplexConstructor::color);
    }
    if (!gnomonWorkspaceMeshFromImage::isEmpty()) {
        d->workspace_bar->addWorkspaceInMenu("Tissue Mesh Forms", tissue_mesh_package_desc, "Image Surface Meshing", "Computes a mesh of the surface of a tissue based on intensity images", "Image Surface Meshing", gnomonWorkspaceMeshFromImage::color);
    }
    if (!gnomonWorkspaceMeshConstructor::isEmpty()) {
        d->workspace_bar->addWorkspaceInMenu("Tissue Mesh Forms", tissue_mesh_package_desc, "Mesh Constructor", "Create a synthetic triangular mesh using various parametric primitives", "Mesh Constructor", gnomonWorkspaceMeshConstructor::color);
    }
    if (!gnomonWorkspaceMeshFilter::isEmpty()) {
        d->workspace_bar->addWorkspaceInMenu("Tissue Mesh Forms", tissue_mesh_package_desc, "Mesh Processing", "Provides plugins to process a 3D triangular mesh (smoothing, subdivision, decimation,...)", "Mesh Processing", gnomonWorkspaceMeshFilter::color);
    }

    const QString branching_package_desc = QString("Workspaces dedicated to the generation and analysis of branching structures.");
    if (!gnomonWorkspaceTreeAnalysis::isEmpty()) {
        d->workspace_bar->addWorkspaceInMenu("Branching Forms", branching_package_desc, "Tree Analysis", workspace_desc, "Tree Analysis", gnomonWorkspaceTreeAnalysis::color);
    }
    if (!gnomonWorkspaceTreeConstructor::isEmpty()) {
        d->workspace_bar->addWorkspaceInMenu("Branching Forms", branching_package_desc, "Tree Constructor", "Create a synthetic branching structure based on random or determinsitc processes", "Tree Constructor", gnomonWorkspaceTreeConstructor::color);
    }
    if (!gnomonWorkspaceTreeFromLString::isEmpty()) {
        d->workspace_bar->addWorkspaceInMenu("Branching Forms", branching_package_desc, "Form Translation", workspace_desc, "Form Translation", gnomonWorkspaceTreeFromLString::color);
    }
    if (!gnomonWorkspaceLSystemSimulator::isEmpty()) {
        d->workspace_bar->addWorkspaceInMenu("Branching Forms", branching_package_desc, "LPy",           workspace_desc, "LPy");
    }
    if (!gnomonWorkspacePlantScan3D::isEmpty()) {
        d->workspace_bar->addWorkspaceInMenu("Branching Forms", branching_package_desc, "PlantScan3D",   workspace_desc, "PlantScan3D");
    }

    d->workspace_bar->createWorkspace("Form Browser", "Form Browser", false);

    d->manager = gnomonFormManager::instance();

//    d->workflow = gnomonComposerWidget::instance();

    QHBoxLayout *b_layout = new QHBoxLayout;
    b_layout->setContentsMargins(0, 0, 32, 0);
    b_layout->addWidget(d->workspace_bar);

    QVBoxLayout *i_layout = new QVBoxLayout;
    i_layout->setContentsMargins(0, 0, 0, 0);
    i_layout->setSpacing(0);
    i_layout->addWidget(d->manager);
    i_layout->addWidget(d->stack);
//    i_layout->addWidget(d->workflow);
    i_layout->addLayout(b_layout);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(this->menubar());
    layout->addLayout(i_layout);

    central->setLayout(layout);

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

    static int l_h = 0;

    connect(d->manager, &gnomonFormManager::expand, [=] (void)
    {
        if(d->stack->height() < 10)
            return;

        int m_h = d->manager->height();
        int s_h = d->stack->height();

        l_h = s_h;

        QVariantAnimation *animation = new QVariantAnimation(this);
        animation->setDuration(500);
        animation->setStartValue(d->stack->height());
        animation->setEndValue(0);
        animation->setEasingCurve(QEasingCurve::OutQuad);

        connect(animation, &QVariantAnimation::valueChanged, [=] (const QVariant& value) {
            d->stack->setFixedHeight(value.toInt());
            d->manager->setFixedHeight(m_h + s_h - value.toInt());
        });

        animation->start(QAbstractAnimation::DeleteWhenStopped);
    });

    connect(d->manager, &gnomonFormManager::shrink, [=] (void)
    {
        if(d->manager->height() < 10)
            return;

        int m_h = d->manager->height();

        QVariantAnimation *animation = new QVariantAnimation(this);
        animation->setDuration(500);
        animation->setStartValue(0);
        animation->setEndValue(l_h);
        animation->setEasingCurve(QEasingCurve::OutQuad);

        connect(animation, &QVariantAnimation::valueChanged, [=] (const QVariant& value) {
            d->stack->setFixedHeight(value.toInt());
            d->manager->setFixedHeight(m_h - value.toInt());
        });

        animation->start(QAbstractAnimation::DeleteWhenStopped);
    });

//    connect(d->workflow, &gnomonComposerWidget::expand, [=] (void)
//    {
//        if(d->stack->height() < 10)
//            return;
//
//        int m_h = d->workflow->height();
//        int s_h = d->stack->height();
//
//        l_h = s_h;
//
//        QVariantAnimation *animation = new QVariantAnimation(this);
//        animation->setDuration(500);
//        animation->setStartValue(d->stack->height());
//        animation->setEndValue(0);
//        animation->setEasingCurve(QEasingCurve::OutQuad);
//
//        connect(animation, &QVariantAnimation::valueChanged, [=] (const QVariant& value) {
//            d->stack->setFixedHeight(value.toInt());
//            d->workflow->setFixedHeight(m_h + s_h - value.toInt());
//        });
//
//        animation->start(QAbstractAnimation::DeleteWhenStopped);
//    });
//
//    connect(d->workflow, &gnomonComposerWidget::shrink, [=] (void)
//    {
//        if(d->workflow->height() < 10)
//            return;
//
//        int m_h = d->workflow->height();
//
//        QVariantAnimation *animation = new QVariantAnimation(this);
//        animation->setDuration(500);
//        animation->setStartValue(0);
//        animation->setEndValue(l_h);
//        animation->setEasingCurve(QEasingCurve::OutQuad);
//
//        connect(animation, &QVariantAnimation::valueChanged, [=] (const QVariant& value) {
//            d->stack->setFixedHeight(value.toInt());
//            d->workflow->setFixedHeight(m_h - value.toInt());
//        });
//
//        animation->start(QAbstractAnimation::DeleteWhenStopped);
//    });

//    connect(d->workspace_bar, SIGNAL(created(const QString&)), d->workflow, SLOT(addWorkspace(const QString&)));

// /////////////////////////////////////////////////////////////////////////////

    this->setWindowFlag(Qt::FramelessWindowHint, true);
    this->setCentralWidget(central);

    d->setup();

    this->populate();

    this->menubar()->touch();

    central->stackUnder(this->titlebar());
}

gnomonMainWindow::~gnomonMainWindow(void)
{
    d->setdw();

    delete d;
}

void gnomonMainWindow::populate(void)
{
    dtkWidgetsMainWindow::populate();

    dtkWidgetsMenu *menu_website = this->menubar()->addMenu(fa::globe, "Gnomon website");
    dtkWidgetsMenuItemDIY *website = new dtkWidgetsMenuItemDIY("Access the documentation");
    menu_website->addItem(website);
//     website->setShowTitle(false);
    website->setSizePolicy(QSizePolicy::Minimum);

    connect(website, &dtkWidgetsMenuItem::clicked, [=] ()
    {
        QDesktopServices::openUrl(QUrl("https://gnomon.gitlabpages.inria.fr/gnomon/"));
    });


//    dtkWidgetsMenu *menu_about = this->menubar()->addMenu(fa::questioncircle, "About this workspace");
}

void gnomonMainWindow::resizeEvent(QResizeEvent *event)
{
    dtkWidgetsMainWindow::resizeEvent(event);
}

void gnomonMainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (event->pos().x() < 300 + 32 + 12)
        return;

    dtkWidgetsMainWindow::mouseMoveEvent(event);
}

//
// gnomonMainWindow.cpp ends here
