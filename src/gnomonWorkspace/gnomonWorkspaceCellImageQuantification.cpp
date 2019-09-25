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

#include "gnomonWorkspaceCellImageQuantification.h"
#include "gnomonWorkspaceTemplate_p.h"

#include <gnomonCore>
#include <gnomonCore/gnomonCommand/gnomonCellImage/gnomonCellImageQuantificationCommand>
#include <gnomonWidgets>
#include <gnomonVisualization>

#include <dtkImagingCore>
#include <dtkScript>
#include <dtkWidgets>
#include <dtkWidgetsMenuBar_p.h>
#include <dtkWidgetsMenu+ux.h>

// ///////////////////////////////////////////////////////////////////
// gnomonWorkspaceCellImageQuantificationPrivate
// ///////////////////////////////////////////////////////////////////

class gnomonWorkspaceCellImageQuantificationPrivate : public gnomonWorkspaceTemplatePrivate<gnomonCellImageQuantificationCommand>
{
public:
     gnomonWorkspaceCellImageQuantificationPrivate(void);
    ~gnomonWorkspaceCellImageQuantificationPrivate(void);

public:
    QString workspace(void) const override;
    QStringList keys(void) const override;

public:
    gnomonViewForm *view = nullptr;
    gnomonViewMatplotlib *mpl_figure = nullptr;

public:
    dtkWidgetsMenu *menu_;

public:
    dtkWidgetsMenuBarContainer *dashboard;

public:
    QVBoxLayout *mpl_layout = nullptr;
    QWidget *mpl_view = nullptr;
};

gnomonWorkspaceCellImageQuantificationPrivate::gnomonWorkspaceCellImageQuantificationPrivate(void) : gnomonWorkspaceTemplatePrivate< gnomonCellImageQuantificationCommand >()
{

}

gnomonWorkspaceCellImageQuantificationPrivate::~gnomonWorkspaceCellImageQuantificationPrivate(void)
{

}

QString gnomonWorkspaceCellImageQuantificationPrivate::workspace(void) const
{
    return "Cell Quantification";
}

QStringList gnomonWorkspaceCellImageQuantificationPrivate::keys(void) const
{
    return gnomonCore::cellImageQuantification::pluginFactory().keys();
}


// ///////////////////////////////////////////////////////////////////
// gnomonWorkspaceCellImageQuantification
// ///////////////////////////////////////////////////////////////////

gnomonWorkspaceCellImageQuantification::gnomonWorkspaceCellImageQuantification(QWidget *parent) : dtkWidgetsWorkspace(parent)
{
    loadPluginGroup("cellImageQuantification");

    d = new gnomonWorkspaceCellImageQuantificationPrivate;

    d->view = new gnomonViewForm(this);
    // d->view->setExportColor(gnomonToolBar::cellImageQuantification_color);

    d->mpl_figure = new gnomonViewMatplotlib(this);

    d->mpl_layout = new QVBoxLayout;
    d->mpl_layout->setContentsMargins(0, 0, 0, 0);
    d->mpl_layout->setSpacing(0);
    d->mpl_layout->addWidget(d->mpl_figure);

    d->mpl_view = new QWidget(this);
    d->mpl_view->setLayout(d->mpl_layout);

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
    layout->addWidget(d->view);
    layout->addWidget(d->mpl_view);
    layout->addWidget(d->dashboard);

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

    d->menu_ = d->menu(this);

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

    connect(d->view, &gnomonViewForm::formAdded, [=] ()
    {
        d->command->setCellImage(d->view->cellImage());
        d->command->setImage(d->view->image());
        d->configure(d->algorithm);
    });

    connect(d, &gnomonWorkspaceCellImageQuantificationPrivate::algorithmChanged, [=] (const QString& algorithm)
    {
        d->command->setCellImage(d->view->cellImage());
        d->command->setImage(d->view->image());
        d->configure(algorithm);
    });

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

    this->enter();
}

gnomonWorkspaceCellImageQuantification::~gnomonWorkspaceCellImageQuantification(void)
{
    delete d;
}

void gnomonWorkspaceCellImageQuantification::enter(void)
{
    dtkApp->window()->menubar()->addMenu(d->view->menu());
    dtkApp->window()->menubar()->addMenu(d->menu_);
    dtkApp->window()->menubar()->touch();
}

void gnomonWorkspaceCellImageQuantification::leave(void)
{
    dtkApp->window()->menubar()->removeMenu(d->view->menu());
    dtkApp->window()->menubar()->removeMenu(d->menu_);
    dtkApp->window()->menubar()->touch();
}

void gnomonWorkspaceCellImageQuantification::apply(void)
{
    Q_ASSERT(d->command);

    d->command->setCellImage(d->view->cellImage());
    d->command->setImage(d->view->image());

    d->command->redo();

    d->view->setCellImage(d->command->cellImage());

    d->mpl_figure->setForm("gnomonDataFrame",d->command->dataFrame());
}

void gnomonWorkspaceCellImageQuantification::configure(const QString& algorithm)
{
    d->configure(algorithm);
}

//
// gnomonWorkspaceCellImageQuantification.cpp ends here
