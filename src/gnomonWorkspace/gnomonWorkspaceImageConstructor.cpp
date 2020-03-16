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

#include "gnomonWorkspaceImageConstructor.h"
#include "gnomonWorkspaceTemplate_p.h"

#include <gnomonCore>
#include <gnomonCore/gnomonCommand/gnomonImage/gnomonImageConstructorCommand>
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

class gnomonWorkspaceImageConstructorPrivate : public gnomonWorkspaceTemplatePrivate<gnomonImageConstructorCommand>
{
public:
     gnomonWorkspaceImageConstructorPrivate(void);
    ~gnomonWorkspaceImageConstructorPrivate(void);

public:
    QString workspace(void) const override;
    QStringList keys(void) const override;

public:
    gnomonViewForm *target = nullptr;

public:
    QStackedWidget *target_stack = nullptr;
    gnomonMessageBoard *target_message = nullptr;

    QSplitter *splitter = nullptr;

public:
    dtkWidgetsMenu *menu_;

public:
    dtkWidgetsMenuBarContainer *dashboard;
};

gnomonWorkspaceImageConstructorPrivate::gnomonWorkspaceImageConstructorPrivate(void) : gnomonWorkspaceTemplatePrivate< gnomonImageConstructorCommand >()
{

}

gnomonWorkspaceImageConstructorPrivate::~gnomonWorkspaceImageConstructorPrivate(void)
{

}

QString gnomonWorkspaceImageConstructorPrivate::workspace(void) const
{
    return "Image Constructor";
}

QStringList gnomonWorkspaceImageConstructorPrivate::keys(void) const
{
    return gnomonCore::imageConstructor::pluginFactory().keys();
}

gnomonWorkspaceImageConstructor::gnomonWorkspaceImageConstructor(QWidget *parent) : dtkWidgetsWorkspace(parent)
{
    loadPluginGroup("imageConstructor");

    d = new gnomonWorkspaceImageConstructorPrivate;

    d->target = new gnomonViewForm(this);
    d->target->setExportColor(this->color);

    connect(d->target, SIGNAL(exportedForm(gnomonAbstractDynamicForm *)), d->pipeline, SLOT(addForm(gnomonAbstractDynamicForm *)));

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

gnomonWorkspaceImageConstructor::~gnomonWorkspaceImageConstructor(void)
{
    delete d;
}

void gnomonWorkspaceImageConstructor::enter(void)
{
    dtkApp->window()->menubar()->touch();
}

void gnomonWorkspaceImageConstructor::leave(void)
{
    dtkApp->window()->menubar()->touch();
}

void gnomonWorkspaceImageConstructor::apply(void)
{
    Q_ASSERT(d->command);


    d->command->redo();

    if (d->command->output()) {
        d->target->setImage(d->command->output());
        d->target->render();
        d->target_stack->setCurrentWidget(d->target);

        d->registerPipeline();
    } else {
        d->target_stack->setCurrentWidget(d->target_message);
    }
}

void gnomonWorkspaceImageConstructor::configure(const QString& algorithm)
{
    d->configure(algorithm);
}

const QColor gnomonWorkspaceImageConstructor::color = QColor("#9e5fa0");

bool gnomonWorkspaceImageConstructor::isEmpty(void)
{
    return gnomonWorkspaceImageConstructorPrivate::isEmpty();
}

//
// gnomonWorkspaceImageConstructor.cpp ends here
