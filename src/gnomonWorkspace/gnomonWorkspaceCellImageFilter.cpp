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

#include "gnomonWorkspaceCellImageFilter.h"
#include "gnomonWorkspaceTemplate_p.h"

#include <gnomonCore>
#include <gnomonCore/gnomonCommand/gnomonCellImage/gnomonCellImageFilterCommand>
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

class gnomonWorkspaceCellImageFilterPrivate : public gnomonWorkspaceTemplatePrivate<gnomonCellImageFilterCommand>
{
public:
     gnomonWorkspaceCellImageFilterPrivate(void);
    ~gnomonWorkspaceCellImageFilterPrivate(void);

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

gnomonWorkspaceCellImageFilterPrivate::gnomonWorkspaceCellImageFilterPrivate(void) : gnomonWorkspaceTemplatePrivate< gnomonCellImageFilterCommand >()
{

}

gnomonWorkspaceCellImageFilterPrivate::~gnomonWorkspaceCellImageFilterPrivate(void)
{

}

QString gnomonWorkspaceCellImageFilterPrivate::workspace(void) const
{
    return "Label Postprocess";
}

QStringList gnomonWorkspaceCellImageFilterPrivate::keys(void) const
{
    return gnomonCore::cellImageFilter::pluginFactory().keys();
}

gnomonWorkspaceCellImageFilter::gnomonWorkspaceCellImageFilter(QWidget *parent) : dtkWidgetsWorkspace(parent)
{
    loadPluginGroup("cellImageFilter");
    
    d = new gnomonWorkspaceCellImageFilterPrivate;

    d->source = new gnomonViewForm(this);
    d->source->setExportColor(this->color);
    d->source->setInputView(true);

    d->target = new gnomonViewForm(this);
    d->target->setExportColor(this->color);

    connect(d->target, SIGNAL(exportedForm(gnomonAbstractDynamicForm *)), d->pipeline, SLOT(addForm(gnomonAbstractDynamicForm *)));

    d->pool = new gnomonViewFormPool(this);
    d->pool->addView(d->source);
    d->pool->addView(d->target);
    d->pool->linkAll();

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
        if(d->command->input() != d->source->cellImage()) {

            if (d->source->cellImage()) {
                d->command->setInput(d->source->cellImage());
            }
        } else {
            qDebug() << "Not changed";
        }
        d->configure(d->algorithm);
    });

    connect(d, &gnomonWorkspaceCellImageFilterPrivate::algorithmChanged, [=] (const QString& algorithm)
    {
        if (d->source->cellImage()) {
            d->command->setInput(d->source->cellImage());
        }
        d->configure(algorithm);
    });

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

    this->enter();
}

gnomonWorkspaceCellImageFilter::~gnomonWorkspaceCellImageFilter(void)
{
    delete d;
}

void gnomonWorkspaceCellImageFilter::enter(void)
{
//    dtkApp->window()->menubar()->addMenu(d->source->menu());
//    dtkApp->window()->menubar()->addMenu(d->target->menu());
    dtkApp->window()->menubar()->touch();
}

void gnomonWorkspaceCellImageFilter::leave(void)
{
//    dtkApp->window()->menubar()->removeMenu(d->source->menu());
//    dtkApp->window()->menubar()->removeMenu(d->target->menu());
    dtkApp->window()->menubar()->touch();
}

void gnomonWorkspaceCellImageFilter::apply(void)
{
    Q_ASSERT(d->command);

    if(d->command->input() != d->source->cellImage()) {
        if (d->source->cellImage()) {
            d->command->setInput(d->source->cellImage());
        }
    } else {
        qDebug() << "Not changed";
    }

    d->command->redo();

    if (d->command->output()) {
        d->target->setCellImage(d->command->output());
        d->target->render();
        d->target_stack->setCurrentWidget(d->target);
        d->source->setEnableLinking(true);
        d->target->setEnableLinking(true);

        d->registerPipeline();
    } else {
        d->target_stack->setCurrentWidget(d->target_message);
        d->source->setEnableLinking(false);
        d->target->setEnableLinking(false);
    }
}

void gnomonWorkspaceCellImageFilter::configure(const QString& algorithm)
{
    d->configure(algorithm);
}

const QColor gnomonWorkspaceCellImageFilter::color = QColor("#dc143c");

bool gnomonWorkspaceCellImageFilter::isEmpty(void)
{
    return gnomonWorkspaceCellImageFilterPrivate::isEmpty();
}

//
// gnomonWorkspaceCellImageFilter.cpp ends here
