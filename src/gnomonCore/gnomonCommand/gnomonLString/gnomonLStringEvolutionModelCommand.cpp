#include "gnomonLStringEvolutionModelCommand.h"

#include <gnomonCore/gnomonModel/gnomonAbstractLStringEvolutionModel.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>

#include <QtConcurrent>
#include <QtCore>
// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonLStringEvolutionModelCommandPrivate
{
public:
    std::shared_ptr<gnomonLStringSeries> init_lString = nullptr;
    std::shared_ptr<gnomonLStringSeries> lString = nullptr;

    int derivationLength = 0;
    int animation_step = 1;

    std::unique_ptr<QFutureWatcher<int>> watcher = nullptr;
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
    d->lString = ((gnomonAbstractLStringEvolutionModel *) this->model)->state(); // we get all the serie at each step, we only want the last one for animate
    if ((!d->lString)||(d->lString->times().empty())) {
        d->lString = nullptr;
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

QFuture<int> gnomonLStringEvolutionModelCommand::redo(QMutex* mutex, QWaitCondition* synchro)
{
    Q_ASSERT(this->model);

    d->watcher = std::make_unique<QFutureWatcher<int>>();
    connect(d->watcher.get(), &QFutureWatcher<void>::finished,
            this, &gnomonLStringEvolutionModelCommand::finished);
    if(this->simulationType == SimulationType::animate)
        connect(d->watcher.get(), &QFutureWatcher<void>::progressValueChanged,
                this, &gnomonLStringEvolutionModelCommand::stepFinished);

    QFuture<int> future = QtConcurrent::run([=](QPromise<int> &promise){
        promise.start();
        int maxDerivationLength = this->simulationType == SimulationType::animate ? d->derivationLength : 1;
        promise.setProgressRange(0, maxDerivationLength);
        auto lstring_model = dynamic_cast<gnomonAbstractLStringEvolutionModel *>(this->model);

        int i=0;
        if(this->simulationType == SimulationType::step) {
            d->lString = lstring_model->state();
            i = int(d->lString->times().last());
            maxDerivationLength = i+1;
        }

        for(; i<maxDerivationLength; i++) {
            this->predo();
            if(this->simulationType == SimulationType::run) {
                this->model->run(0, 0, 0);
                this->postdo();
            } else {
                if ((i+1) % d->animation_step == 0) {
                    d->lString->insert(i+1, lstring_model->stepAndReturn(i, 1));
                    promise.setProgressValue(i+1);
                } else {
                    lstring_model->step(i,1);
                }
            }

            promise.suspendIfRequested();
            if (promise.isCanceled())
                return;
        }
        promise.finish();
    }); //.onFailed([] {
    // qWarning() << "Error running " << Q_FUNC_INFO;
    //});

    d->watcher->setFuture(future);
    return future;
}

void gnomonLStringEvolutionModelCommand::setAxiom(std::shared_ptr<gnomonLStringSeries> lString)
{
    if ((!lString)||(lString->times().empty())) {
        d->init_lString = nullptr;
    } else {
        d->init_lString = lString;
    }
    ((gnomonAbstractLStringEvolutionModel *) this->model)->setInitialState(d->init_lString);
}

std::shared_ptr<gnomonLStringSeries> gnomonLStringEvolutionModelCommand::axiom(void)
{
    return d->init_lString;
}

std::shared_ptr<gnomonLStringSeries> gnomonLStringEvolutionModelCommand::lString(void)
{
    return d->lString;
}

void gnomonLStringEvolutionModelCommand::setLSystem(const QString& code)
{
    ((gnomonAbstractLStringEvolutionModel *) this->model)->setLSystem(code);
}

const QString& gnomonLStringEvolutionModelCommand::lSystemCode(void) const
{
    return ((gnomonAbstractLStringEvolutionModel *) this->model)->lSystemCode();
}

int gnomonLStringEvolutionModelCommand::derivationLength(void) const
{
    return d->derivationLength;
}

void gnomonLStringEvolutionModelCommand::setDerivationLength(int l)
{
    d->derivationLength = l;
}

int gnomonLStringEvolutionModelCommand::animationStep(void) const
{
    return d->animation_step;
}

void gnomonLStringEvolutionModelCommand::setAnimationStep(int s)
{
    d->animation_step = s;
}

void gnomonLStringEvolutionModelCommand::setModelName(const QString& model_name)
{
    this->model_name = model_name;
    delete this->model;
    this->model = gnomonCore::lStringEvolutionModel::pluginFactory().create(this->model_name);
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonLStringEvolutionModelCommand::initialState()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > initial_state;
    initial_state["axiom"] = this->axiom();
    return initial_state;
}

gnomonAbstractCommand::orderedMap gnomonLStringEvolutionModelCommand::initialStateTypes() {
    gnomonAbstractCommand::orderedMap types;
    types.emplace_back(std::make_pair("axiom", "gnomonLString"));
    return types;
}

void gnomonLStringEvolutionModelCommand::setInitialState(const QString& name, std::shared_ptr<gnomonAbstractDynamicForm> form) {
    if (name == "axiom") {
        this->setAxiom(std::dynamic_pointer_cast<gnomonLStringSeries>(form));
    } else {
        dtkWarn()<<Q_FUNC_INFO<<"Unknown initial state "<< name;
    }
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonLStringEvolutionModelCommand::state()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > state;
    state["lString"] = this->lString();
    return state;
}

gnomonAbstractCommand::orderedMap gnomonLStringEvolutionModelCommand::stateTypes() {
    gnomonAbstractCommand::orderedMap types;
    types.emplace_back(std::make_pair("lString", "gnomonLString"));
    return types;
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
