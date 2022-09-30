#include <functional>
#include <QtCore>
#include <QtConcurrent>

#include <dtkScript>

#include "gnomonAbstractCommand.h"
#include "gnomonAlgorithm/gnomonAbstractAlgorithm.h"

gnomonAbstractCommand::gnomonAbstractCommand(void)
{

}

gnomonAbstractCommand::~gnomonAbstractCommand(void)
{
    delete action;
}

void gnomonAbstractCommand::redo(void)
{
    // TODO: adapters command will probably need to run in main thread due to difficulties in deserialization
    Q_ASSERT(this->action);

    this->predo(); //setting is_async here

    if(override_async) {
        this->action->is_async = false;  //TODO delete now not needed anymore on pipeline load!!!!!!!
    }
    
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
        connect(watcher, &QFutureWatcher<void>::finished, this, &gnomonAbstractCommand::finished);
        auto future = QtConcurrent::run(runner, this);
        watcher->setFuture(future);
    } else {
        this->action->run();
        this->postdo();
        emit finished();
    }
}

//QPromise<void> &promise,
extern void runner(gnomonAbstractCommand* command) {
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

    if(gnomonAbstractCommand::gui_thread ) {
        for(const QString& k : command->outputs().keys()) {
            if(command->outputs()[k])
                command->outputs()[k]->metadata()->moveToThread(gnomonAbstractCommand::gui_thread);
        }
    }
}

QThread *gnomonAbstractCommand::gui_thread = nullptr;