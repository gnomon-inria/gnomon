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

#include "gnomonWorkspaceLStringFromTree.h"
#include "gnomonWorkspaceTemplate_p.h"

#include <gnomonCore>
#include <gnomonCore/gnomonCommand/gnomonLString/gnomonLStringFromTreeCommand>
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

class gnomonWorkspaceLStringFromTreePrivate : public gnomonWorkspaceTemplatePrivate<gnomonLStringFromTreeCommand>
{
public:
     gnomonWorkspaceLStringFromTreePrivate(void);
    ~gnomonWorkspaceLStringFromTreePrivate(void);

public:
    QString workspace(void) const override;
    QStringList keys(void) const override;

public:
    gnomonViewMatplotlib *source = nullptr;
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

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonWorkspaceLStringFromTreePrivate::gnomonWorkspaceLStringFromTreePrivate(void) : gnomonWorkspaceTemplatePrivate< gnomonLStringFromTreeCommand >()
{

}

gnomonWorkspaceLStringFromTreePrivate::~gnomonWorkspaceLStringFromTreePrivate(void)
{

}

QString gnomonWorkspaceLStringFromTreePrivate::workspace(void) const
{
    return "LString From Tree";
}

QStringList gnomonWorkspaceLStringFromTreePrivate::keys(void) const
{
    return gnomonCore::lStringFromTree::pluginFactory().keys();
}

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonWorkspaceLStringFromTree::gnomonWorkspaceLStringFromTree(QWidget *parent) : dtkWidgetsWorkspace(parent)
{
    loadPluginGroup("lStringFromTree");

    d = new gnomonWorkspaceLStringFromTreePrivate;

    d->source = new gnomonViewMatplotlib(this);

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
    d->splitter->addWidget(d->source);
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

    connect(d->source, &gnomonViewMatplotlib::formAdded, [=] ()
    {
        if (d->command->input() != dynamic_cast<gnomonTreeSeries *>(d->source->form("gnomonTree"))) {
            if (dynamic_cast<gnomonTreeSeries *>(d->source->form("gnomonTree"))) {
                d->command->setInput(dynamic_cast<gnomonTreeSeries *>(d->source->form("gnomonTree")));
            }
        } else {
            qDebug() << "Not changed";
        }

        d->configure(d->algorithm);
    });

    connect(d, &gnomonWorkspaceLStringFromTreePrivate::algorithmChanged, [=] (const QString& algorithm)
    {
        if (dynamic_cast<gnomonTreeSeries *>(d->source->form("gnomonTree"))) {
            d->command->setInput(dynamic_cast<gnomonTreeSeries *>(d->source->form("gnomonTree")));
        }
        d->configure(algorithm);
    });

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

    this->enter();
}

gnomonWorkspaceLStringFromTree::~gnomonWorkspaceLStringFromTree(void)
{
    delete d;
}

void gnomonWorkspaceLStringFromTree::enter(void)
{
//    dtkApp->window()->menubar()->addMenu(d->source->menu());
//    dtkApp->window()->menubar()->addMenu(d->target->menu());
    dtkApp->window()->menubar()->touch();
}

void gnomonWorkspaceLStringFromTree::leave(void)
{
//    dtkApp->window()->menubar()->removeMenu(d->source->menu());
//    dtkApp->window()->menubar()->removeMenu(d->target->menu());
    dtkApp->window()->menubar()->touch();
}

void gnomonWorkspaceLStringFromTree::apply(void)
{
    Q_ASSERT(d->command);

    if (d->command->input() != dynamic_cast<gnomonTreeSeries *>(d->source->form("gnomonTree"))) {
        if (dynamic_cast<gnomonTreeSeries *>(d->source->form("gnomonTree"))) {
            d->command->setInput(dynamic_cast<gnomonTreeSeries *>(d->source->form("gnomonTree")));
        }
    } else {
        qDebug() << "Not changed";
    }

    d->command->redo();

    if (d->command->output()) {
        d->target->setForm("gnomonLString",d->command->output());
        d->target_stack->setCurrentWidget(d->target);
    } else {
        d->target_stack->setCurrentWidget(d->target_message);
    }
}

void gnomonWorkspaceLStringFromTree::configure(const QString& algorithm)
{
    d->configure(algorithm);
}

const QColor gnomonWorkspaceLStringFromTree::color = QColor("209854");

//
// gnomonWorkspaceLStringFromTree.cpp ends here
