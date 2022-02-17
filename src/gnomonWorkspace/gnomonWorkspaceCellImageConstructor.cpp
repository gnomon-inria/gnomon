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

#include "gnomonWorkspaceCellImageConstructor.h"
#include "gnomonWorkspaceTemplate_p.h"

#include <gnomonCore>
#include <gnomonCore/gnomonCommand/gnomonCellImage/gnomonCellImageConstructorCommand>
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

class gnomonWorkspaceCellImageConstructorPrivate : public gnomonWorkspaceTemplatePrivate<gnomonCellImageConstructorCommand>
{
public:
     gnomonWorkspaceCellImageConstructorPrivate(void);
    ~gnomonWorkspaceCellImageConstructorPrivate(void);

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

gnomonWorkspaceCellImageConstructorPrivate::gnomonWorkspaceCellImageConstructorPrivate(void) : gnomonWorkspaceTemplatePrivate< gnomonCellImageConstructorCommand >()
{

}

gnomonWorkspaceCellImageConstructorPrivate::~gnomonWorkspaceCellImageConstructorPrivate(void)
{

}

QString gnomonWorkspaceCellImageConstructorPrivate::workspace(void) const
{
    return "Cell Image Constructor";
}

QStringList gnomonWorkspaceCellImageConstructorPrivate::keys(void) const
{
    return gnomonCore::cellImageConstructor::pluginFactory().keys();
}

gnomonWorkspaceCellImageConstructor::gnomonWorkspaceCellImageConstructor(QWidget *parent) : dtkWidgetsWorkspace(parent)
{
    loadPluginGroup("cellImageConstructor");

    d = new gnomonWorkspaceCellImageConstructorPrivate;

    d->target = new gnomonViewForm(this);
    d->target->setExportColor(this->color);
    d->target->setAcceptForm("gnomonCellImage",true);

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
// TODO: Later on ...
// /////////////////////////////////////////////////////////////////////////////

//  connect(d->command, SIGNAL(finished()), this, SIGNAL(finished()));

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

    this->enter();
}

gnomonWorkspaceCellImageConstructor::~gnomonWorkspaceCellImageConstructor(void)
{
    delete d;
}

void gnomonWorkspaceCellImageConstructor::enter(void)
{
    dtkApp->window()->menubar()->touch();
}

void gnomonWorkspaceCellImageConstructor::leave(void)
{
    dtkApp->window()->menubar()->touch();
}

void gnomonWorkspaceCellImageConstructor::apply(void)
{
    Q_ASSERT(d->command);


    d->command->redo();

    if (d->command->output()) {
        d->target->setCellImage(d->command->output());
        d->target->render();
        d->target_stack->setCurrentWidget(d->target);

        d->registerPipeline();
    } else {
        d->target_stack->setCurrentWidget(d->target_message);
    }
}

void gnomonWorkspaceCellImageConstructor::configure(const QString& algorithm)
{
    d->configure(algorithm);
}

const QColor gnomonWorkspaceCellImageConstructor::color = QColor("#9e5fa0");

bool gnomonWorkspaceCellImageConstructor::isEmpty(void)
{
    return gnomonWorkspaceCellImageConstructorPrivate::isEmpty();
}

//
// gnomonWorkspaceCellImageConstructor.cpp ends here
