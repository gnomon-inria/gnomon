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
    dtkWidgetsWorkspaceBar *workspace_bar = nullptr;

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
public:
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

    dtk::widgets::workspace::pluginFactory().record("Image Preprocessing", creator<gnomonWorkspacePreprocess>);
    dtk::widgets::workspace::pluginFactory().record( "Form Browser", creator<gnomonWorkspaceBrowser>);
    dtk::widgets::workspace::pluginFactory().record( "Cell Meshing", creator<gnomonWorkspaceCellComplexFromCellImage>);
    dtk::widgets::workspace::pluginFactory().record( "Cell Morpho Filter", creator<gnomonWorkspaceCellImageFilter>);
    dtk::widgets::workspace::pluginFactory().record( "Cell Analysis", creator<gnomonWorkspaceCellImageQuantification>);
    dtk::widgets::workspace::pluginFactory().record( "Image Fusion", creator<gnomonWorkspaceFusion>);
    dtk::widgets::workspace::pluginFactory().record( "LPy", creator<gnomonWorkspaceLSystemSimulator>);
    dtk::widgets::workspace::pluginFactory().record("Image Surface Meshing", creator<gnomonWorkspaceMeshFromImage>);
    dtk::widgets::workspace::pluginFactory().record( "Image Cell Detection", creator<gnomonWorkspacePointCloudFromImage>);
    dtk::widgets::workspace::pluginFactory().record( "Python Simulation", creator<gnomonWorkspacePythonSimulator>);
    dtk::widgets::workspace::pluginFactory().record( "Image Registration", creator<gnomonWorkspaceRegistration>);
    dtk::widgets::workspace::pluginFactory().record( "Image Segmentation", creator<gnomonWorkspaceSegmentation>);
    dtk::widgets::workspace::pluginFactory().record( "FEM Simulation", creator<gnomonWorkspaceSimulation>);
    dtk::widgets::workspace::pluginFactory().record( "Tree Analysis", creator<gnomonWorkspaceTreeAnalysis>);
    dtk::widgets::workspace::pluginFactory().record( "LString to Tree", creator<gnomonWorkspaceTreeFromLString>);
    dtk::widgets::workspace::pluginFactory().record(  "Tree to LString", creator<gnomonWorkspaceLStringFromTree>);
    dtk::widgets::workspace::pluginFactory().record( "PlantScan3D", creator<gnomonWorkspacePlantScan3D>);

    dtkApp->setWindow(this);

    d->stack = new QStackedWidget(this);

    const QString package_desc = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Suspendisse laoreet dictum arcu vitae ultrices.";
    const QString workspace_desc = "Duis risus felis, suscipit ut libero id, sollicitudin sagittis est. Etiam blandit feugiat est id luctus. Quisque a ante erat.";

    QWidget *central = new QWidget(this);

    d->workspace_bar = new dtkWidgetsWorkspaceBar(this);
    d->workspace_bar->setStack(d->stack);
    d->workspace_bar->setDynamic(true);
    d->workspace_bar->addWorkspaceInMenu("General", package_desc, "Form Browser",      workspace_desc, "Form Browser");
    d->workspace_bar->addWorkspaceInMenu("General", package_desc, "Python Simulation", workspace_desc, "Python Simulation");

    d->workspace_bar->addWorkspaceInMenu("Tissue Forms", package_desc, "Cell Analysis",         workspace_desc, "Cell Analysis");
    d->workspace_bar->addWorkspaceInMenu("Tissue Forms", package_desc, "Cell Meshing",          workspace_desc, "Cell Meshing");
    d->workspace_bar->addWorkspaceInMenu("Tissue Forms", package_desc, "Cell Morpho Filter",    workspace_desc, "Cell Morpho Filter");
    d->workspace_bar->addWorkspaceInMenu("Tissue Forms", package_desc, "Image Cell Detection",  workspace_desc, "Image Cell Detection");
    d->workspace_bar->addWorkspaceInMenu("Tissue Forms", package_desc, "Image Fusion",          workspace_desc, "Image Fusion");
    d->workspace_bar->addWorkspaceInMenu("Tissue Forms", package_desc, "Image Preprocessing",   workspace_desc, "Image Preprocessing");
    d->workspace_bar->addWorkspaceInMenu("Tissue Forms", package_desc, "Image Segmentation",    workspace_desc, "Image Segmentation");
    d->workspace_bar->addWorkspaceInMenu("Tissue Forms", package_desc, "Image Surface Meshing", workspace_desc, "Image Surface Meshing");
    d->workspace_bar->addWorkspaceInMenu("Tissue Forms", package_desc, "Image Registration",    workspace_desc, "Image Registation");

    // d->workspace_bar->addWorkspaceInMenu("Branching Forms", package_desc, "Tree Analysis", workspace_desc, "Tree Analysis");
    // d->workspace_bar->addWorkspaceInMenu("Branching Forms", package_desc, "LPy",           workspace_desc, "LPy");
    // d->workspace_bar->addWorkspaceInMenu("Branching Forms", package_desc, "PlantScan3D",   workspace_desc, "PlantScan3D");

    // d->workspace_bar->addWorkspaceInMenu("Form Converters", package_desc, "LString to Tree", workspace_desc, "LString to Tree");
    // d->workspace_bar->addWorkspaceInMenu("Form Converters", package_desc, "Tree to LString", workspace_desc, "Tree to LString");

    d->workspace_bar->createWorkspace("Form Browser", "Form Browser", false);

    d->manager = gnomonFormManager::instance();

//    d->workflow = gnomonComposerWidget::instance();

    QVBoxLayout *i_layout = new QVBoxLayout;
    i_layout->setContentsMargins(0, 0, 0, 0);
    i_layout->setSpacing(0);
    i_layout->addWidget(d->manager);
    i_layout->addWidget(d->stack);
//    i_layout->addWidget(d->workflow);
    i_layout->addWidget(d->workspace_bar);

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


    dtkWidgetsMenu *menu_about = this->menubar()->addMenu(fa::questioncircle, "About this workspace");
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
