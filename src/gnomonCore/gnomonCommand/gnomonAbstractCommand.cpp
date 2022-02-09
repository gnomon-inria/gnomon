#include <QtCore>
#include <QtConcurrent>

#include <dtkScript>

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
    qDebug() << Q_FUNC_INFO << "start runner with command" << command;

    command->predo();
    qDebug() << Q_FUNC_INFO << "start run";

    command->action->run();
    qDebug() << Q_FUNC_INFO << "end run";

    command->postdo();
    qDebug() << Q_FUNC_INFO << "end postdo";

}
