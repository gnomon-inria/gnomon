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

void gnomonLStringEvolutionModelCommand::redo(QMutex* mutex, QWaitCondition* synchro)
{
    Q_ASSERT(this->model);

    d->watcher = std::make_unique<QFutureWatcher<int>>();
    connect(d->watcher.get(), &QFutureWatcher<void>::finished, this, &gnomonLStringEvolutionModelCommand::finished);
    if(this->simulationType == SimulationType::animate)
        connect(d->watcher.get(), &QFutureWatcher<void>::progressValueChanged, this, &gnomonLStringEvolutionModelCommand::stepFinished);

    auto future = QtConcurrent::run([=](QPromise<int> &promise){
        promise.start();
        // TODO: retrieve max derivation length from lsystem
        int maxDerivationLength = this->simulationType == SimulationType::animate ? 1400 : 1;
        promise.setProgressRange(0, maxDerivationLength);
        for(int i=0; i<maxDerivationLength; i++) {
            qDebug() <<"Step:"<< i;
            this->predo();

            std::shared_ptr<gnomonLStringSeries> lString = ((gnomonAbstractLStringEvolutionModel *) this->model)->state();
            if (lString) {
                int t = int(lString->times().last());
                if(this->simulationType == SimulationType::run) {
                    this->model->run(0, 0, 0);
                } else {
                    this->model->step(t, 1);
                }
            }
            this->postdo();
            promise.setProgressValue(i);
            promise.suspendIfRequested();
            if(synchro && mutex) {
                QMutexLocker locker(mutex);
                synchro->wait(locker.mutex());
            }
            QThread::msleep(300);
        }
        promise.finish();
    });
    d->watcher->setFuture(future);
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
