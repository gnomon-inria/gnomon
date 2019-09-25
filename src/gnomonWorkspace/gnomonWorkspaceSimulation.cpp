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

#include "gnomonWorkspaceSimulation.h"
#include "gnomonWorkspaceTemplate_p.h"

#include <gnomonCore>
#include <gnomonCore/gnomonCommand/gnomonMesh/gnomonFemSolverCommand>
#include <gnomonWidgets>

#include <gnomonVisualization>

#include <dtkImagingCore>
#include <dtkScript>
#include <dtkWidgets>
#include <dtkWidgetsMenuBar_p.h>
#include <dtkWidgetsMenu+ux.h>

#include <vtkImageData.h>

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonWorkspaceSimulationPrivate : public gnomonWorkspaceTemplatePrivate<gnomonFemSolverCommand>
{
public:
     gnomonWorkspaceSimulationPrivate(void);
    ~gnomonWorkspaceSimulationPrivate(void);

public:
    QString workspace(void) const override;
    QStringList keys(void) const override;

public:
    gnomonViewForm *source = nullptr;
    gnomonViewForm *target = nullptr;

public:
    dtkWidgetsMenu *menu_;

public:
    dtkWidgetsMenuBarContainer *dashboard;
};

gnomonWorkspaceSimulationPrivate::gnomonWorkspaceSimulationPrivate(void) : gnomonWorkspaceTemplatePrivate<gnomonFemSolverCommand>()
{

}

gnomonWorkspaceSimulationPrivate::~gnomonWorkspaceSimulationPrivate(void)
{

}

QString gnomonWorkspaceSimulationPrivate::workspace(void) const
{
    return "FEM Simulation";
}

QStringList gnomonWorkspaceSimulationPrivate::keys(void) const
{
    return gnomonCore::femSolver::pluginFactory().keys();
}

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonWorkspaceSimulation::gnomonWorkspaceSimulation(QWidget *parent) : dtkWidgetsWorkspace(parent)
{
    int stat;
    dtkScriptInterpreterPython::instance()->interpret("import gnomonFemSolver", &stat);

    d = new gnomonWorkspaceSimulationPrivate;

    d->source = new gnomonViewForm(this);
    d->source->setExportColor(gnomonToolBar::registration_color);
    d->source->setAcceptCellComplex(false);

    d->target = new gnomonViewForm(this);
    d->target->setExportColor(gnomonToolBar::registration_color);

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

    this->enter();
}

gnomonWorkspaceSimulation::~gnomonWorkspaceSimulation(void)
{
    delete d;
}

void gnomonWorkspaceSimulation::enter(void)
{
    dtkApp->window()->menubar()->addMenu(d->source->menu());
    dtkApp->window()->menubar()->addMenu(d->target->menu());
    dtkApp->window()->menubar()->addMenu(d->menu_);
    dtkApp->window()->menubar()->touch();
}

void gnomonWorkspaceSimulation::leave(void)
{
    dtkApp->window()->menubar()->removeMenu(d->source->menu());
    dtkApp->window()->menubar()->removeMenu(d->target->menu());
    dtkApp->window()->menubar()->removeMenu(d->menu_);
    dtkApp->window()->menubar()->touch();
}

void gnomonWorkspaceSimulation::apply(void)
{
    Q_ASSERT(d->command);

    d->command->setMesh(d->source->mesh());
    d->command->redo();

    d->target->setMesh(d->command->updatedMesh());
}

void gnomonWorkspaceSimulation::configure(const QString& algorithm)
{
    d->configure(algorithm);
}

//
// gnomonWorkspaceSimulation.cpp ends here
