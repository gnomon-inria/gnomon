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
#include "gnomonViewVolumic.h"
#include "gnomonOverlayPane.h"
#include "gnomonOverlayPaneItem.h"

#include "gnomonWorkspaceTemplate_p.h"

#include <gnomonAbstractFemSolver>
#include <gnomonFemSolverCommand>
#include <gnomonMesh>
#include <dtkImagingCore>
#include <dtkScript>

#include <vtkImageData.h>

class gnomonWorkspaceSimulationPrivate : public gnomonWorkspaceTemplatePrivate< gnomonFemSolverCommand >
{
public:
    gnomonWorkspaceSimulationPrivate();
    virtual ~gnomonWorkspaceSimulationPrivate();

public:
    QString workspace() const override;
    QStringList keys() const override;

public:
    gnomonViewVolumic *source = nullptr;
    gnomonViewVolumic *target = nullptr;
};

gnomonWorkspaceSimulationPrivate::gnomonWorkspaceSimulationPrivate() : gnomonWorkspaceTemplatePrivate< gnomonFemSolverCommand >()
{
}

gnomonWorkspaceSimulationPrivate::~gnomonWorkspaceSimulationPrivate()
{
}

QString gnomonWorkspaceSimulationPrivate::workspace() const
{ return "Simulation"; }

QStringList gnomonWorkspaceSimulationPrivate::keys() const
{
    return gnomonCore::femSolver::pluginFactory().keys();
}

gnomonWorkspaceSimulation::gnomonWorkspaceSimulation(QWidget *parent) : gnomonWorkspace(parent)
{
    int stat;

    dtkScriptInterpreterPython::instance()->interpret("import gnomonFemSolver", &stat);

    d = new gnomonWorkspaceSimulationPrivate;

    d->source = new gnomonViewVolumic(this);
    d->target = new gnomonViewVolumic(this);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(d->source);
    layout->addWidget(d->target);
    layout->addWidget(d->pane(this));
}

gnomonWorkspaceSimulation::~gnomonWorkspaceSimulation(void)
{
    delete d;
}

void gnomonWorkspaceSimulation::apply(void)
{
    qDebug()<<"Apply";
    Q_ASSERT(d->command);
    qDebug()<<"Command OK";
    d->command->setMesh(d->source->mesh());
    d->command->redo();
    // d->target->setMesh((gnomonMesh *)d->command->updatedMesh()->clone());

}

void gnomonWorkspaceSimulation::configure(const QString& algorithm)
{
    d->configure(this, algorithm);
}

//
// gnomonWorkspaceSimulation.cpp ends here
