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
#include <gnomonWidgets>
#include <gnomonWorkspace>

// /////////////////////////////////////////////////////////////////////////////
// TODO: Visualization
// /////////////////////////////////////////////////////////////////////////////

// #include <gnomonVisualization>

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

public:
    QStackedWidget *stack;

public:
    gnomonFormManager *manager;

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

    dtk::widgets::workspace::pluginFactory().record(     "PreProcessing", creator<gnomonWorkspacePreprocess>);
    dtk::widgets::workspace::pluginFactory().record(           "Browser", creator<gnomonWorkspaceBrowser>);
    dtk::widgets::workspace::pluginFactory().record(       "CellComplex", creator<gnomonWorkspaceCellComplexFromCellImage>);
    dtk::widgets::workspace::pluginFactory().record(        "CellFilter", creator<gnomonWorkspaceCellImageFilter>);
    dtk::widgets::workspace::pluginFactory().record("CellQuantification", creator<gnomonWorkspaceCellImageQuantification>);
    dtk::widgets::workspace::pluginFactory().record(            "Fusion", creator<gnomonWorkspaceFusion>);
    dtk::widgets::workspace::pluginFactory().record(           "LSystem", creator<gnomonWorkspaceLSystemSimulator>);
    dtk::widgets::workspace::pluginFactory().record(              "Mesh", creator<gnomonWorkspaceMeshFromImage>);
    dtk::widgets::workspace::pluginFactory().record(        "PointCloud", creator<gnomonWorkspacePointCloudFromImage>);
    dtk::widgets::workspace::pluginFactory().record(            "Python", creator<gnomonWorkspacePythonSimulator>);
    dtk::widgets::workspace::pluginFactory().record(      "Registration", creator<gnomonWorkspaceRegistration>);
    dtk::widgets::workspace::pluginFactory().record(        "Simulation", creator<gnomonWorkspaceSimulation>);
    dtk::widgets::workspace::pluginFactory().record(     "Tree Analysis", creator<gnomonWorkspaceTreeAnalysis>);

    dtkApp->setWindow(this);

    d->stack = new QStackedWidget(this);

    QWidget *central = new QWidget(this);

    d->workspace_bar = new dtkWidgetsWorkspaceBar(central);
    d->workspace_bar->setStack(d->stack);
    d->workspace_bar->setDynamic(true);
    d->workspace_bar->buildFromFactory();

    d->manager = gnomonFormManager::instance();

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(d->manager);
    layout->addWidget(d->stack);
    layout->addWidget(d->workspace_bar);

    central->setLayout(layout);

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

    static int l_h = 0;

    connect(d->manager, &gnomonFormManager::expand, [=] (void)
    {
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

    this->setCentralWidget(central);

    d->setup();
    this->populate(); // setup themes
    this->menubar()->touch();
}

void gnomonMainWindow::resizeEvent(QResizeEvent *event)
{
    dtkWidgetsMainWindow::resizeEvent(event);

    this->menubar()->setFixedHeight(event->size().height() - d->workspace_bar->sizeHint().height());
    d->workspace_bar->setFixedWidth(event->size().width() - 16);
}

void gnomonMainWindow::mouseMoveEvent(QMouseEvent *event)
{
    // do not show title bar when over the menu. use hardcoded values of menu :-(
    if (event->pos().x() < 300 + 32 + 12)
        return;
    dtkWidgetsMainWindow::mouseMoveEvent(event);
}


gnomonMainWindow::~gnomonMainWindow(void)
{
    d->setdw();

    delete d;
}

//
// gnomonMainWindow.cpp ends here
