#include "gnomonCellImageConstructorCommand.h"

#include <gnomonCore/gnomonAlgorithm/gnomonCellImage/gnomonAbstractCellImageConstructor.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>

class gnomonCellImageConstructorCommandPrivate
{
public:
    std::shared_ptr<gnomonCellImageSeries> output = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonCellImageConstructorCommand::gnomonCellImageConstructorCommand() : d(new gnomonCellImageConstructorCommandPrivate)
{
    GNOMON_COMMANDS_INIT(gnomonCellImageConstructorCommand)

    QStringList keys = gnomonCore::cellImageConstructor::pluginFactory().keys();
    if (!keys.empty()) {
        this->algorithm_name = keys[0];
        this->action = gnomonCore::cellImageConstructor::pluginFactory().create(this->algorithm_name);
    }
}

gnomonCellImageConstructorCommand::~gnomonCellImageConstructorCommand()
{
    delete d;
}

void gnomonCellImageConstructorCommand::setAlgorithmName(const QString& algo_name)
{
    this->algorithm_name = algo_name;

        delete this->action;
    this->action = gnomonCore::cellImageConstructor::pluginFactory().create(algo_name);
}

void gnomonCellImageConstructorCommand::predo(void)
{

}

void gnomonCellImageConstructorCommand::postdo(void)
{
    std::shared_ptr<gnomonCellImageSeries> cellImage = ((gnomonAbstractCellImageConstructor *) this->action)->output();

    if ((!cellImage)||(cellImage->times().empty())) {
        d->output = nullptr;
    } else {
        d->output = cellImage;
    }
}

void gnomonCellImageConstructorCommand::undo()
{
}

std::shared_ptr<gnomonCellImageSeries> gnomonCellImageConstructorCommand::output()
{
    return d->output;
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonCellImageConstructorCommand::outputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs;
    outputs["output"] = this->output();
    return outputs;
}

gnomonAbstractCommand::orderedMap gnomonCellImageConstructorCommand::outputTypes() {
    orderedMap types;
    types.emplace_back(std::make_pair("output", "gnomonCellImage"));
    return types;
}

void gnomonCellImageConstructorCommand::deserializeResults(QJsonObject &serialization) {
    if(!d->output) {
        d->output = std::make_shared<gnomonCellImageSeries>();
    }
    auto tmp = serialization["output"].toObject();
    d->output->deserialize(tmp);
}

QJsonObject gnomonCellImageConstructorCommand::serializeResults(void) {
    QJsonObject out;
    out["output"] = d->output->serialize();
    return out;
}

void gnomonCellImageConstructorCommand::clear(void) {
    gnomonAbstractAlgorithmCommand::clear();
    d->output = nullptr;
}

//
// gnomonCellImageConstructorCommand.cpp ends here
