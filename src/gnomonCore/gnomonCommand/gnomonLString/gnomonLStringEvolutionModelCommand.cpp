#include "gnomonLStringEvolutionModelCommand.h"
#include "gnomonCommand/gnomonAbstractEvolutionModelCommand.h"
#include "gnomonModel/gnomonAbstractModel.h"

#include <dtkLog.h>
#include <gnomonCore>
#include <gnomonCore/gnomonModel/gnomonAbstractLStringEvolutionModel.h>
#include <gnomonCore/gnomonPythonPluginLoader.h>

#include <QtConcurrent>
#include <QtCore>
#include <qthread.h>

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonLStringEvolutionModelCommandPrivate
{
public:
    std::shared_ptr<gnomonLStringSeries> init_lString = nullptr;
    std::shared_ptr<gnomonLStringSeries> lString = nullptr;

    QStringList opened_files;
    int derivationLength = 0;
    int animation_step = 1;
    double animation_time = 10;

    std::unique_ptr<QFutureWatcher<int>> watcher = nullptr;
};


// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

gnomonLStringEvolutionModelCommand::gnomonLStringEvolutionModelCommand() : d(new gnomonLStringEvolutionModelCommandPrivate)
{
    GNOMON_COMMANDS_INIT(gnomonLStringEvolutionModelCommand)

    QStringList keys = gnomonCore::lStringEvolutionModel::pluginFactory().keys();
    if (!keys.empty()) {
        this->model_name = keys[0];
        this->model = gnomonCore::lStringEvolutionModel::pluginFactory().create(this->model_name);
        connect(this->model, &gnomonAbstractModel::modelMessage, this, &gnomonAbstractEvolutionModelCommand::modelMessage);
    } else {
        dtkWarn() << "lStringEvolutionModel factory is empty";
        dtkWarn() << "No model created, I may crash!!";
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
    gnomonAbstractEvolutionModelCommand::predo();
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

        if(this->simulationType == SimulationType::animate)
            d->lString->setAutoSave(false);

        //let's slow down the computation!
        // time during each iteration
        int sleeptime = int(d->animation_time*1000*d->animation_step / maxDerivationLength);
        QElapsedTimer timer;
        timer.start();

        for(; i<maxDerivationLength; i++) {
            this->predo();
            if(this->simulationType == SimulationType::run) {
                this->model->run(0, d->derivationLength, 0);
                this->postdo();
                if (gnomonCore::gui_thread) {
                    d->lString->metadata()->moveToThread(gnomonCore::gui_thread);
                }
            } else {
                if ((i+1) % d->animation_step == 0) {
                    d->lString->insert(i+1, lstring_model->stepAndReturn(i, 1));
                    if (gnomonCore::gui_thread) {
                        d->lString->metadata()->moveToThread(gnomonCore::gui_thread);
                    }
                    promise.setProgressValue(i+1);
                    if(this->simulationType == SimulationType::animate) {
                        int remainingTime = sleeptime - timer.restart();
                        if(remainingTime > 50) {
                            QThread::msleep(remainingTime);
                        }
                    }
                } else {
                    lstring_model->step(i,1);
                    if (gnomonCore::gui_thread) {
                        auto lString = ((gnomonAbstractLStringEvolutionModel *) this->model)->state();
                        if (lString) {
                            lString->metadata()->moveToThread(gnomonCore::gui_thread);
                        }
                    }
                }
            }

            promise.suspendIfRequested();
            if (promise.isCanceled())
                return;
        }

        if(this->simulationType == SimulationType::animate)
            d->lString->setAutoSave(true);

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

void gnomonLStringEvolutionModelCommand::setLSystem(const QString& path)
{
    // code is an opened file
    if(!d->opened_files.contains(path))
        d->opened_files.append(path);
    ((gnomonAbstractLStringEvolutionModel *) this->model)->setLSystem(path);
}

const QString& gnomonLStringEvolutionModelCommand::lSystemCode(void) const
{
    return ((gnomonAbstractLStringEvolutionModel *) this->model)->lSystemCode();
}

int gnomonLStringEvolutionModelCommand::derivationLength(void) const
{
    int dlength = dynamic_cast<gnomonAbstractLStringEvolutionModel*>(this->model)->derivationLength();
    d->derivationLength = dlength;
    return dlength;
}

void gnomonLStringEvolutionModelCommand::setDerivationLength(int l)
{
    d->derivationLength = l;
    dynamic_cast<gnomonAbstractLStringEvolutionModel*>(this->model)->setDerivationLength(l);
}

int gnomonLStringEvolutionModelCommand::animationStep(void) const
{
    return d->animation_step;
}

void gnomonLStringEvolutionModelCommand::setAnimationStep(int s)
{
    d->animation_step = s;
}


void gnomonLStringEvolutionModelCommand::setAnimationTime(double t)
{
    d->animation_time = t;
}

void gnomonLStringEvolutionModelCommand::setModelName(const QString& model_name)
{
    this->model_name = model_name;
    delete this->model;
    this->model = gnomonCore::lStringEvolutionModel::pluginFactory().create(this->model_name);
    connect(this->model, &gnomonAbstractModel::modelMessage, this, &gnomonAbstractEvolutionModelCommand::modelMessage);
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonLStringEvolutionModelCommand::initialState()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > initial_state;
    initial_state["initialState"] = this->axiom();
    return initial_state;
}

gnomonAbstractCommand::orderedMap gnomonLStringEvolutionModelCommand::initialStateTypes() {
    gnomonAbstractCommand::orderedMap types;
    types.emplace_back(std::make_pair("initialState", "gnomonLString"));
    return types;
}

void gnomonLStringEvolutionModelCommand::setInitialState(const QString& name, std::shared_ptr<gnomonAbstractDynamicForm> form) {
    if (name == "initialState" || name == "axiom") { // DEPRECATED: "axiom" is the old name
        this->setAxiom(std::dynamic_pointer_cast<gnomonLStringSeries>(form));
    } else {
        dtkWarn()<<Q_FUNC_INFO<<"Unknown initial state "<< name;
    }
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > gnomonLStringEvolutionModelCommand::state()
{
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > state;
    state["state"] = this->lString();
    return state;
}

gnomonAbstractCommand::orderedMap gnomonLStringEvolutionModelCommand::stateTypes() {
    gnomonAbstractCommand::orderedMap types;
    types.emplace_back(std::make_pair("state", "gnomonLString"));
    return types;
}

void gnomonLStringEvolutionModelCommand::clear(void) {
    gnomonAbstractEvolutionModelCommand::clear();
    // TODO: monitor here, clearing may cause problem later in this workspace
    d->init_lString = nullptr;
    d->lString = nullptr;
}

//
// gnomonLStringEvolutionModelCommand.cpp ends here
