#include "gnomonCellComplexConstructorCommand.h"

#include <gnomonCore/gnomonAlgorithm/gnomonCellComplex/gnomonAbstractCellComplexConstructor.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>

class gnomonCellComplexConstructorCommandPrivate
{
public:
    std::shared_ptr<gnomonCellComplexSeries> output = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonCellComplexConstructorCommand::gnomonCellComplexConstructorCommand() : d(new gnomonCellComplexConstructorCommandPrivate)
{
    GNOMON_COMMANDS_INIT(gnomonCellComplexConstructorCommand)

    QStringList keys = gnomonCore::cellComplexConstructor::pluginFactory().keys();
    if (!keys.empty()) {
        this->algorithm_name = keys[0];
        this->action = gnomonCore::cellComplexConstructor::pluginFactory().create(this->algorithm_name);
    }
}

gnomonCellComplexConstructorCommand::~gnomonCellComplexConstructorCommand()
{
    delete d;
}

void gnomonCellComplexConstructorCommand::setAlgorithmName(const QString& algo_name)
{
    this->algorithm_name = algo_name;

        delete this->action;
    this->action = gnomonCore::cellComplexConstructor::pluginFactory().create(algo_name);
}


void gnomonCellComplexConstructorCommand::predo(void) {}
void gnomonCellComplexConstructorCommand::postdo(void)
{
    std::shared_ptr<gnomonCellComplexSeries> cellComplex = ((gnomonAbstractCellComplexConstructor *) this->action)->output();

    if ((!cellComplex)||(cellComplex->times().empty())) {
        d->output = nullptr;
    } else {
        d->output = cellComplex;
    }
}

void gnomonCellComplexConstructorCommand::undo()
{
}

std::shared_ptr<gnomonCellComplexSeries> gnomonCellComplexConstructorCommand::output()
{
    return d->output;
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonCellComplexConstructorCommand::outputs()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs;
    outputs["output"] = this->output();
    return outputs;
}

gnomonAbstractCommand::orderedMap gnomonCellComplexConstructorCommand::outputTypes() {
    orderedMap types;
    types.emplace_back(std::make_pair("output", "gnomonCellComplex"));
    return types;
}

void gnomonCellComplexConstructorCommand::deserializeResults(QJsonObject &serialization) {
    if(!d->output) {
        d->output = std::make_shared<gnomonCellComplexSeries>();
    }
    auto tmp = serialization["output"].toObject();
    d->output->deserialize(tmp);
}

QJsonObject gnomonCellComplexConstructorCommand::serializeResults(void) {
    QJsonObject out;
    out["output"] = d->output->serialize();
    return out;
}

//
// gnomonCellComplexConstructorCommand.cpp ends here
