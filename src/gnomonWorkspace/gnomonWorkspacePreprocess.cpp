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

#include "gnomonWorkspacePreprocess.h"
#include "gnomonWorkspaceTemplate_p.h"

#include <gnomonCore>
#include <gnomonCore/gnomonCommand/gnomonImage/gnomonImageFilterCommand>
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

class gnomonWorkspacePreprocessPrivate : public gnomonWorkspaceTemplatePrivate<gnomonImageFilterCommand>
{
public:
     gnomonWorkspacePreprocessPrivate(void);
    ~gnomonWorkspacePreprocessPrivate(void);

public:
    QString workspace(void) const override;
    QStringList keys(void) const override;

public:
    gnomonViewForm *source = nullptr;
    gnomonViewForm *target = nullptr;

public:
    gnomonViewFormPool *pool = nullptr;

public:
    QStackedWidget *target_stack = nullptr;
    gnomonMessageBoard *target_message = nullptr;

    QSplitter *splitter = nullptr;

public:
    dtkWidgetsMenu *menu_;

public:
    dtkWidgetsMenuBarContainer *dashboard;
};

gnomonWorkspacePreprocessPrivate::gnomonWorkspacePreprocessPrivate(void) : gnomonWorkspaceTemplatePrivate< gnomonImageFilterCommand >()
{

}

gnomonWorkspacePreprocessPrivate::~gnomonWorkspacePreprocessPrivate(void)
{

}

QString gnomonWorkspacePreprocessPrivate::workspace(void) const
{
    return "Preprocess";
}

QStringList gnomonWorkspacePreprocessPrivate::keys(void) const
{
    return gnomonCore::imageFilter::pluginFactory().keys();
}

gnomonWorkspacePreprocess::gnomonWorkspacePreprocess(QWidget *parent) : dtkWidgetsWorkspace(parent)
{
    loadPluginGroup("imageFilter");

    d = new gnomonWorkspacePreprocessPrivate;

    d->source = new gnomonViewForm(this);
    d->source->setExportColor(gnomonToolBar::preprocess_color);
    d->source->setInputView(true);

    d->target = new gnomonViewForm(this);
    d->target->setExportColor(gnomonToolBar::preprocess_color);

    d->pool = new gnomonViewFormPool(this);
    d->pool->addView(d->source);
    d->pool->addView(d->target);

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

    connect(d->source, &gnomonViewForm::formAdded, [=] ()
    {
        if(d->command->input() != d->source->image()) {
            if (d->source->image()) {
                d->command->setInput(d->source->image());
            }
        } else {
            qDebug() << "Not changed";
        }

        d->configure(d->algorithm);
    });

    connect(d, &gnomonWorkspacePreprocessPrivate::algorithmChanged, [=] (const QString& algorithm)
    {
        if (d->source->image()) {
            d->command->setInput(d->source->image());
        }
        d->configure(algorithm);
    });

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

    this->enter();
}

gnomonWorkspacePreprocess::~gnomonWorkspacePreprocess(void)
{
    delete d;
}

void gnomonWorkspacePreprocess::enter(void)
{
//    dtkApp->window()->menubar()->addMenu(d->source->menu());
//    dtkApp->window()->menubar()->addMenu(d->target->menu());
    dtkApp->window()->menubar()->touch();
}

void gnomonWorkspacePreprocess::leave(void)
{
//    dtkApp->window()->menubar()->removeMenu(d->source->menu());
//    dtkApp->window()->menubar()->removeMenu(d->target->menu());
    dtkApp->window()->menubar()->touch();
}

void gnomonWorkspacePreprocess::apply(void)
{
    Q_ASSERT(d->command);

    if (d->command->input() != d->source->image()) {
        if (d->source->image()) {
            d->command->setInput(d->source->image());
        }
    } else {
        qDebug() << "Not changed";
    }

    d->command->redo();

    if (d->command->output()) {
        d->target->setImage(d->command->output());
        d->target->render();
        d->target_stack->setCurrentWidget(d->target);
    } else {
        d->target_stack->setCurrentWidget(d->target_message);
    }
}

void gnomonWorkspacePreprocess::configure(const QString& algorithm)
{
    d->configure(algorithm);
}

//
// gnomonWorkspacePreprocess.cpp ends here
