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

#include "gnomonWorkspaceFusion.h"
#include "gnomonWorkspaceTemplate_p.h"

#include <gnomonCore>
#include <gnomonCore/gnomonCommand/gnomonImage/gnomonImageFusionCommand>
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

class gnomonWorkspaceFusionPrivate : public gnomonWorkspaceTemplatePrivate<gnomonImageFusionCommand>
{
public:
    QString workspace(void) const override;
    QStringList keys(void) const override;

public:
    gnomonGridLayout *sources_layout;

public:
    gnomonVtkView *target;

public:
    QStackedWidget *target_stack = nullptr;
    gnomonMessageBoard *target_message = nullptr;

    QSplitter *splitter = nullptr;

public:
    dtkWidgetsMenu *menu_;

public:
    dtkWidgetsMenuBarContainer *dashboard;
};

QString gnomonWorkspaceFusionPrivate::workspace(void) const
{
    return "Image Fusion";
}

QStringList gnomonWorkspaceFusionPrivate::keys(void) const
{
    return gnomonCore::imageFusion::pluginFactory().keys();
}

gnomonWorkspaceFusion::gnomonWorkspaceFusion(QWidget *parent) : dtkWidgetsWorkspace(parent)
{
    loadPluginGroup("imageFusion");

    d = new gnomonWorkspaceFusionPrivate;

    d->sources_layout = new gnomonGridLayout;
    d->sources_layout->addView();

    for(gnomonVtkView *view : d->sources_layout->views()) {
        view->setInputView(true);
        view->setEnableLinking(false);
        view->setAcceptForm("gnomonImage",true);
    }

    d->target = new gnomonVtkView(this);
    d->target->setExportColor(this->color);
    d->target->setMinimumWidth(250);
    d->target->setEnableLinking(false);
    d->target->setAcceptForm("gnomonImage",true);

    connect(d->target, SIGNAL(exportedForm(gnomonAbstractDynamicForm *)), d->pipeline_manager, SLOT(addForm(gnomonAbstractDynamicForm *)));

    QWidget *dummy = new QWidget(this);
    dummy->setLayout(d->sources_layout);

// /////////////////////////////////////////////////////////////////////////////
// NOTE: Stacked target view
// /////////////////////////////////////////////////////////////////////////////

    d->target_message = new gnomonMessageBoard(this);
    d->target_message->setMessage("Result will be displayed here");

    d->target_stack = new QStackedWidget(this);
    d->target_stack->addWidget(d->target_message);
    d->target_stack->addWidget(d->target);

    d->splitter = new QSplitter(this);
    d->splitter->addWidget(dummy);
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

    connect(d->sources_layout, &gnomonGridLayout::formAdded, [=] ()
    {
        d->command->undo();
        d->target_message->setMessage("Result will be displayed here");
        for(gnomonVtkView *view : d->sources_layout->views()) {
            if (view->image()) {
                d->command->addImage(view->image());
                d->target_message->setMessage("Result will be displayed here");
            }
        }
        d->configure(d->algorithm);
    });

    connect(d->sources_layout, &gnomonGridLayout::viewAdded, [=] (gnomonVtkView *view)
    {
        view->setInputView(true);
        view->setEnableLinking(false);
        view->setAcceptForm("gnomonImage",true);
    });


    connect(d, &gnomonWorkspaceFusionPrivate::algorithmChanged, [=] (const QString& algorithm) {
        d->command->undo();
        for(gnomonVtkView *view : d->sources_layout->views()) {
            if (view->image()) {
                d->command->addImage(view->image());
            }
        }
        d->configure(algorithm);
    });

// /////////////////////////////////////////////////////////////////////////////
// TODO: Later on ...
// /////////////////////////////////////////////////////////////////////////////

//  connect(d->command, SIGNAL(finished()), this, SIGNAL(finished()));

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

    this->enter();
}

gnomonWorkspaceFusion::~gnomonWorkspaceFusion(void)
{
    delete d;
}

void gnomonWorkspaceFusion::enter(void)
{
}

void gnomonWorkspaceFusion::leave(void)
{
}

void gnomonWorkspaceFusion::apply(void)
{
    if(d->sources_layout->views().isEmpty())
        return;

    //d->command->removeLandmarks();
    d->command->undo();

    for(gnomonVtkView *view : d->sources_layout->views()) {
        if (view->image()) {
            d->command->addImage(view->image());
//        d->command->addLandmarks(view->landmarks());
        }
    }

    d->command->redo();

    if (d->command->output()) {
        d->target->setForm("gnomonImage", d->command->output());
        d->target_stack->setCurrentWidget(d->target);

        d->registerPipeline();
    } else {
        d->target_stack->setCurrentWidget(d->target_message);
    }
}

void gnomonWorkspaceFusion::configure(const QString& algorithm)
{
    d->configure(algorithm);
}

const QColor gnomonWorkspaceFusion::color = QColor("#ff9500");

bool gnomonWorkspaceFusion::isEmpty(void)
{
    return gnomonWorkspaceFusionPrivate::isEmpty();
}

//
// gnomonWorkspaceFusion.cpp ends here
