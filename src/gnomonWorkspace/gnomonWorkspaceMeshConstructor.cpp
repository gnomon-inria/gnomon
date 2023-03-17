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

#include "gnomonWorkspaceMeshConstructor.h"
#include "gnomonWorkspaceTemplate_p.h"

#include <gnomonCore>
#include <gnomonCore/gnomonCommand/gnomonMesh/gnomonMeshConstructorCommand>
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

class gnomonWorkspaceMeshConstructorPrivate : public gnomonWorkspaceTemplatePrivate<gnomonMeshConstructorCommand>
{
public:
     gnomonWorkspaceMeshConstructorPrivate(void);
    ~gnomonWorkspaceMeshConstructorPrivate(void);

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

gnomonWorkspaceMeshConstructorPrivate::gnomonWorkspaceMeshConstructorPrivate(void) : gnomonWorkspaceTemplatePrivate< gnomonMeshConstructorCommand >()
{

}

gnomonWorkspaceMeshConstructorPrivate::~gnomonWorkspaceMeshConstructorPrivate(void)
{

}

QString gnomonWorkspaceMeshConstructorPrivate::workspace(void) const
{
    return "Mesh Constructor";
}

QStringList gnomonWorkspaceMeshConstructorPrivate::keys(void) const
{
    return gnomonCore::meshConstructor::pluginFactory().keys();
}

gnomonWorkspaceMeshConstructor::gnomonWorkspaceMeshConstructor(QWidget *parent) : dtkWidgetsWorkspace(parent)
{
    loadPluginGroup("meshConstructor");

    d = new gnomonWorkspaceMeshConstructorPrivate;

    d->target = new gnomonViewForm(this);
    d->target->setNodePortNames({});
    d->target->setExportColor(this->color);
    d->target->setAcceptForm("gnomonMesh",true);

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

gnomonWorkspaceMeshConstructor::~gnomonWorkspaceMeshConstructor(void)
{
    delete d;
}

void gnomonWorkspaceMeshConstructor::enter(void)
{
    dtkApp->window()->menubar()->touch();
}

void gnomonWorkspaceMeshConstructor::leave(void)
{
    dtkApp->window()->menubar()->touch();
}

void gnomonWorkspaceMeshConstructor::apply(void)
{
    Q_ASSERT(d->command);


    d->command->redo();

    if (d->command->output()) {
        d->target->setForm("gnomonMesh", d->command->output());
        d->target->render();
        d->target_stack->setCurrentWidget(d->target);

        d->registerPipeline();
    } else {
        d->target_stack->setCurrentWidget(d->target_message);
    }
}

void gnomonWorkspaceMeshConstructor::configure(const QString& algorithm)
{
    d->configure(algorithm);
}

const QColor gnomonWorkspaceMeshConstructor::color = QColor("#5f9ea0");

bool gnomonWorkspaceMeshConstructor::isEmpty(void)
{
    return gnomonWorkspaceMeshConstructorPrivate::isEmpty();
}

//
// gnomonWorkspaceMeshConstructor.cpp ends here
