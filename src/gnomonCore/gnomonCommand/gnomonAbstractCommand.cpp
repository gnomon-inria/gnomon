#include <dtkScript>

#include "gnomonAbstractCommand.h"

void gnomonAbstractCommand::redo(void)
{
    Q_ASSERT(this->action);

    this->predo();

    int a = QThreadPool::globalInstance()->activeThreadCount();

    void *main_t = dtkScriptInterpreterPython::instance()->allowThreads();

    QtConcurrent::run([=] (void) -> void
    {
        // dtkScriptInterpreterPython::instance()->childAcquireLock(main_t);
        // QThread::msleep(1000);
        this->action->run();
        // QThread::msleep(1000);
        // dtkScriptInterpreterPython::instance()->childReleaseLock();
    });

    while(QThreadPool::globalInstance()->activeThreadCount() > a) {
        qApp->processEvents();
    }

    dtkScriptInterpreterPython::instance()->endAllowThreads();

    emit finished();

    this->postdo();
}
