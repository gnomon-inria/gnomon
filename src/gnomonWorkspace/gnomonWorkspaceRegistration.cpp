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

#include "gnomonWorkspaceRegistration.h"
#include "gnomonWorkspaceTemplate_p.h"

#include <gnomonCore>
#include <gnomonCore/gnomonCommand/gnomonImage/gnomonImageRegistrationCommand>
#include <gnomonVisualization>

#include <dtkImagingCore>
#include <dtkScript>

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonWorkspaceRegistrationPrivate : public gnomonWorkspaceTemplatePrivate<gnomonImageRegistrationCommand>
{
public:
    QString workspace(void) const override;
    QStringList keys(void) const override;

public:
    gnomonViewFormList *sources;
    gnomonViewForm *target = nullptr;

/* public:
    QStackedWidget *target_stack = nullptr;
    gnomonMessageBoard *target_message = nullptr;

    QSplitter *splitter = nullptr; */

public:
    gnomonViewFormPool *pool = nullptr;

/* public:
    dtkWidgetsMenu *menu_;

public:
    dtkWidgetsMenuBarContainer *dashboard; */
};

QString gnomonWorkspaceRegistrationPrivate::workspace(void) const
{
    return "Time Registration";
}

QStringList gnomonWorkspaceRegistrationPrivate::keys(void) const
{
    return gnomonCore::imageRegistration::pluginFactory().keys();
}

// /////////////////////////////////////////////////////////////////////////////
// gnomonWorkspaceRegistration
// /////////////////////////////////////////////////////////////////////////////

gnomonWorkspaceRegistration::gnomonWorkspaceRegistration(QObject *parent) : QObject(parent)
{
    loadPluginGroup("imageRegistration");

    d = new gnomonWorkspaceRegistrationPrivate;

    d->sources = new gnomonViewFormList;
    d->sources->addView();
    d->sources->addView();

//    QWidget *sources_dummy = new QWidget(this);
//    sources_dummy->setLayout(d->sources);

    d->target  = new gnomonViewForm(this);
//    d->target->setExportColor(this->color);
    d->target->setAcceptForm("gnomonImage",true);

    connect(d->target, SIGNAL(exportedForm(gnomonAbstractDynamicForm *)), d->pipeline, SLOT(addForm(gnomonAbstractDynamicForm *)));

    d->pool = new gnomonViewFormPool(this);
    for(gnomonViewForm *view : d->sources->views()) {
        view->setInputView(true);
        view->setEnableLinking(false);
        view->setAcceptForm("gnomonImage",true);
        d->pool->addView(view);
    }
    d->pool->addView(d->target);

// /////////////////////////////////////////////////////////////////////////////
// NOTE: Stacked target view
// /////////////////////////////////////////////////////////////////////////////

    /* d->target_message = new gnomonMessageBoard(this);
    d->target_message->setMessage("Result will be displayed here");

    d->target_stack = new QStackedWidget(this);
    d->target_stack->addWidget(d->target_message);
    d->target_stack->addWidget(d->target);

    d->splitter = new QSplitter(this);
    d->splitter->addWidget(sources_dummy);
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
    layout->addWidget(d->dashboard); */

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

    connect(d->sources, &gnomonViewFormList::formAdded, [=] ()
    {
        d->command->undo();
        // d->target_message->setMessage("Result will be displayed here");
        for(gnomonViewForm *view : d->sources->views()) {
            if (view->image()) {
                d->command->addImage(view->image());
                // d->target_message->setMessage("Result will be displayed here");
            }
        }
        d->configure(d->algorithm);
    });

    connect(d->sources, &gnomonViewFormList::viewAdded, [=] (gnomonViewForm *view)
    {
        d->pool->addView(view);
        view->setInputView(true);
        view->setEnableLinking(false);
        view->setAcceptForm("gnomonImage",true);
    });

    connect(d, &gnomonWorkspaceRegistrationPrivate::algorithmChanged, [=] (const QString& algorithm)
    {
        d->command->undo();
        for(gnomonViewForm *view : d->sources->views()) {
            if (view->image()) {
                d->command->addImage(view->image());
            }
        }
        d->configure(algorithm);
    });

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

    // this->enter();
}

gnomonWorkspaceRegistration::~gnomonWorkspaceRegistration(void)
{
    delete d;
}

/* void gnomonWorkspaceRegistration::enter(void)
{
}

void gnomonWorkspaceRegistration::leave(void)
{
} */

QString gnomonWorkspaceRegistration::algoName(void) const
{
    return d->algorithm;
}

QStringList gnomonWorkspaceRegistration::algorithms(void) const
{
    return d->keys();
}

void gnomonWorkspaceRegistration::setAlgoName(const QString& algorithm)
{
    d->configure(algorithm);
}

QJSValue gnomonWorkspaceRegistration::parameters(void)
{
    return dtkCoreParameterCollection(d->command->parameters()).toJSValue(this);
}

gnomonViewFormList *gnomonWorkspaceRegistration::sources(void) const
{
    return d->sources;
}

gnomonViewForm* gnomonWorkspaceRegistration::target(void) const
{
    return d->target;
}

void gnomonWorkspaceRegistration::run(void)
{
    Q_ASSERT(d->command);

    if(d->sources->views().isEmpty())
        return;

    d->command->undo();

    for(gnomonViewForm *view : d->sources->views())
        if (view->image())
            d->command->addImage(view->image());

    d->command->redo();

    if (d->command->output()) {
        d->target->setForm("gnomonImage",d->command->output());
        d->target->render();
//        d->target_stack->setCurrentWidget(d->target);

        d->registerPipeline();
    } else {
//        d->target_stack->setCurrentWidget(d->target_message);
    }
}

/* void gnomonWorkspaceRegistration::configure(const QString& algorithm)
{
    d->configure(algorithm);
}

const QColor gnomonWorkspaceRegistration::color = QColor("#ffcc00");

bool gnomonWorkspaceRegistration::isEmpty(void)
{
    return gnomonWorkspaceRegistrationPrivate::isEmpty();
} */

//
// gnomonWorkspaceRegistration.cpp ends here
