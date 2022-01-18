#include <dtkScript>

#include "gnomonAbstractCommand.h"

void gnomonAbstractCommand::redo(void)
{

    Q_ASSERT(this->action);
    // this->action->setAutoDelete(false);

    qInfo() << this->algorithm_name << "from" << this->factory_name;

    this->predo();

    // this->action->run();

    int a = QThreadPool::globalInstance()->activeThreadCount();

    // qDebug() << Q_FUNC_INFO << QThreadPool::globalInstance()->activeThreadCount();
    // QThreadPool::globalInstance()->start(this->action);
    // qDebug() << Q_FUNC_INFO << QThreadPool::globalInstance()->activeThreadCount();
    // QThreadPool::globalInstance()->waitForDone(2500);
    // qDebug() << Q_FUNC_INFO << QThreadPool::globalInstance()->activeThreadCount();

    qDebug() << Q_FUNC_INFO << "Before" << a;

    void *main_t = dtkScriptInterpreterPython::instance()->allowThreads();

    QtConcurrent::run([=] (void) -> void
    {
        qDebug() << "Coucou from thread;";
        // dtkScriptInterpreterPython::instance()->childAcquireLock(main_t);
        QThread::msleep(5000);
        this->action->run();
        QThread::msleep(5000);
        // dtkScriptInterpreterPython::instance()->childReleaseLock();
        // QThread::msleep(2000);
        qDebug() << "ReCoucou from thread;";
    });

    while(QThreadPool::globalInstance()->activeThreadCount() > a) {
        qApp->processEvents();
    }

    dtkScriptInterpreterPython::instance()->endAllowThreads();

    qDebug() << "Processing ..." << a << QThreadPool::globalInstance()->activeThreadCount();

    this->postdo();
}
