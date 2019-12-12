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

#include "gnomonWorkspaceCellComplexFromCellImage.h"
#include "gnomonWorkspaceTemplate_p.h"

#include <gnomonCore>
#include <gnomonCore/gnomonCommand/gnomonCellComplex/gnomonCellComplexFromCellImageCommand>
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

class gnomonWorkspaceCellComplexFromCellImagePrivate : public gnomonWorkspaceTemplatePrivate<gnomonCellComplexFromCellImageCommand>
{
public:
     gnomonWorkspaceCellComplexFromCellImagePrivate(void);
    ~gnomonWorkspaceCellComplexFromCellImagePrivate(void);

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

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonWorkspaceCellComplexFromCellImagePrivate::gnomonWorkspaceCellComplexFromCellImagePrivate(void) : gnomonWorkspaceTemplatePrivate< gnomonCellComplexFromCellImageCommand >()
{

}

gnomonWorkspaceCellComplexFromCellImagePrivate::~gnomonWorkspaceCellComplexFromCellImagePrivate(void)
{

}

QString gnomonWorkspaceCellComplexFromCellImagePrivate::workspace(void) const
{
    return "Cell Reconstruction";
}

QStringList gnomonWorkspaceCellComplexFromCellImagePrivate::keys(void) const
{
    return gnomonCore::cellComplexFromCellImage::pluginFactory().keys();
}

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonWorkspaceCellComplexFromCellImage::gnomonWorkspaceCellComplexFromCellImage(QWidget *parent) : dtkWidgetsWorkspace(parent)
{
    loadPluginGroup("cellComplexFromCellImage");

    d = new gnomonWorkspaceCellComplexFromCellImagePrivate;

    d->source = new gnomonViewForm(this);
    d->source->setExportColor(gnomonToolBar::cellComplexFromCellImage_color);
    d->source->setInputView(true);

    d->target = new gnomonViewForm(this);
    d->target->setExportColor(gnomonToolBar::cellComplexFromCellImage_color);

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
        if (d->command->input() != d->source->cellImage()) {
            if (d->source->cellImage()) {
                d->command->setInput(d->source->cellImage());
            }
        } else {
            qDebug() << "Not changed";
        }

        d->configure(d->algorithm);
    });

    connect(d, &gnomonWorkspaceCellComplexFromCellImagePrivate::algorithmChanged, [=] (const QString& algorithm)
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

gnomonWorkspaceCellComplexFromCellImage::~gnomonWorkspaceCellComplexFromCellImage(void)
{
    delete d;
}

void gnomonWorkspaceCellComplexFromCellImage::enter(void)
{
//    dtkApp->window()->menubar()->addMenu(d->source->menu());
//    dtkApp->window()->menubar()->addMenu(d->target->menu());
    dtkApp->window()->menubar()->touch();
}

void gnomonWorkspaceCellComplexFromCellImage::leave(void)
{
//    dtkApp->window()->menubar()->removeMenu(d->source->menu());
//    dtkApp->window()->menubar()->removeMenu(d->target->menu());
    dtkApp->window()->menubar()->touch();
}

void gnomonWorkspaceCellComplexFromCellImage::apply(void)
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
        d->target->setCellComplex(d->command->output());
        d->target->render();
        d->target_stack->setCurrentWidget(d->target);
    } else {
        d->target_stack->setCurrentWidget(d->target_message);
    }
}

void gnomonWorkspaceCellComplexFromCellImage::configure(const QString& algorithm)
{
    d->configure(algorithm);
}

//
// gnomonWorkspaceCellComplexFromCellImage.cpp ends here
