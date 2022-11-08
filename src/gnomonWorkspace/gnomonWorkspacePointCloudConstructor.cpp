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

#include "gnomonWorkspacePointCloudConstructor.h"
#include "gnomonWorkspaceTemplate_p.h"

#include <gnomonCore>
#include <gnomonCore/gnomonCommand/gnomonPointCloud/gnomonPointCloudConstructorCommand>
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

class gnomonWorkspacePointCloudConstructorPrivate : public gnomonWorkspaceTemplatePrivate<gnomonPointCloudConstructorCommand>
{
public:
     gnomonWorkspacePointCloudConstructorPrivate(void);
    ~gnomonWorkspacePointCloudConstructorPrivate(void);

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

gnomonWorkspacePointCloudConstructorPrivate::gnomonWorkspacePointCloudConstructorPrivate(void) : gnomonWorkspaceTemplatePrivate< gnomonPointCloudConstructorCommand >()
{

}

gnomonWorkspacePointCloudConstructorPrivate::~gnomonWorkspacePointCloudConstructorPrivate(void)
{

}

QString gnomonWorkspacePointCloudConstructorPrivate::workspace(void) const
{
    return "Point Cloud Constructor";
}

QStringList gnomonWorkspacePointCloudConstructorPrivate::keys(void) const
{
    return gnomonCore::pointCloudConstructor::pluginFactory().keys();
}

gnomonWorkspacePointCloudConstructor::gnomonWorkspacePointCloudConstructor(QWidget *parent) : dtkWidgetsWorkspace(parent)
{
    loadPluginGroup("pointCloudConstructor");

    d = new gnomonWorkspacePointCloudConstructorPrivate;

    d->target = new gnomonViewForm({}, this);
    d->target->setExportColor(this->color);
    d->target->setAcceptForm("gnomonPointCloud",true);

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

gnomonWorkspacePointCloudConstructor::~gnomonWorkspacePointCloudConstructor(void)
{
    delete d;
}

void gnomonWorkspacePointCloudConstructor::enter(void)
{
    dtkApp->window()->menubar()->touch();
}

void gnomonWorkspacePointCloudConstructor::leave(void)
{
    dtkApp->window()->menubar()->touch();
}

void gnomonWorkspacePointCloudConstructor::apply(void)
{
    Q_ASSERT(d->command);


    d->command->redo();

    if (d->command->output()) {
        d->target->setPointCloud(d->command->output());
        d->target->render();
        d->target_stack->setCurrentWidget(d->target);

        d->registerPipeline();
    } else {
        d->target_stack->setCurrentWidget(d->target_message);
    }
}

void gnomonWorkspacePointCloudConstructor::configure(const QString& algorithm)
{
    d->configure(algorithm);
}

const QColor gnomonWorkspacePointCloudConstructor::color = QColor("#9e5fa0");

bool gnomonWorkspacePointCloudConstructor::isEmpty(void)
{
    return gnomonWorkspacePointCloudConstructorPrivate::isEmpty();
}

//
// gnomonWorkspacePointCloudConstructor.cpp ends here
