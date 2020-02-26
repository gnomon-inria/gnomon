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
    loadPluginGroup("femSolver");

    d = new gnomonWorkspaceSimulationPrivate;

    d->source = new gnomonViewForm(this);
    d->source->setExportColor(this->color);
    d->source->setAcceptCellComplex(false);

    d->target = new gnomonViewForm(this);
    d->target->setExportColor(this->color);

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
    layout->addWidget(d->source);
    layout->addWidget(d->target);
    layout->addWidget(d->dashboard);

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
//    dtkApp->window()->menubar()->addMenu(d->source->menu());
//    dtkApp->window()->menubar()->addMenu(d->target->menu());
    dtkApp->window()->menubar()->touch();
}

void gnomonWorkspaceSimulation::leave(void)
{
//    dtkApp->window()->menubar()->removeMenu(d->source->menu());
//    dtkApp->window()->menubar()->removeMenu(d->target->menu());
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

const QColor gnomonWorkspaceSimulation::color = QColor("#5856d6");

bool gnomonWorkspaceSimulation::isEmpty(void)
{
    return gnomonWorkspaceSimulationPrivate::isEmpty();
}

//
// gnomonWorkspaceSimulation.cpp ends here
