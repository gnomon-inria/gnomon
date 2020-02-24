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

#include "gnomonWidgetsWorkspaceBar.h"

#include <dtkFonts/dtkFontAwesome>

#include <dtkThemes>
#include <dtkWidgets>

#include <QtGui>
#include <QtWidgets>
#include <QtQuick>
#include <QtQuickWidgets>

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class gnomonWidgetsWorkspaceBarMenu : public QQuickWidget
{
    Q_OBJECT

public:
    gnomonWidgetsWorkspaceBarMenu(QVariantMap, QVariantMap, QVariantMap, QWidget *parent = nullptr);

protected:
    void focusOutEvent(QFocusEvent *event) override
    {
        this->close();
        this->deleteLater();

        QQuickWidget::focusOutEvent(event);
    }
};

gnomonWidgetsWorkspaceBarMenu::gnomonWidgetsWorkspaceBarMenu(QVariantMap groups, QVariantMap group_descs, QVariantMap works_descs, QWidget *parent) : QQuickWidget(parent)
{
    this->engine()->addImportPath("qrc:/");

    QQmlContext *context = this->rootContext();
    context->setContextProperty("font", dtkFontAwesome::instance());
    context->setContextProperty("theme", dtkThemesEngine::instance());
    context->setContextProperty("groups", groups);
    context->setContextProperty("group_descs", group_descs);
    context->setContextProperty("works_descs", works_descs);

    this->setResizeMode(QQuickWidget::SizeRootObjectToView);
    this->setSource(QUrl("qrc:/gnomonWidgetsWorkspaceBar.qml"));
    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    this->setFocus(Qt::PopupFocusReason);
}

// ///////////////////////////////////////////////////////////////////
//  gnomonWidgetsWorkspaceBarPrivate
// ///////////////////////////////////////////////////////////////////

class gnomonWidgetsWorkspaceBarPrivate: public QObject
{
    Q_OBJECT

public:
    gnomonWidgetsWorkspaceBar *q;

public:
    bool inside = false;

public:
    QStackedWidget *stack;

public slots:
    void onItemClicked(int);
    void onItemDestroyed(int);

signals:
    void indexDeleted(int);

public:
    void createWorkspace(const QColor &, const QString &, dtkWidgetsWorkspace *workspace, bool display_destroy);

public:
    QMap<QString, QColor> workspace_colors;
    QMap<QString, QString> workspaces;
    QMap<int, dtkWidgetsWorkspace *> workspace_map;

public:
    dtkWidgetsWorkspace *current_workspace = nullptr;

public:
    QVariantMap groups;
    QVariantMap group_descs;
    QVariantMap works_descs;

public:
    gnomonWidgetsWorkspaceBarMenu *menu;
};

void gnomonWidgetsWorkspaceBarPrivate::createWorkspace(const QColor& color, const QString& name, dtkWidgetsWorkspace *workspace,  bool display_destroy)
{
    int index = q->insertTab(q->count() - 1, name);

    q->QTabBar::setCurrentIndex(index);
    q->setTabText(index, name);
    q->setTabTextColor(index, color.isValid() ? color : dtkThemesEngine::instance()->color("@fg"));

    this->workspace_map.insert(index, workspace);

    q->repaint();

    this->current_workspace = workspace;

    if(q->count() > 1)
        q->setTabsClosable(true);

    q->setTabButton(             0, QTabBar::RightSide, 0);
    q->setTabButton(q->count() - 1, QTabBar::RightSide, 0);
}

void gnomonWidgetsWorkspaceBarPrivate::onItemClicked(int index)
{
    if(index == -1)
        return;

    if(index == q->count() - 1) {

        this->menu = new gnomonWidgetsWorkspaceBarMenu(this->groups, this->group_descs, this->works_descs);
        this->menu->setAttribute(Qt::WA_DeleteOnClose, true);
        this->menu->resize(dtkApp->window()->width() * 1/3, dtkApp->window()->height() - 40);
        this->menu->move(dtkApp->window()->frameGeometry().topLeft() + QPoint(86,0));
        this->menu->show();

        QObject *context = this->menu->rootObject();

        connect(context, SIGNAL(clicked(const QString&)), q, SLOT(createWorkspace(const QString&)));

        QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(this->menu);

        return;
    }

    q->emit indexChanged(index);

    this->stack->setCurrentIndex(index);

    dtkWidgetsWorkspace *workspace = dynamic_cast<dtkWidgetsWorkspace *>(this->stack->widget(index));

    if (workspace != this->current_workspace) {

        this->current_workspace->leave();
        this->current_workspace = workspace;
        this->current_workspace->enter();
    }
}

void gnomonWidgetsWorkspaceBarPrivate::onItemDestroyed(int index)
{
    delete this->workspace_map.take(index);

    q->removeTab(index);

    int id = index > 1 ? index - 1 : 0;

    q->QTabBar::setCurrentIndex(id);

    this->current_workspace = dynamic_cast<dtkWidgetsWorkspace *>(this->stack->widget(id));

    for(int i = index; i < workspace_map.count() - 1; i++)
        workspace_map[workspace_map.keys().at(i)] = workspace_map.values().at(i+1);

    emit indexDeleted(index);

    if(q->count() <= 2)
        q->setTabsClosable(false);
}

