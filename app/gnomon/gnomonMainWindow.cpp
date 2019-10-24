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
#include <gnomonComposer>
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
    gnomonComposerWidget *workflow;

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

    dtk::widgets::workspace::pluginFactory().record(      "PreProcessing", creator<gnomonWorkspacePreprocess>);
    dtk::widgets::workspace::pluginFactory().record(            "Browser", creator<gnomonWorkspaceBrowser>);
    dtk::widgets::workspace::pluginFactory().record("Cell Reconstruction", creator<gnomonWorkspaceCellComplexFromCellImage>);
    dtk::widgets::workspace::pluginFactory().record(         "CellFilter", creator<gnomonWorkspaceCellImageFilter>);
    dtk::widgets::workspace::pluginFactory().record(      "Cell Analysis", creator<gnomonWorkspaceCellImageQuantification>);
    dtk::widgets::workspace::pluginFactory().record(             "Fusion", creator<gnomonWorkspaceFusion>);
    dtk::widgets::workspace::pluginFactory().record(            "LSystem", creator<gnomonWorkspaceLSystemSimulator>);
    dtk::widgets::workspace::pluginFactory().record(    "Surface Meshing", creator<gnomonWorkspaceMeshFromImage>);
    dtk::widgets::workspace::pluginFactory().record(     "Cell Detection", creator<gnomonWorkspacePointCloudFromImage>);
    dtk::widgets::workspace::pluginFactory().record(             "Python", creator<gnomonWorkspacePythonSimulator>);
    dtk::widgets::workspace::pluginFactory().record(       "Registration", creator<gnomonWorkspaceRegistration>);
    dtk::widgets::workspace::pluginFactory().record(       "Segmentation", creator<gnomonWorkspaceSegmentation>);
    dtk::widgets::workspace::pluginFactory().record(         "Simulation", creator<gnomonWorkspaceSimulation>);
    dtk::widgets::workspace::pluginFactory().record(      "Tree Analysis", creator<gnomonWorkspaceTreeAnalysis>);
    dtk::widgets::workspace::pluginFactory().record( "LString Conversion", creator<gnomonWorkspaceTreeFromLString>);
    dtk::widgets::workspace::pluginFactory().record(  "LString From Tree", creator<gnomonWorkspaceLStringFromTree>);
    dtk::widgets::workspace::pluginFactory().record(      "Plant Scan 3D", creator<gnomonWorkspacePlantScan3D>);

    dtkApp->setWindow(this);

    d->stack = new QStackedWidget(this);

    QWidget *central = new QWidget(this);

    d->workspace_bar = new dtkWidgetsWorkspaceBar(central);
    d->workspace_bar->setStack(d->stack);
    d->workspace_bar->setDynamic(true);
    d->workspace_bar->buildFromFactory();
    d->workspace_bar->createWorkspace("Browser", "Browser", false);

    d->manager = gnomonFormManager::instance();
    d->workflow = gnomonComposerWidget::instance();

    QVBoxLayout *i_layout = new QVBoxLayout;
    i_layout->setContentsMargins(0, 0, 0, 0);
    i_layout->setSpacing(0);
    i_layout->addWidget(d->manager);
    i_layout->addWidget(d->stack);
    i_layout->addWidget(d->workflow);
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

    connect(d->workflow, &gnomonComposerWidget::expand, [=] (void)
    {
        if(d->stack->height() < 10)
            return;

        int m_h = d->workflow->height();
        int s_h = d->stack->height();

        l_h = s_h;

        QVariantAnimation *animation = new QVariantAnimation(this);
        animation->setDuration(500);
        animation->setStartValue(d->stack->height());
        animation->setEndValue(0);
        animation->setEasingCurve(QEasingCurve::OutQuad);

        connect(animation, &QVariantAnimation::valueChanged, [=] (const QVariant& value) {
            d->stack->setFixedHeight(value.toInt());
            d->workflow->setFixedHeight(m_h + s_h - value.toInt());
        });

        animation->start(QAbstractAnimation::DeleteWhenStopped);
    });

    connect(d->workflow, &gnomonComposerWidget::shrink, [=] (void)
    {
        if(d->workflow->height() < 10)
            return;

        int m_h = d->workflow->height();

        QVariantAnimation *animation = new QVariantAnimation(this);
        animation->setDuration(500);
        animation->setStartValue(0);
        animation->setEndValue(l_h);
        animation->setEasingCurve(QEasingCurve::OutQuad);

        connect(animation, &QVariantAnimation::valueChanged, [=] (const QVariant& value) {
            d->stack->setFixedHeight(value.toInt());
            d->workflow->setFixedHeight(m_h - value.toInt());
        });

        animation->start(QAbstractAnimation::DeleteWhenStopped);
    });

    connect(d->workspace_bar, SIGNAL(created(const QString&)), d->workflow, SLOT(addWorkspace(const QString&)));

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
