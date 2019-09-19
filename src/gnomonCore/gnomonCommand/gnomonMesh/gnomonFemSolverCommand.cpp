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
    QMap<QString, QVariant> parameters;

public:
    gnomonMeshSeries* mesh = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonFemSolverCommand::gnomonFemSolverCommand(const QString& key) : d(new gnomonFemSolverCommandPrivate)
{
    QString command = "import gnomonFemSolver";

    int stat;

    dtkScriptInterpreterPython::instance()->interpret(command, &stat);

    Q_ASSERT(stat == dtkScriptInterpreter::Status::Status_Ok);

    this->action = gnomonCore::femSolver::pluginFactory().create(key);

    Q_ASSERT(this->action);
}

gnomonFemSolverCommand::~gnomonFemSolverCommand(void)
{
    delete d;
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

QMap<QString, gnomonCoreParameter *> gnomonFemSolverCommand::parameters(void) const
{
    return this->action->parameters();
}

//
// gnomonFemSolverCommand.cpp ends here