// ///////////////////////////////////////////////////////////////////
// gnomonWidgetsWorkspaceBar
// ///////////////////////////////////////////////////////////////////

gnomonWidgetsWorkspaceBar::gnomonWidgetsWorkspaceBar(QWidget *parent): QTabBar(parent), d(new gnomonWidgetsWorkspaceBarPrivate)
{
    d->q = this;

    dtkFontAwesome::instance()->initFontAwesome();
    dtkFontAwesome::instance()->setDefaultOption("color", dtkThemesEngine::instance()->color("@fg"));

    this->addTab(dtkFontAwesome::instance()->icon(fa::plussquare).pixmap(40, 40), "");

    QShortcut *nextWS = new QShortcut(QKeySequence(Qt::ControlModifier + Qt::ShiftModifier + Qt::Key_PageDown), this);
    QShortcut *prevWS = new QShortcut(QKeySequence(Qt::ControlModifier + Qt::ShiftModifier + Qt::Key_PageUp), this);

    connect(this, SIGNAL(tabBarClicked(int)), d, SLOT(onItemClicked(int)));
    connect(this, SIGNAL(tabCloseRequested(int)), d, SLOT(onItemDestroyed(int)));
    connect(d, SIGNAL(indexDeleted(int)), this, SIGNAL(indexDeleted(int)));
    connect(nextWS, &QShortcut::activated, [=] (void)
    {
        int count = d->stack->count();
        int index = (d->stack->currentIndex()+1) % count ;
        this->setCurrentIndex(index);
    });
    connect(prevWS, &QShortcut::activated, [=] (void)
    {
        int count = d->stack->count();
        int index = (d->stack->currentIndex()+count-1) % count;
        this->setCurrentIndex(index);
    });

    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    this->setStyleSheet("QTabBar::tab {font-size: 16pt} QTabBar::tab:!selected {background-color: @base2;}");
    this->setMouseTracking(true);
    this->setMovable(false);
    this->setExpanding(false);
    this->setTabsClosable(false);
}

void gnomonWidgetsWorkspaceBar::setStack(QStackedWidget *stack)
{
    d->stack = stack;
}

void gnomonWidgetsWorkspaceBar::setDynamic(bool value)
{
    Q_UNUSED(value);
}

void gnomonWidgetsWorkspaceBar::addWorkspaceInMenu(const QString& group, const QString& group_desciption, const QString& type, const QString& type_description, const QString& workspace_name, const QColor& color)
{
    if (!dtk::widgets::workspace::pluginFactory().keys().contains(type)) {
        qWarning() << "type" << type << "unknown in factory";
        return;
    }

    QString name = workspace_name;

    if (name.isEmpty())
        name = type;

    d->workspaces[type] = name;
    d->workspace_colors[type] = color;

    QStringList list;
    list = d->groups[group].toStringList();
    list << type;

    d->groups[group] = list;

    d->group_descs[group] = group_desciption;
    d->works_descs[type] = type_description;
}

void gnomonWidgetsWorkspaceBar::createWorkspace(const QString& type)
{
    this->createWorkspace(d->workspaces[type], type);

    d->menu->close();
}

void gnomonWidgetsWorkspaceBar::createWorkspace(const QString &name, const QString& type, bool display_destroy)
{
    dtkWidgetsWorkspace *workspace = dtk::widgets::workspace::pluginFactory().create(type);

    workspace->setObjectName(name);

    if (!workspace) {
        qWarning() <<  "can't create workspace of type" << type << dtk::widgets::workspace::pluginFactory().keys();
        return;
    }

    // the leave() must be done BEFORE changing d->current_workspace (which is what d->createWorkspace DOES!!)
    if (d->current_workspace)
        d->current_workspace->leave();

    d->createWorkspace(d->workspace_colors[type], name, workspace, display_destroy);  // this does: d->current_workspace = workspace; 

    d->stack->addWidget(workspace);
    d->stack->setCurrentWidget(workspace);
    workspace->enter();

    emit created(name);
}

gnomonWidgetsWorkspaceBar::~gnomonWidgetsWorkspaceBar(void)
{
    d->workspaces.clear();
    d->workspace_map.clear();

    delete d;
}

void gnomonWidgetsWorkspaceBar::enterEvent(QEvent *)
{
    d->inside = true;

    this->update();
}

void gnomonWidgetsWorkspaceBar::leaveEvent(QEvent *)
{
    d->inside = false;

    this->update();
}

QSize gnomonWidgetsWorkspaceBar::sizeHint(void) const
{
    return QSize(200, 40);
}

QSize gnomonWidgetsWorkspaceBar::tabSizeHint(int index) const
{
    if(index == this->count() - 1) {
        return QSize(40, 40);
    } else {
        return QSize(200, 40);
    }
}

void gnomonWidgetsWorkspaceBar::setCurrentIndex(int i)
{
    d->onItemClicked(i);
    this->QTabBar::setCurrentIndex(i);
}

void gnomonWidgetsWorkspaceBar::buildFromFactory(void)
{
    auto list = dtk::widgets::workspace::pluginFactory().keys();

    list.sort();

    for (const QString &key: list)
        this->addWorkspaceInMenu("All", "", key, "");
}

#include "gnomonWidgetsWorkspaceBar.moc"

//
// gnomonWidgetsWorkspaceBar.cpp ends here
