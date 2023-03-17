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

#include "gnomonWorkspaceCellComplexConstructor.h"
#include "gnomonWorkspaceTemplate_p.h"

#include <gnomonCore>
#include <gnomonCore/gnomonCommand/gnomonCellComplex/gnomonCellComplexConstructorCommand>
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

class gnomonWorkspaceCellComplexConstructorPrivate : public gnomonWorkspaceTemplatePrivate<gnomonCellComplexConstructorCommand>
{
public:
     gnomonWorkspaceCellComplexConstructorPrivate(void);
    ~gnomonWorkspaceCellComplexConstructorPrivate(void);

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

gnomonWorkspaceCellComplexConstructorPrivate::gnomonWorkspaceCellComplexConstructorPrivate(void) : gnomonWorkspaceTemplatePrivate< gnomonCellComplexConstructorCommand >()
{

}

gnomonWorkspaceCellComplexConstructorPrivate::~gnomonWorkspaceCellComplexConstructorPrivate(void)
{

}

QString gnomonWorkspaceCellComplexConstructorPrivate::workspace(void) const
{
    return "CellComplex Constructor";
}

QStringList gnomonWorkspaceCellComplexConstructorPrivate::keys(void) const
{
    return gnomonCore::cellComplexConstructor::pluginFactory().keys();
}

gnomonWorkspaceCellComplexConstructor::gnomonWorkspaceCellComplexConstructor(QWidget *parent) : dtkWidgetsWorkspace(parent)
{
    loadPluginGroup("cellComplexConstructor");

    d = new gnomonWorkspaceCellComplexConstructorPrivate;

    d->target = new gnomonViewForm(this);
    d->target->setNodePortNames({});
    d->target->setExportColor(this->color);
    d->target->setAcceptForm("gnomonCellComplex",true);

    connect(d->target, SIGNAL(exportedForm(gnomonAbstractDynamicForm *)), d->pipeline_manager, SLOT(addForm(gnomonAbstractDynamicForm *)));

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

gnomonWorkspaceCellComplexConstructor::~gnomonWorkspaceCellComplexConstructor(void)
{
    delete d;
}

void gnomonWorkspaceCellComplexConstructor::enter(void)
{
    dtkApp->window()->menubar()->touch();
}

void gnomonWorkspaceCellComplexConstructor::leave(void)
{
    dtkApp->window()->menubar()->touch();
}

void gnomonWorkspaceCellComplexConstructor::apply(void)
{
    Q_ASSERT(d->command);


    d->command->redo();

    if (d->command->output()) {
        d->target->setForm("gnomonCellComplex", d->command->output());
        d->target->render();
        d->target_stack->setCurrentWidget(d->target);

        d->registerPipeline();
    } else {
        d->target_stack->setCurrentWidget(d->target_message);
    }
}

void gnomonWorkspaceCellComplexConstructor::configure(const QString& algorithm)
{
    d->configure(algorithm);
}

const QColor gnomonWorkspaceCellComplexConstructor::color = QColor("#9e5fa0");

bool gnomonWorkspaceCellComplexConstructor::isEmpty(void)
{
    return gnomonWorkspaceCellComplexConstructorPrivate::isEmpty();
}

//
// gnomonWorkspaceCellComplexConstructor.cpp ends here
