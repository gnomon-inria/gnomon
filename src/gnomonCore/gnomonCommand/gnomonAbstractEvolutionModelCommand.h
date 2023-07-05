#pragma once

#include <gnomonCore/gnomonCommand/gnomonAbstractCommand.h>

#include <gnomonCore/gnomonModel/gnomonAbstractEvolutionModel>
#include "gnomonAlgorithmsLogs/gnomonLogCaptureServer"

class GNOMONCORE_EXPORT gnomonAbstractEvolutionModelCommand : public QObject
{
Q_OBJECT

public:
    gnomonAbstractEvolutionModelCommand(void) = default;
    ~gnomonAbstractEvolutionModelCommand(void) = default;

public slots:
    virtual void  predo(void) {
        this->model->setLogServerAddress(gnomonLogCaptureServer::instance()->completeAddress());
};
    virtual void postdo(void) = 0;
    virtual void   undo(void) = 0;
    virtual void   redo(void) = 0;

signals:
    void modelMessage(QString);

public:
    inline virtual const QString& modelName(void) { return this->model_name; };
    inline virtual void setModelName(const QString& name) = 0;

    inline QString version(void) { return "0.1.0"; };

    inline virtual const QString& factoryName(void) { return this->factory_name; }

public:
    virtual QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > initialState() = 0;
    virtual gnomonAbstractCommand::orderedMap initialStateTypes() = 0;
    virtual void setInitialState(const QString& name, std::shared_ptr<gnomonAbstractDynamicForm> form) = 0;

    virtual QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > state() = 0;
    virtual gnomonAbstractCommand::orderedMap stateTypes() = 0;

public:
    inline virtual dtkCoreParameters parameters() const { return this->model->parameters(); };
    inline virtual void setParameter(const QString& parameter, const QVariant& value) { this->model->setParameter(parameter, value); }
    inline virtual QMap<QString, QString> parameterGroups() const { return this->model->parameterGroups(); };

protected:
    class gnomonAbstractModel *model = nullptr;
    QString model_name = "";
    QString factory_name = "";

};
