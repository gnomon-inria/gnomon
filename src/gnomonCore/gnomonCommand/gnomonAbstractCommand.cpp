#include <QtCore>
#include <QtConcurrent>

#pragma push_macro("slots")
#undef slots
#include <Python.h>
#pragma pop_macro("slots")

#include <dtkScript>
#include <unistd.h>

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
    Q_ASSERT(this->action);

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////



    connect(&watcher, &QFutureWatcher<QJsonObject>::finished,
            this,&gnomonAbstractCommand::futureFinished);
    connect(&watcher, &QFutureWatcher<QJsonObject>::finished,
            this, &gnomonAbstractCommand::finished);

    // prepare forking
    int stdoutPipe[2];  // [read, write]
    pipe(stdoutPipe);
    int outputPipe[2];  // [read, write]
    pipe(outputPipe);

    PyOS_BeforeFork();
    int pid = fork();
    if(pid<0) {
        PyOS_AfterFork_Parent();
        dtkError() << Q_FUNC_INFO << "Fork failed";

        // cleaning up
        close(stdoutPipe[0]);  // close reading end
        close(outputPipe[0]);  // close reading end
        close(stdoutPipe[1]);  // close writing end
        close(outputPipe[1]);  // close writing end
        // running
        this->action->run();
        this->postdo();

    } else if(pid==0) {
        // child
        PyOS_AfterFork_Child();
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
        PyOS_AfterFork_Parent();
        close(stdoutPipe[1]);  // close writing end
        close(outputPipe[1]);  // close writing end
        auto future = QtConcurrent::run([=] (void) -> QJsonObject
                                        {
                                            // monitoring child process and capturing output
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
                                        });
        watcher.setFuture(future);
        while(future.isRunning()) {
            qApp->processEvents();
        }
    }
}
