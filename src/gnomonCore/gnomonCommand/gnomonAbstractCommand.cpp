#include <QtCore>
#include <QtConcurrent>

#pragma push_macro("slots")
#undef slots
#include <Python.h>
#pragma pop_macro("slots")

#include <dtkScript>
#include <unistd.h>
#include <sys/wait.h>

#include "gnomonAbstractCommand.h"

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
    connect(watcher, &QFutureWatcher<void>::finished,
            this,&gnomonAbstractCommand::futureFinished);
    connect(watcher, &QFutureWatcher<void>::finished,
            this, &gnomonAbstractCommand::finished);


    auto future = QtConcurrent::run(runner, this);
    watcher->setFuture(future);
    future.waitForFinished();
}

extern void runner(gnomonAbstractCommand* command) {
    dtkScriptInterpreterPython::instance()->childAcquireLock(1);
    command->predo();
    command->action->run();
    command->postdo();
    dtkScriptInterpreterPython::instance()->childReleaseLock();
}
