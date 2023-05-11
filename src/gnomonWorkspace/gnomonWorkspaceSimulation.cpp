#include "gnomonWorkspaceSimulation.h"
#include "gnomonAlgorithmWorkspace_p.h"

#include <gnomonCore/gnomonAlgorithm/gnomonMesh/gnomonAbstractFemSolver>
#include <gnomonCore/gnomonCommand/gnomonMesh/gnomonFemSolverCommand>
#include <gnomonCore/gnomonPythonPluginLoader.h>
#include <gnomonConfig.h>

gnomonWorkspaceSimulation::gnomonWorkspaceSimulation(QObject *parent) : gnomonAlgorithmWorkspace(parent)
{
    WORKSPACEINIT(Solver, femSolver, gnomonFemSolverCommand)

    //create the views
    this->addInputView();
    this->addOutputView();

    emit parametersChanged();

    d->updatePool();

}

gnomonWorkspaceSimulation::~gnomonWorkspaceSimulation(void)
{
    auto *command = (gnomonFemSolverCommand *)d->command;
    delete command;
}
