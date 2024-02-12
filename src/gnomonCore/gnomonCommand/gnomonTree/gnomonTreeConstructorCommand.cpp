#include "gnomonTreeConstructorCommand.h"

#include <gnomonCore/gnomonAlgorithm/gnomonTree/gnomonAbstractTreeConstructor.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>

class gnomonTreeConstructorCommandPrivate
{
public:
    std::shared_ptr<gnomonTreeSeries> output = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonTreeConstructorCommand::gnomonTreeConstructorCommand() : d(new gnomonTreeConstructorCommandPrivate)
{
    GNOMON_COMMANDS_INIT(gnomonTreeConstructorCommand)

    QStringList keys = gnomonCore::treeConstructor::pluginFactory().keys();
    if (!keys.empty()) {
        this->algorithm_name = keys[0];
        this->action = gnomonCore::treeConstructor::pluginFactory().create(this->algorithm_name);
    }
}

gnomonTreeConstructorCommand::~gnomonTreeConstructorCommand()
{
    delete d;
}

void gnomonTreeConstructorCommand::setAlgorithmName(const QString& algo_name)
{
    this->algorithm_name = algo_name;

        delete this->action;
    this->action = gnomonCore::treeConstructor::pluginFactory().create(algo_name);
}

void gnomonTreeConstructorCommand::predo(void)
{

}

void gnomonTreeConstructorCommand::postdo(void)
{
    std::shared_ptr<gnomonTreeSeries> tree = ((gnomonAbstractTreeConstructor *) this->action)->output();

    if ((!tree)||(tree->times().empty())) {
        d->output = nullptr;
    } else {
        d->output = tree;
    }
}

void gnomonTreeConstructorCommand::undo()
{
}

std::shared_ptr<gnomonTreeSeries> gnomonTreeConstructorCommand::output()
{
    return d->output;
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonTreeConstructorCommand::outputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs;
    outputs["output"] = this->output();
    return outputs;
}

gnomonAbstractCommand::orderedMap gnomonTreeConstructorCommand::outputTypes() {
    orderedMap types;
    types.emplace_back(std::make_pair("output", "gnomonTree"));
    return types;
}

void gnomonTreeConstructorCommand::deserializeResults(QJsonObject &serialization) {
    if(!d->output) {
        d->output = std::make_shared<gnomonTreeSeries>();
    }
    auto tmp = serialization["output"].toObject();
    d->output->deserialize(tmp);
}

QJsonObject gnomonTreeConstructorCommand::serializeResults(void) {
    QJsonObject out;
    out["output"] = d->output->serialize();
    return out;
}

//
// gnomonTreeConstructorCommand.cpp ends here
