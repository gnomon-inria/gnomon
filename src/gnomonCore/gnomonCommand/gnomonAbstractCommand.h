#pragma once

#include <gnomonCore>

#include "gnomonAlgorithm/gnomonAbstractAlgorithm.h"

#include <QtConcurrent>

class gnomonAbstractCommand
{
public:
    using orderedMap = std::vector<std::pair <QString, QString>>; // to respect the order of inserting

public:
             gnomonAbstractCommand(void) = default;
    virtual ~gnomonAbstractCommand(void);

public:
    virtual void  predo(void) = 0;
    virtual void postdo(void) = 0;
    virtual void   undo(void) = 0;
    virtual void   redo(void) final {

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
        // qWarning() << Q_FUNC_INFO << qApp->thread();

        qDebug() << Q_FUNC_INFO << "Before" << a;

        QtConcurrent::run([this] (void) -> void
        {
            qDebug() << "Coucou from thread;";
            this->action->run();
            qDebug() << "ReCoucou from thread;";
        });

        while(QThreadPool::globalInstance()->activeThreadCount() > a) {
            qDebug() << "Processing ..." << a << QThreadPool::globalInstance()->activeThreadCount();
            qApp->processEvents();
        }

        this->postdo();
    }

public:
    QString documentation(void)
    {
        return action->documentation();
    }

    const QString& algorithmName(void)
    {
        return this->algorithm_name;
    }

    virtual void setAlgorithmName(const QString &name) = 0;

    const QString& factoryName(void)
    {
        return this->factory_name;
    }

public:
    inline virtual dtkCoreParameters parameters() const {return this->action->parameters();};
    inline virtual void setParameter(const QString& parameter, const QVariant& value) {
        this->action->setParameter(parameter, value);
    }
    virtual QMap<QString, gnomonAbstractDynamicForm *> inputs() = 0;
    virtual orderedMap inputTypes() = 0;
    virtual void setInputForm(const QString& name, gnomonAbstractDynamicForm *form) = 0;
    [[deprecated]] virtual void addInputForm(gnomonAbstractDynamicForm *form) {}

    virtual QMap<QString, gnomonAbstractDynamicForm *> outputs() = 0;
    virtual orderedMap outputTypes() = 0;

protected:
    gnomonAbstractAlgorithm *action = nullptr;
    QString algorithm_name = "";
    QString factory_name = "";

};

// ///////////////////////////////////////////////////////////////////

inline gnomonAbstractCommand::~gnomonAbstractCommand()
{
    if (action) {
        delete action;
        action = nullptr;
    }
}
