#pragma once

#include <gnomonCoreExport>

#include <memory>
#include <QtCore>

#include <dtkCore/dtkCoreParameter>

#include <gnomonCore/gnomonAlgorithm/gnomonAbstractAlgorithm>
#include <gnomonCore/gnomonForm/gnomonAbstractDynamicForm>
#include "gnomonCore/gnomonPluginFactory.h"

#include "gnomonAbstractCommand.h"


class GNOMONCORE_EXPORT gnomonAbstractAlgorithmCommand : public gnomonAbstractCommand
{
    Q_OBJECT
    friend void runner(gnomonAbstractAlgorithmCommand* command);

public:
             gnomonAbstractAlgorithmCommand(void);
    virtual ~gnomonAbstractAlgorithmCommand(void);

public slots:
    virtual void redo(void) final override;
    virtual void clear(void) override;

public:
    inline virtual QString documentation(void) override { return action->documentation(); }
    inline virtual QString version(void) override { return action->version(); }

    const QString& algorithmName(void)
    {
        return this->algorithm_name;
    }

    virtual void setAlgorithmName(const QString &name) = 0;

    inline virtual void pause(void) override {
        if(this->action)
            this->action->pause();
    };

    inline virtual void resume(void) override {
        if(this->action)
            this->action->resume();
    };

    inline virtual void stop(void) override {
        if(this->action)
            this->action->stop();
    };

    inline virtual int progress(void) override {
        if(this->action) {
            return this->action->progress();
        } else {
            return -1;
        }
    };

public:
    inline virtual dtkCoreParameters parameters() const override {return this->action->parameters();};
    inline virtual void setParameter(const QString& parameter, const QVariant& value) override {
        this->action->setParameter(parameter, value);
    }
    inline virtual QMap<QString, QString> parameterGroups() const override {return this->action->parameterGroups();};

protected:
    class gnomonAbstractAlgorithm *action = nullptr;
    QString algorithm_name = "";
};

void runner(gnomonAbstractAlgorithmCommand* command);
