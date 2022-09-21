#include "gnomonLStringEvolutionModelCommand.h"

#include <gnomonCore/gnomonModel/gnomonAbstractLStringEvolutionModel.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonLStringEvolutionModelCommandPrivate
{
public:
    std::shared_ptr<gnomonLStringSeries> init_lString = nullptr;
    std::shared_ptr<gnomonLStringSeries> lString = nullptr;
};

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonLStringEvolutionModelCommand::gnomonLStringEvolutionModelCommand() : d(new gnomonLStringEvolutionModelCommandPrivate)
{
    this->factory_name = groupName;
    loadPluginGroup(this->factoryName());

    QStringList keys = gnomonCore::lStringEvolutionModel::pluginFactory().keys();
    if (!keys.empty()) {
        this->model_name = keys[0];
        this->model = gnomonCore::lStringEvolutionModel::pluginFactory().create(this->model_name);
    }
}

gnomonLStringEvolutionModelCommand::~gnomonLStringEvolutionModelCommand()
{
    delete this->model;
    this->model = nullptr;
    delete d;
}

void gnomonLStringEvolutionModelCommand::predo(void)
{
}

void gnomonLStringEvolutionModelCommand::postdo(void)
{
    std::shared_ptr<gnomonLStringSeries> lString = ((gnomonAbstractLStringEvolutionModel *) this->model)->state();
    if ((!lString)||(lString->times().empty())) {
        d->lString = nullptr;
    } else {
        d->lString = lString;
    }
}

void gnomonLStringEvolutionModelCommand::undo()
{
    ((gnomonAbstractLStringEvolutionModel *) this->model)->setInitialState(d->init_lString);
    ((gnomonAbstractLStringEvolutionModel *) this->model)->reset();
    std::shared_ptr<gnomonLStringSeries> lString = ((gnomonAbstractLStringEvolutionModel *) this->model)->state();
    if ((!lString)||(lString->times().empty())) {
        d->lString = nullptr;
    } else {
        d->lString = lString;
    }
}

void gnomonLStringEvolutionModelCommand::redo(void)
{
    Q_ASSERT(this->model);

    this->predo();

    int t = 0;
    std::shared_ptr<gnomonLStringSeries> lString = ((gnomonAbstractLStringEvolutionModel *) this->model)->state();
    if (lString) {
        t = int(lString->times().last());
    }
    this->model->step(t, 1);
    this->postdo();
}

void gnomonLStringEvolutionModelCommand::setInitialState(std::shared_ptr<gnomonLStringSeries> lString)
{
    if ((!lString)||(lString->times().empty())) {
        d->init_lString = nullptr;
    } else {
        d->init_lString = lString;
    }
    ((gnomonAbstractLStringEvolutionModel *) this->model)->setInitialState(d->init_lString);
}

std::shared_ptr<gnomonLStringSeries> gnomonLStringEvolutionModelCommand::initialState(void)
{
    return d->init_lString;
}

std::shared_ptr<gnomonLStringSeries> gnomonLStringEvolutionModelCommand::state(void)
{
    return d->lString;
}

void gnomonLStringEvolutionModelCommand::setLSystem(const QString& code)
{
    ((gnomonAbstractLStringEvolutionModel *) this->model)->setLSystem(code);
}

void gnomonLStringEvolutionModelCommand::setModelName(const QString& model_name)
{
    this->model_name = model_name;
    delete this->model;
    this->model = gnomonCore::lStringEvolutionModel::pluginFactory().create(this->model_name);
}


bool gnomonLStringEvolutionModelCommand::isEmpty()
{
    return availablePlugins().empty();
}

QStringList gnomonLStringEvolutionModelCommand::availablePlugins() {
    return availablePluginsFromGroup(groupName);
}

//
// gnomonLStringEvolutionModelCommand.cpp ends here
