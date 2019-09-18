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

// /////////////////////////////////////////////////////////////////////////////
// TODO: Script
// /////////////////////////////////////////////////////////////////////////////

// #include <dtkScript>

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

gnomonWorkspaceSimulation::gnomonWorkspaceSimulation(QWidget *parent) : dtkWidgetsWorkspace(parent)
{
    // int stat;

    // dtkScriptInterpreterPython::instance()->interpret("import gnomonFemSolver", &stat);

    d = new gnomonWorkspaceSimulationPrivate;

    d->source = new gnomonViewForm(this);
    d->source->setExportColor(gnomonToolBar::registration_color);
    d->source->setAcceptCellComplex(false);

    d->target = new gnomonViewForm(this);
    d->target->setExportColor(gnomonToolBar::registration_color);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(d->source);
    layout->addWidget(d->target);
    // layout->addWidget(d->pane(this));
}

gnomonWorkspaceSimulation::~gnomonWorkspaceSimulation(void)
{
    delete d;
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
