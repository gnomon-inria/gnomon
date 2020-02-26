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

#include "gnomonWorkspaceTreeConstructor.h"
#include "gnomonWorkspaceTemplate_p.h"

#include <gnomonCore>
#include <gnomonCore/gnomonCommand/gnomonTree/gnomonTreeConstructorCommand>
#include <gnomonWidgets>
#include <gnomonVisualization>

#include <dtkImagingCore>
#include <dtkScript>
#include <dtkWidgets>
#include <dtkWidgetsMenuBar_p.h>
#include <dtkWidgetsMenu+ux.h>

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonWorkspaceTreeConstructorPrivate : public gnomonWorkspaceTemplatePrivate<gnomonTreeConstructorCommand>
{
public:
     gnomonWorkspaceTreeConstructorPrivate(void);
    ~gnomonWorkspaceTreeConstructorPrivate(void);

public:
    QString workspace(void) const override;
    QStringList keys(void) const override;

public:
    gnomonViewMatplotlib *target = nullptr;

public:
    QStackedWidget *target_stack = nullptr;
    gnomonMessageBoard *target_message = nullptr;

    QSplitter *splitter = nullptr;

public:
    dtkWidgetsMenu *menu_;

public:
    dtkWidgetsMenuBarContainer *dashboard;
};

gnomonWorkspaceTreeConstructorPrivate::gnomonWorkspaceTreeConstructorPrivate(void) : gnomonWorkspaceTemplatePrivate< gnomonTreeConstructorCommand >()
{

}

gnomonWorkspaceTreeConstructorPrivate::~gnomonWorkspaceTreeConstructorPrivate(void)
{

}

QString gnomonWorkspaceTreeConstructorPrivate::workspace(void) const
{
    return "Tree Constructor";
}

QStringList gnomonWorkspaceTreeConstructorPrivate::keys(void) const
{
    return gnomonCore::treeConstructor::pluginFactory().keys();
}

gnomonWorkspaceTreeConstructor::gnomonWorkspaceTreeConstructor(QWidget *parent) : dtkWidgetsWorkspace(parent)
{
    loadPluginGroup("treeConstructor");

    d = new gnomonWorkspaceTreeConstructorPrivate;

    d->target = new gnomonViewMatplotlib(this);

// /////////////////////////////////////////////////////////////////////////////
// NOTE: Stacked target view
// /////////////////////////////////////////////////////////////////////////////

    d->target_message = new gnomonMessageBoard(this);
    d->target_message->setMessage("Result will be displayed here");

    d->target_stack = new QStackedWidget(this);
    d->target_stack->addWidget(d->target_message);
    d->target_stack->addWidget(d->target);

    d->splitter = new QSplitter(this);
    d->splitter->addWidget(d->target_stack);

// /////////////////////////////////////////////////////////////////////////////
// NOTE: Dashboard inception
// /////////////////////////////////////////////////////////////////////////////

    d->dashboard = new dtkWidgetsMenuBarContainer(this);
    d->dashboard->navigator->deleteLater();
    d->dashboard->build(QVector<dtkWidgetsMenu *>() << d->menu(this));
    d->dashboard->setFixedWidth(300);

// /////////////////////////////////////////////////////////////////////////////

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(d->splitter);
    layout->addWidget(d->dashboard);

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////


// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

    this->enter();
}

gnomonWorkspaceTreeConstructor::~gnomonWorkspaceTreeConstructor(void)
{
    delete d;
}

void gnomonWorkspaceTreeConstructor::enter(void)
{
    dtkApp->window()->menubar()->touch();
}

void gnomonWorkspaceTreeConstructor::leave(void)
{
    dtkApp->window()->menubar()->touch();
}

void gnomonWorkspaceTreeConstructor::apply(void)
{
    Q_ASSERT(d->command);


    d->command->redo();

    if (d->command->output()) {
        d->target->setForm("gnomonTree",d->command->output());
        d->target_stack->setCurrentWidget(d->target);
    } else {
        d->target_stack->setCurrentWidget(d->target_message);
    }
}

void gnomonWorkspaceTreeConstructor::configure(const QString& algorithm)
{
    d->configure(algorithm);
}

const QColor gnomonWorkspaceTreeConstructor::color = QColor("#9e5fa0");

bool gnomonWorkspaceTreeConstructor::isEmpty(void)
{
    return gnomonWorkspaceTreeConstructorPrivate::isEmpty();
}

//
// gnomonWorkspaceTreeConstructor.cpp ends here
