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

#include "gnomonWorkspaceLStringTranslation.h"
#include "gnomonWorkspaceTemplate_p.h"

#include <gnomonCore>
#include <gnomonCore/gnomonCommand/gnomonLString/gnomonLStringTranslationCommand>
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

class gnomonWorkspaceLStringTranslationPrivate : public gnomonWorkspaceTemplatePrivate<gnomonLStringTranslationCommand>
{
public:
     gnomonWorkspaceLStringTranslationPrivate(void);
    ~gnomonWorkspaceLStringTranslationPrivate(void);

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

gnomonWorkspaceLStringTranslationPrivate::gnomonWorkspaceLStringTranslationPrivate(void) : gnomonWorkspaceTemplatePrivate< gnomonLStringTranslationCommand >()
{

}

gnomonWorkspaceLStringTranslationPrivate::~gnomonWorkspaceLStringTranslationPrivate(void)
{

}

QString gnomonWorkspaceLStringTranslationPrivate::workspace(void) const
{
    return "Tree Form Translation";
}

QStringList gnomonWorkspaceLStringTranslationPrivate::keys(void) const
{
    return gnomonCore::lStringTranslation::pluginFactory().keys();
}

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonWorkspaceLStringTranslation::gnomonWorkspaceLStringTranslation(QWidget *parent) : dtkWidgetsWorkspace(parent)
{
    loadPluginGroup("lStringTranslation");

    d = new gnomonWorkspaceLStringTranslationPrivate;

    d->source = new gnomonViewMatplotlib(this);
    d->source->setAcceptForm("gnomonTree",true);
    d->source->setAcceptForm("gnomonLString",true);

    d->target = new gnomonViewMatplotlib(this);
    d->target->setAcceptForm("gnomonTree",true);
    d->target->setAcceptForm("gnomonLString",true);

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
        if (d->command->inputTree() != dynamic_cast<gnomonTreeSeries *>(d->source->form("gnomonTree"))) {
            if (dynamic_cast<gnomonTreeSeries *>(d->source->form("gnomonTree"))) {
                d->command->setInputTree(dynamic_cast<gnomonTreeSeries *>(d->source->form("gnomonTree")));
            }
        }
        if (d->command->inputLString() != dynamic_cast<gnomonLStringSeries *>(d->source->form("gnomonLString"))) {
            if (dynamic_cast<gnomonLStringSeries *>(d->source->form("gnomonLString"))) {
                d->command->setInputLString(dynamic_cast<gnomonLStringSeries *>(d->source->form("gnomonLString")));
            }
        }

        d->configure(d->algorithm);
    });

    connect(d, &gnomonWorkspaceLStringTranslationPrivate::algorithmChanged, [=] (const QString& algorithm)
    {
        if (dynamic_cast<gnomonTreeSeries *>(d->source->form("gnomonTree"))) {
            d->command->setInputTree(dynamic_cast<gnomonTreeSeries *>(d->source->form("gnomonTree")));
        }
        if (dynamic_cast<gnomonLStringSeries *>(d->source->form("gnomonLString"))) {
            d->command->setInputLString(dynamic_cast<gnomonLStringSeries *>(d->source->form("gnomonLString")));
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

gnomonWorkspaceLStringTranslation::~gnomonWorkspaceLStringTranslation(void)
{
    delete d;
}

void gnomonWorkspaceLStringTranslation::enter(void)
{
    dtkApp->window()->menubar()->touch();
}

void gnomonWorkspaceLStringTranslation::leave(void)
{
    dtkApp->window()->menubar()->touch();
}

void gnomonWorkspaceLStringTranslation::apply(void)
{
    Q_ASSERT(d->command);

    qDebug()<<Q_FUNC_INFO<<d->command->inputTree();
    qDebug()<<Q_FUNC_INFO<<d->command->inputLString();

    if (d->command->inputTree() != dynamic_cast<gnomonTreeSeries *>(d->source->form("gnomonTree"))) {
        if (dynamic_cast<gnomonTreeSeries *>(d->source->form("gnomonTree"))) {
            d->command->setInputTree(dynamic_cast<gnomonTreeSeries *>(d->source->form("gnomonTree")));
        }
    }
    if (d->command->inputLString() != dynamic_cast<gnomonLStringSeries *>(d->source->form("gnomonLString"))) {
        if (dynamic_cast<gnomonLStringSeries *>(d->source->form("gnomonLString"))) {
            d->command->setInputLString(dynamic_cast<gnomonLStringSeries *>(d->source->form("gnomonLString")));
        }
    }

    d->command->redo();

    qDebug()<<Q_FUNC_INFO<<d->command->outputTree();
    if (d->command->outputTree()) {
        d->target->setForm("gnomonTree",d->command->outputTree());
        d->target_stack->setCurrentWidget(d->target);
    }
    qDebug()<<Q_FUNC_INFO<<d->command->outputLString();
    if (d->command->outputLString()) {
        d->target->setForm("gnomonLString",d->command->outputLString());
        d->target_stack->setCurrentWidget(d->target);
    }

    if ((d->command->outputTree() != nullptr) | (d->command->outputLString() != nullptr)) {
        d->registerPipeline();
    } else {
        d->target_stack->setCurrentWidget(d->target_message);
    }
}

void gnomonWorkspaceLStringTranslation::configure(const QString& algorithm)
{
    d->configure(algorithm);
}

const QColor gnomonWorkspaceLStringTranslation::color = QColor("#734906");

bool gnomonWorkspaceLStringTranslation::isEmpty(void)
{
    return gnomonWorkspaceLStringTranslationPrivate::isEmpty();
}

//
// gnomonWorkspaceLStringTranslation.cpp ends here
