#pragma once

#include <gnomonCore/gnomonCommand/gnomonAbstractCommand.h>

#include <gnomonCore/gnomonModel/gnomonAbstractEvolutionModel>
#include "gnomonAlgorithmsLogs/gnomonLogCaptureServer"

class GNOMONCORE_EXPORT gnomonAbstractEvolutionModelCommand : public gnomonAbstractCommand
{
Q_OBJECT

public:
    gnomonAbstractEvolutionModelCommand(void) = default;
    ~gnomonAbstractEvolutionModelCommand(void) = default;

public slots:
    inline virtual void predo(void) override {
        this->model->setLogServerAddress(gnomonLogCaptureServer::instance()->completeAddress());
    };
    inline void clear(void) override { }

    inline void pause(void) override { }
    inline void resume(void) override { }
    inline void stop(void) override { }
    inline int progress(void) override { return 0; }
    inline QString progressMessage(void) override { return ""; }

public:
    inline virtual QString documentation(void) override { return model->documentation(); }
    inline virtual QString version(void) override { return model->version(); };

signals:
    void modelMessage(QString);

public:
    inline virtual const QString& modelName(void) { return this->model_name; };
    inline virtual void setModelName(const QString& name) = 0;

    inline virtual const QString& factoryName(void) { return this->factory_name; }

public:
    virtual QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > initialState() = 0;
    virtual gnomonAbstractCommand::orderedMap initialStateTypes() = 0;
    virtual void setInitialState(const QString& name, std::shared_ptr<gnomonAbstractDynamicForm> form) = 0;

    virtual QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > state() = 0;
    virtual gnomonAbstractCommand::orderedMap stateTypes() = 0;

public:
    inline QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs() override {
        return this->initialState();
    }

    inline orderedMap inputTypes() override {
        return this->initialStateTypes();
    }

    inline void setInputForm(const QString& name, std::shared_ptr<gnomonAbstractDynamicForm> form) override {
        this->setInitialState(name, form);
    }

    inline QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs() override {
        return this->state();
    }

    inline orderedMap outputTypes() override {
        return this->stateTypes();
    }

    inline void deserializeResults(QJsonObject &serialization) override {

    }

    inline QJsonObject serializeResults(void) override {
        return QJsonObject();
    }

public:
    inline virtual dtkCoreParameters parameters() const override { return this->model->parameters(); };
    inline virtual void setParameter(const QString& parameter, const QVariant& value) override { this->model->setParameter(parameter, value); }
    inline virtual QMap<QString, QString> parameterGroups() const override { return this->model->parameterGroups(); };
    inline virtual void addParameter(const QString& parameterName, const QString& parameterType, const QString& group) {
        return this->model->addParameter(parameterName, parameterType, group);
    };
    inline virtual void duplicateParameter(const QString& parameterName, const QString& newName, const QString& group) {
        return this->model->duplicateParameter(parameterName, newName, group);
    };
    inline virtual void removeParameter(const QString& parameterName) {
        return this->model->removeParameter(parameterName);
    };
    inline virtual void setGroup(const QString& parameterName, const QString& group) {
        return this->model->setGroup(parameterName, group);
    };
    inline virtual void duplicateGroup(const QString& groupName, const QString& newName) {
        return this->model->duplicateGroup(groupName, newName);
    };
    inline virtual void removeGroup(const QString& groupName) {
        return this->model->removeGroup(groupName);
    };
    inline virtual void renameParameter(const QString& oldName, const QString& newName) {
        return this->model->renameParameter(oldName, newName);
    };
    inline virtual void renameGroup(const QString& oldName, const QString& newName) {
        return this->model->renameGroup(oldName, newName);
    };

protected:
    class gnomonAbstractModel *model = nullptr;
    QString model_name = "";
    QString factory_name = "";

};
