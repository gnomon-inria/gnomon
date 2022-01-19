#include <QtCore>
#include <QtConcurrent>

#include <dtkScript>

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

    QFile file("/tmp/gnomon_command.log");
    file.open(QIODevice::ReadOnly);

    QString buffer;
    QString buffered;

    QTextStream stream(&file);

    while(!stream.atEnd())
        stream.seek(stream.pos()+1);

    qInfo() << "Buffered:" << stream.pos();

// /////////////////////////////////////////////////////////////////////////////

    this->predo();

    int s;
    int a = QThreadPool::globalInstance()->activeThreadCount();

    dtkScriptInterpreterPython::instance()->interpret("old_stdout = sys.stdout", &s);
    dtkScriptInterpreterPython::instance()->interpret("fsock = open('/tmp/gnomon_command.log','a', 0)", &s);
    dtkScriptInterpreterPython::instance()->interpret("sys.stdout = fsock", &s);

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

        if(!stream.atEnd()) {
            qInfo() << "There is something to read";

            // get the stream diff
            // emit logged(...);
        }

    }

    dtkScriptInterpreterPython::instance()->endAllowThreads();

    dtkScriptInterpreterPython::instance()->interpret("sys.stdout = old_stdout", &s);
    dtkScriptInterpreterPython::instance()->interpret("fsock.close()", &s);

    emit finished();

    this->postdo();
}
