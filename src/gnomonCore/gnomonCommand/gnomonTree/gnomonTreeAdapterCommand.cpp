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

#include "gnomonTreeAdapterCommand.h"

#include <dtkScript>

class gnomonTreeAdapterCommandPrivate
{
public:
    gnomonTreeSeries* input = nullptr;
    gnomonAbstractDynamicForm* output = nullptr;
};

gnomonTreeAdapterCommand::gnomonTreeAdapterCommand() : d(new gnomonTreeAdapterCommandPrivate)
{
    this->factory_name = groupName;
    loadPluginGroup(this->factoryName());

    QStringList keys = gnomonCore::treeAdapter::pluginFactory().keys();
    if (!keys.empty()) {
        this->algorithm_name = keys[0];
        this->action = gnomonCore::treeAdapter::pluginFactory().create(this->algorithm_name);
    }
}

gnomonTreeAdapterCommand::~gnomonTreeAdapterCommand()
{
    delete d;
}

void gnomonTreeAdapterCommand::setAlgorithmName(const QString& algo_name)
{
    this->algorithm_name = algo_name;

        delete this->action;
    this->action = gnomonCore::treeAdapter::pluginFactory().create(algo_name);
}

void gnomonTreeAdapterCommand::predo(void)
{

}

void gnomonTreeAdapterCommand::postdo(void)
{
    qDebug()<<Q_FUNC_INFO<<((gnomonAbstractTreeAdapter *) this->action)->output();
    qDebug()<<Q_FUNC_INFO<<((gnomonAbstractTreeAdapter *) this->action)->output()->times();
    qDebug()<<Q_FUNC_INFO<<((gnomonAbstractTreeAdapter *) this->action)->output()->times().size();
    
    gnomonAbstractDynamicForm *output = ((gnomonAbstractTreeAdapter *) this->action)->output();

    if ((!output)||(output->times().empty())) {
        d->output = nullptr;
    } else {
        d->output = output;
    }
}

void gnomonTreeAdapterCommand::undo()
{
    ((gnomonAbstractTreeAdapter *) this->action)->setInput(nullptr);
}

void gnomonTreeAdapterCommand::setInput(gnomonTreeSeries *input)
{
    if ((!input)||(input->times().empty())) {
        d->input = nullptr;
    } else {
        d->input = input;
        Q_ASSERT(this->action);
        ((gnomonAbstractTreeAdapter *) this->action)->setInput(d->input);
        qDebug()<<Q_FUNC_INFO<<((gnomonAbstractTreeAdapter *) this->action)->input();
    }
}

gnomonTreeSeries *gnomonTreeAdapterCommand::input()
{
    return d->input;
}

gnomonAbstractDynamicForm *gnomonTreeAdapterCommand::output()
{
    return d->output;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonTreeAdapterCommand::inputs()
{
    QMap<QString, gnomonAbstractDynamicForm *> inputs;
    inputs["input"] = this->input();
    return inputs;
}

QMap<QString, gnomonAbstractDynamicForm *> gnomonTreeAdapterCommand::outputs()
{
    QMap<QString, gnomonAbstractDynamicForm *> outputs;
    outputs["output"] = this->output();
    return outputs;
}

bool gnomonTreeAdapterCommand::isEmpty()
{
    return availablePlugins().empty();
}

QStringList gnomonTreeAdapterCommand::availablePlugins() {
    return availablePluginsFromGroup(groupName);
}

gnomonAbstractCommand::orderedMap gnomonTreeAdapterCommand::inputTypes() {
    orderedMap types;
    types.emplace_back(std::make_pair("input", "gnomonTree"));
    return types;
}

gnomonAbstractCommand::orderedMap gnomonTreeAdapterCommand::outputTypes() {
    orderedMap types;
    types.emplace_back(std::make_pair("output", "gnomonAbstractDynamicForm"));
    return types;
}

void gnomonTreeAdapterCommand::setInputForm(const QString &name, gnomonAbstractDynamicForm *form) {
    if (name == "input") {
        this->setInput(dynamic_cast<gnomonTreeSeries *>(form));
    } else {
        dtkWarn()<<Q_FUNC_INFO<<"Unknown input "<< name;
    }
}

//
// gnomonTreeAdapterCommand.cpp ends here
