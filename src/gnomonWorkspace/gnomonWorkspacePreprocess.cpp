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
#include <gnomonVisualization>

#include <dtkCore>
#include <dtkImagingCore>
#include <dtkScript>

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

// public:
//     QStackedWidget *target_stack = nullptr;
//     gnomonMessageBoard *target_message = nullptr;

//     QSplitter *splitter = nullptr;

public:
    gnomonViewFormPool *pool = nullptr;

public:
    QMetaObject::Connection c_o;
    QMetaObject::Connection c_s;

// public:
//     dtkWidgetsMenu *menu_;

// public:
//     dtkWidgetsMenuBarContainer *dashboard;
};

gnomonWorkspacePreprocessPrivate::gnomonWorkspacePreprocessPrivate(void) : gnomonWorkspaceTemplatePrivate< gnomonImageFilterCommand >()
{
    this->command   = new gnomonImageFilterCommand;
    this->algorithm = this->command->algorithmName();
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

// ///////////////////////////////////////////////////////////////////
// gnomonWorkspacePreprocess
// ///////////////////////////////////////////////////////////////////


gnomonWorkspacePreprocess::gnomonWorkspacePreprocess(QObject *parent) : QObject(parent)
{
    loadPluginGroup("imageFilter");
    emit algorithmsLoaded();

    d = new gnomonWorkspacePreprocessPrivate;
    emit parametersChanged();

    d->source = new gnomonViewForm(this);
//    d->source->setExportColor(this->color);
    d->source->setInputView(true);
    d->source->setAcceptForm("gnomonImage",true);

    d->target = new gnomonViewForm(this);
//    d->target->setExportColor(this->color);
    d->target->setAcceptForm("gnomonImage",true);

    connect(d->target, SIGNAL(exportedForm(gnomonAbstractDynamicForm *)), d->pipeline, SLOT(addForm(gnomonAbstractDynamicForm *)));

    d->pool = new gnomonViewFormPool(this);
    d->pool->addView(d->source);
    d->pool->addView(d->target);
    // d->pool->linkAll();

// /////////////////////////////////////////////////////////////////////////////
// NOTE: Stacked target view
// /////////////////////////////////////////////////////////////////////////////

    /*d->target_message = new gnomonMessageBoard(this);
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
    layout->addWidget(d->dashboard);*/

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

    connect(d->source, &gnomonViewForm::formAdded, [=] ()
    {
        if(d->command->input() != d->source->image()) {
            if (d->source->image()) {
                d->command->setInput(d->source->image());
                emit parametersChanged();
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
        emit parametersChanged();
    });

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

    // this->enter();
}

gnomonWorkspacePreprocess::~gnomonWorkspacePreprocess(void)
{
    delete d;
}

QString gnomonWorkspacePreprocess::algoName(void) const
{
    return d->algorithm;
}

QStringList gnomonWorkspacePreprocess::algorithms(void) const
{
    return d->keys();
}

void gnomonWorkspacePreprocess::setAlgoName(const QString& algorithm)
{
    d->configure(algorithm);
}

QJSValue gnomonWorkspacePreprocess::parameters(void)
{
    return dtkCoreParameterCollection(d->command->parameters()).toJSValue(this);
}

gnomonViewForm* gnomonWorkspacePreprocess::source(void) const
{
    return d->source;
}

gnomonViewForm* gnomonWorkspacePreprocess::target(void) const
{
    return d->target;
}

//void gnomonWorkspacePreprocess::enter(void)
//{
////    dtkApp->window()->menubar()->addMenu(d->source->menu());
////    dtkApp->window()->menubar()->addMenu(d->target->menu());
//    dtkApp->window()->menubar()->touch();
//}
//
//void gnomonWorkspacePreprocess::leave(void)
//{
////    dtkApp->window()->menubar()->removeMenu(d->source->menu());
////    dtkApp->window()->menubar()->removeMenu(d->target->menu());
//    dtkApp->window()->menubar()->touch();
//}

void gnomonWorkspacePreprocess::run(void)
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
//        d->target_stack->setCurrentWidget(d->target);
        d->source->setEnableLinking(true);
        d->target->setEnableLinking(true);

        d->registerPipeline();

    } else {
//        d->target_stack->setCurrentWidget(d->target_message);
        d->source->setEnableLinking(false);
        d->target->setEnableLinking(false);
    }
}

//void gnomonWorkspacePreprocess::configure(const QString& algorithm)
//{
//    d->configure(algorithm);
//}
//
//const QColor gnomonWorkspacePreprocess::color = QColor("#4cd964");
//
//bool gnomonWorkspacePreprocess::isEmpty(void)
//{
//    return gnomonWorkspacePreprocessPrivate::isEmpty();
//}

//
// gnomonWorkspacePreprocess.cpp ends here
