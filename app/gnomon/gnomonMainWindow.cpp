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
    dtk::widgets::workspace::pluginFactory().record( "Image Browser", creator<gnomonWorkspaceBrowser>);
    dtk::widgets::workspace::pluginFactory().record( "Cell Reconstruction", creator<gnomonWorkspaceCellComplexFromCellImage>);
    dtk::widgets::workspace::pluginFactory().record( "Cell Filter", creator<gnomonWorkspaceCellImageFilter>);
    dtk::widgets::workspace::pluginFactory().record( "Cell Analysis", creator<gnomonWorkspaceCellImageQuantification>);
    dtk::widgets::workspace::pluginFactory().record( "Image Fusion", creator<gnomonWorkspaceFusion>);
    dtk::widgets::workspace::pluginFactory().record( "Tree L-Systems", creator<gnomonWorkspaceLSystemSimulator>);
    dtk::widgets::workspace::pluginFactory().record("Cell Surface Meshing", creator<gnomonWorkspaceMeshFromImage>);
    dtk::widgets::workspace::pluginFactory().record( "Cell Detection", creator<gnomonWorkspacePointCloudFromImage>);
    dtk::widgets::workspace::pluginFactory().record( "Python Shell", creator<gnomonWorkspacePythonSimulator>);
    dtk::widgets::workspace::pluginFactory().record( "Image Registration", creator<gnomonWorkspaceRegistration>);
    dtk::widgets::workspace::pluginFactory().record( "Image Segmentation", creator<gnomonWorkspaceSegmentation>);
    dtk::widgets::workspace::pluginFactory().record( "Simulation", creator<gnomonWorkspaceSimulation>);
    dtk::widgets::workspace::pluginFactory().record( "Tree Analysis", creator<gnomonWorkspaceTreeAnalysis>);
    dtk::widgets::workspace::pluginFactory().record( "LString to Tree", creator<gnomonWorkspaceTreeFromLString>);
    dtk::widgets::workspace::pluginFactory().record(  "LString From Tree", creator<gnomonWorkspaceLStringFromTree>);
    dtk::widgets::workspace::pluginFactory().record( "Tree Scanner", creator<gnomonWorkspacePlantScan3D>);

    dtkApp->setWindow(this);

    d->stack = new QStackedWidget(this);

    QWidget *central = new QWidget(this);

    d->workspace_bar = new dtkWidgetsWorkspaceBar(this);
    d->workspace_bar->setStack(d->stack);
    d->workspace_bar->setDynamic(true);
    d->workspace_bar->addWorkspaceInMenu("General", "Image Browser", "Image Browser");
    d->workspace_bar->addWorkspaceInMenu("General", "Python Shell", "Python Shell");

    d->workspace_bar->addWorkspaceInMenu("Tissue Forms", "Cell Analysis", "Cell Analysis");
    d->workspace_bar->addWorkspaceInMenu("Tissue Forms", "Cell Detection", "Cell Detection");
    d->workspace_bar->addWorkspaceInMenu("Tissue Forms", "Cell Reconstruction", "Cell Reconstruction");
    d->workspace_bar->addWorkspaceInMenu("Tissue Forms", "Cell Filter", "Cell Filter");
    d->workspace_bar->addWorkspaceInMenu("Tissue Forms", "Cell Surface Meshing", "Cell Surface Meshing");
    d->workspace_bar->addWorkspaceInMenu("Tissue Forms", "Image Fusion", "Image Fusion");
    d->workspace_bar->addWorkspaceInMenu("Tissue Forms", "Image Preprocessing", "Image Preprocessing");
    d->workspace_bar->addWorkspaceInMenu("Tissue Forms", "Image Segmentation", "Image Segmentation");
    d->workspace_bar->addWorkspaceInMenu("Tissue Forms", "Image Registration", "Image Registation");

    d->workspace_bar->addWorkspaceInMenu("Branching Forms", "Tree Analysis", "Tree Analysis");
    d->workspace_bar->addWorkspaceInMenu("Branching Forms", "Tree L-Systems", "Tree L-Systems");
    d->workspace_bar->addWorkspaceInMenu("Branching Forms", "Tree Scanner", "Tree Scanner");

    d->workspace_bar->addWorkspaceInMenu("Converting Forms", "LString to Tree", "LString to Tree");
    d->workspace_bar->addWorkspaceInMenu("Converting Forms", "LString from Tree", "LString from Tree");

    d->workspace_bar->createWorkspace("Image Browser", "Image Browser", false);

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
