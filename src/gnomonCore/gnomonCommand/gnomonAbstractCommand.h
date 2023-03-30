#pragma once

#include <gnomonCoreExport>

#include <memory>
#include <QtCore>

#include <dtkCore/dtkCoreParameter>

#include <gnomonCore/gnomonAlgorithm/gnomonAbstractAlgorithm>
#include <gnomonCore/gnomonForm/gnomonAbstractDynamicForm>

class GNOMONCORE_EXPORT gnomonAbstractCommand : public QObject
{
    Q_OBJECT
    friend void runner(gnomonAbstractCommand* command);

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

public:
    virtual void deserializeResults(QJsonObject &serialization) = 0;
    virtual QJsonObject serializeResults(void) = 0;

signals:
    void finished(void);
    void logged(const QString&);

public:
    inline QString documentation(void) { return action->documentation(); }
    inline QString version(void) { return action->version(); }

    const QString& algorithmName(void)
    {
        return this->algorithm_name;
    }

    virtual void setAlgorithmName(const QString &name) = 0;

    const QString& factoryName(void)
    {
        return this->factory_name;
    }

    inline void pause(void) {
        if(this->action)
            this->action->pause();
    };

    inline void resume(void) {
        if(this->action)
            this->action->resume();
    };

    inline void stop(void) {
        if(this->action)
            this->action->stop();
    };

    inline int progress(void) {
        if(this->action) {
            return this->action->progress();
        } else {
            return -1;
        }
    }

    void setNoAsync() {this->override_async = true;}

public:
    inline virtual dtkCoreParameters parameters() const {return this->action->parameters();};
    inline virtual void setParameter(const QString& parameter, const QVariant& value) {
        this->action->setParameter(parameter, value);
    }
    inline virtual QMap<QString, QString> parameterGroups() const {return this->action->parameterGroups();};
    virtual QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs() = 0;
    virtual orderedMap inputTypes() = 0;
    virtual void setInputForm(const QString& name, std::shared_ptr<gnomonAbstractDynamicForm> form) = 0;
    [[deprecated]] virtual void addInputForm(gnomonAbstractDynamicForm *form) {}

    virtual QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs() = 0;
    virtual orderedMap outputTypes() = 0;

public:
    static QThread *gui_thread;

protected:
    class gnomonAbstractAlgorithm *action = nullptr;
    QString algorithm_name = "";
    QString factory_name = "";
    QFutureWatcher<void> *watcher = nullptr;
    bool override_async = false;
};

void runner(gnomonAbstractCommand* command);

