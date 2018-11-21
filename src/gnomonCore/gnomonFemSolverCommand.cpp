#include "gnomonFemSolverCommand.h"

#include <dtkScript>

#include "gnomonMesh.h"

class gnomonFemSolverCommandPrivate
{
public:
    QMap<QString, QVariant> parameters;

public:
    gnomonMesh* mesh = nullptr;
};

gnomonFemSolverCommand::gnomonFemSolverCommand(const QString& key) : d(new gnomonFemSolverCommandPrivate)
{
    QString command = "import " + key;

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
    this->action->setMesh(d->mesh);

    for (auto it = d->parameters.cbegin(); it != d->parameters.cend(); ++it) {
        this->action->setParameter(it.key(), *it);
    }

    this->action->run();
}

void gnomonFemSolverCommand::undo(void)
{
    this->action->setMesh(nullptr);
}

void gnomonFemSolverCommand::setMesh(gnomonMesh *mesh)
{
    d->mesh = mesh;
}

void gnomonFemSolverCommand::setParameter(const QString& param_name, const QVariant& param_value)
{
    d->parameters[param_name] = param_value;
}

QMap<QString, QVariant> gnomonFemSolverCommand::parameters(void) const
{
    return this->action->parameters();
}

gnomonMesh *gnomonFemSolverCommand::updatedMesh(void)
{
    return this->action->updatedMesh();
}
