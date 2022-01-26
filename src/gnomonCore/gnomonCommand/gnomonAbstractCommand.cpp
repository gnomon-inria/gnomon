#include <QtCore>
#include <QtConcurrent>

#include <dtkScript>
#include <unistd.h>

#include "gnomonAbstractCommand.h"

gnomonAbstractCommand::gnomonAbstractCommand(void)
{

}

gnomonAbstractCommand::~gnomonAbstractCommand(void)
{
    if (action)
        delete action;
}

void gnomonAbstractCommand::redo(void)
{
    Q_ASSERT(this->action);

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////



    connect(&watcher, &QFutureWatcher<QJsonObject>::finished,
            this,&gnomonAbstractCommand::futureFinished);
    connect(&watcher, &QFutureWatcher<QJsonObject>::finished,
            this, &gnomonAbstractCommand::finished);

    auto future = QtConcurrent::run([=] (void) -> QJsonObject
    {
        int stdoutPipe[2];  // [read, write]
        pipe(stdoutPipe);
        int outputPipe[2];  // [read, write]
        pipe(outputPipe);
        int pid = fork();
        if(pid<0) {
            dtkError() << Q_FUNC_INFO << "Fork failed";
        } else if(pid==0) {
            // child
            close(stdoutPipe[0]);  // close reading end
            close(outputPipe[0]);  // close reading end
            dup2(stdoutPipe[1], STDOUT_FILENO); // changing output

            this->action->run();
            this->postdo();
            auto outputs = this->serializeResults();
            QFile outputFile;
            outputFile.open(outputPipe[1], QIODevice::WriteOnly);
            QDataStream outputStream(&outputFile);
            outputStream << outputs;
            outputFile.close();
            close(stdoutPipe[1]);  // close writing end
            close(outputPipe[1]);  // close writing end
            exit(0);
        } else {
            // parent
            close(stdoutPipe[1]);  // close writing end
            close(outputPipe[1]);  // close writing end

            QFile outputFile;
            outputFile.open(outputPipe[0], QIODevice::ReadOnly);
            QDataStream outputStream(&outputFile);
            QJsonObject outputs ;
            outputStream >> outputs;
            outputFile.close();
            // closing thread
            int status;
            waitpid(pid, &status, WEXITED);
            close(stdoutPipe[0]);  // close reading end
            close(outputPipe[0]);  // close reading end
            return outputs;
        }

        return {};
        // dtkScriptInterpreterPython::instance()->childAcquireLock(main_t);
        // QThread::msleep(1000);

        // QThread::msleep(1000);
        // dtkScriptInterpreterPython::instance()->childReleaseLock();
    });
    watcher.setFuture(future);
    while(future.isRunning()) {

        qApp->processEvents();

    }

}
