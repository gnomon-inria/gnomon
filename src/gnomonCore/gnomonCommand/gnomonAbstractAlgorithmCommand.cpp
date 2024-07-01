#include <functional>
#include <QtCore>
#include <QtConcurrent>

#include <dtkScript>

#include "gnomonCore.h"
#include "gnomonAbstractAlgorithmCommand.h"
#include "gnomonAlgorithm/gnomonAbstractAlgorithm.h"
#include "gnomonAlgorithmsLogs/gnomonLogCaptureServer"

gnomonAbstractAlgorithmCommand::gnomonAbstractAlgorithmCommand(void)
{

}

gnomonAbstractAlgorithmCommand::~gnomonAbstractAlgorithmCommand(void)
{
    delete action;
}

void  gnomonAbstractAlgorithmCommand::clear(void)
{
    if(this->action) {
        this->action->clearInputs();
        this->action->clearOutputs();
        /*
        auto name = algorithmName();
        delete this->action;
        this->action = nullptr;
        setAlgorithmName(name);
        */
    }
}

void gnomonAbstractAlgorithmCommand::redo(void)
{
    // TODO: adapters command will probably need to run in main thread due to difficulties in deserialization
    Q_ASSERT(this->action);

    this->predo(); //setting is_async here

    if(override_async) {
        this->action->is_async = false;  //TODO delete now not needed anymore on pipeline load!!!!!!!
    }

    this->action->setLogServerAddress(gnomonLogCaptureServer::instance()->completeAddress());

    if(this->action->is_async) {

        // cleaning watcher
        if(watcher){
            watcher->disconnect();
            if(watcher->isRunning()) {
                watcher->cancel();
                watcher->waitForFinished();
            }
        }
        delete watcher;
        // preparing watcher
        watcher = new QFutureWatcher<void>();
        connect(watcher, &QFutureWatcher<void>::finished, this, &gnomonAbstractAlgorithmCommand::finished);
        auto future = QtConcurrent::run(runner, this);
        watcher->setFuture(future);
    } else {
        this->action->run();
        this->postdo(); //  DEPRECATED: here, the output of the action is stored by the action. so we can now clear the action outputs.
        //this->action->clearOutputs(); // postdo copy a pointer but the data stays in the action. So no delete by clearOutputs
        emit finished();
    }
}

//QPromise<void> &promise,
extern void runner(gnomonAbstractAlgorithmCommand* command) {
    auto lambda_callback = [](gnomonAbstractCommand *command) {
        qDebug() << "in the lambda " << command;
        if(command) {
            command->postdo();
        } else {
            qWarning() << Q_FUNC_INFO << "command is null";
        }
    };

    command->action->callback = lambda_callback;
    //choice 1 : the callback is called from python
    //command->action->run_async(command);

    // choice 2: callback not called, we directly call postdo and finished
    command->action->run();
    command->postdo();

    if (gnomonCore::gui_thread) {
        for(const QString& k : command->outputs().keys()) {
            if(command->outputs()[k])
                command->outputs()[k]->metadata()->moveToThread(gnomonCore::gui_thread);
        }
    }
    //command->action->clearOutputs();
}
