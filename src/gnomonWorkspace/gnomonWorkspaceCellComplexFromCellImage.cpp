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
    loadPluginGroup("gnomonCellComplexFromCellImage");

    d = new gnomonWorkspaceCellComplexFromCellImagePrivate;

    d->source = new gnomonViewForm(this);
    d->source->setExportColor(gnomonToolBar::cellComplexFromCellImage_color);

    d->target = new gnomonViewForm(this);
    d->target->setExportColor(gnomonToolBar::cellComplexFromCellImage_color);

    d->pool = new gnomonViewFormPool(this);
    d->pool->addView(d->source);
    d->pool->addView(d->target);

// /////////////////////////////////////////////////////////////////////////////
// NOTE: Dashboard inception
// /////////////////////////////////////////////////////////////////////////////

    dtkWidgetsMenu *menu_1 = new dtkWidgetsMenu(fa::circlethin, "MainLevel 1");
    dtkWidgetsMenuItem *menuitem_11 = menu_1->addItem(fa::circleo, "Cycle through background");
    menu_1->addItem(fa::circleo, "SubLevel 1-2");
    menu_1->addItem(fa::circleo, "SubLevel 1-3");
    menu_1->addSeparator();
    menu_1->addItem(fa::circleo, "SubLevel 1-4");

    dtkWidgetsMenu *menu_2 = new dtkWidgetsMenu(fa::circlethin, "MainLevel 2");
    menu_2->addItem(fa::circleo, "SubLevel 2-1");

    dtkWidgetsMenu *menu_3 = new dtkWidgetsMenu(fa::circlethin, "MainLevel 3");
    menu_3->addItem(fa::circleo, "Sublevel 3-1");
    menu_3->addItem(fa::circleo, "Sublevel 3-2");

    d->dashboard = new dtkWidgetsMenuBarContainer(this);
    d->dashboard->navigator->deleteLater();
    d->dashboard->build(QVector<dtkWidgetsMenu *>() << menu_1 << menu_2 << menu_3);
    d->dashboard->setFixedWidth(300);

// /////////////////////////////////////////////////////////////////////////////

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(d->source);
    layout->addWidget(d->target);
    layout->addWidget(d->dashboard);

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

    d->menu_ = d->menu(this);

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

    connect(d->source, &gnomonViewForm::formAdded, [=] ()
    {
        if (d->command->input() != d->source->cellImage())
            d->command->setInput(d->source->cellImage());
        else
            qDebug() << "Not changed";

        d->configure(d->algorithm);
    });

    connect(d, &gnomonWorkspaceCellComplexFromCellImagePrivate::algorithmChanged, [=] (const QString& algorithm)
    {
        d->command->setInput(d->source->cellImage());
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
    dtkApp->window()->menubar()->addMenu(d->source->menu());
    dtkApp->window()->menubar()->addMenu(d->target->menu());
    dtkApp->window()->menubar()->addMenu(d->menu_);
    dtkApp->window()->menubar()->touch();
}

void gnomonWorkspaceCellComplexFromCellImage::leave(void)
{
    dtkApp->window()->menubar()->removeMenu(d->source->menu());
    dtkApp->window()->menubar()->removeMenu(d->target->menu());
    dtkApp->window()->menubar()->removeMenu(d->menu_);
    dtkApp->window()->menubar()->touch();
}

void gnomonWorkspaceCellComplexFromCellImage::apply(void)
{
    Q_ASSERT(d->command);

    if(d->command->input() != d->source->cellImage())
        d->command->setInput(d->source->cellImage());
    else
        qDebug() << "Not changed";

    d->command->redo();

    d->target->setCellComplex(d->command->output());
}

void gnomonWorkspaceCellComplexFromCellImage::configure(const QString& algorithm)
{
    d->configure(algorithm);
}

//
// gnomonWorkspaceCellComplexFromCellImage.cpp ends here
