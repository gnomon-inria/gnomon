#pragma once

#include <gnomonCore>

#include "gnomonAlgorithm/gnomonAbstractAlgorithm.h"

class gnomonAbstractCommand : public QObject
{
    Q_OBJECT

public:
    using orderedMap = std::vector<std::pair <QString, QString>>; // to respect the order of inserting

public:
             gnomonAbstractCommand(void);
    virtual ~gnomonAbstractCommand(void);

public slots:
    virtual void  predo(void) = 0;
    virtual void postdo(void) = 0;
    virtual void   undo(void) = 0;
    virtual void   redo(void) final;
    virtual void futureFinished(){
        deserializeResults(watcher.result());
    }

public:
    virtual void deserializeResults(QJsonObject serialization) {};
    virtual QJsonObject serializeResults(void) { return {}; };

signals:
    void finished(void);
    void logged(const QString&);

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
    QFutureWatcher<QJsonObject> watcher;
};
