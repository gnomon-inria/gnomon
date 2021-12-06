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

#include "gnomonFemSolverCommand.h"

#include <dtkScript>

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonFemSolverCommandPrivate
{
public:
    QVariantMap parameters;

public:
    gnomonMeshSeries* mesh = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonFemSolverCommand::gnomonFemSolverCommand(void) : d(new gnomonFemSolverCommandPrivate)
{
    loadPluginGroup("femSolver");

    QStringList keys = gnomonCore::femSolver::pluginFactory().keys();
    if (keys.size() > 0) {
        this->algorithm_name = keys[0];
        this->action = gnomonCore::femSolver::pluginFactory().create(this->algorithm_name);
    }
}

gnomonFemSolverCommand::~gnomonFemSolverCommand(void)
{
    delete d;
}

void gnomonFemSolverCommand::setAlgorithmName(const QString& algo_name)
{
    this->algorithm_name = algo_name;
    if (this->action)
        delete this->action;
    this->action = gnomonCore::femSolver::pluginFactory().create(algo_name);
}

void gnomonFemSolverCommand::redo(void)
{
    Q_ASSERT(this->action);
    qDebug()<<"redo command"<<d->mesh;
    ((gnomonAbstractFemSolver *) this->action)->setMesh(d->mesh);

    this->action->run();
}

void gnomonFemSolverCommand::undo(void)
{
    ((gnomonAbstractFemSolver *) this->action)->setMesh(nullptr);
}

void gnomonFemSolverCommand::setMesh(gnomonMeshSeries *mesh)
{
    d->mesh = mesh;
    qDebug()<<"setmesh"<<d->mesh;
}

void gnomonFemSolverCommand::setParameter(const QString& parameter, const QVariant& value)
{
    this->action->setParameter(parameter, value);
}

gnomonMeshSeries *gnomonFemSolverCommand::updatedMesh(void)
{
    return ((gnomonAbstractFemSolver *) this->action)->updatedMesh();
}

dtkCoreParameters gnomonFemSolverCommand::parameters(void) const
{
    return this->action->parameters();
}

bool gnomonFemSolverCommand::isEmpty(void)
{
    loadPluginGroup("femSolver");
    return gnomonCore::femSolver::pluginFactory().keys().size() == 0;
}

//
// gnomonFemSolverCommand.cpp ends here
