#pragma once

#include <gnomonCoreExport>

#include <memory>
#include <QtCore>

#include <dtkCore/dtkCoreParameter>

#include <gnomonCore/gnomonAlgorithm/gnomonAbstractAlgorithm>
#include <gnomonCore/gnomonForm/gnomonAbstractDynamicForm>
#include "gnomonCore/gnomonPluginFactory.h"

class GNOMONCORE_EXPORT gnomonAbstractCommand : public QObject
{
    Q_OBJECT

public:
    using orderedMap = std::vector<std::pair <QString, QString>>; // to respect the order of inserting

public:
             gnomonAbstractCommand(void) = default;
    virtual ~gnomonAbstractCommand(void) = default;

public slots:
    virtual void  predo(void) = 0;
    virtual void postdo(void) = 0;
    virtual void   undo(void) = 0;
    virtual void   redo(void) = 0;
    virtual void  clear(void) = 0;

    virtual void pause(void) = 0;
    virtual void resume(void) = 0;
    virtual void stop(void) = 0;
    virtual int progress(void) = 0;

public:
    virtual void deserializeResults(QJsonObject &serialization) = 0;
    virtual QJsonObject serializeResults(void) = 0;

signals:
    void finished(void);
    void logged(const QString&);

public:
    virtual QString documentation(void) = 0;
    virtual QString version(void) = 0;

    const QString& factoryName(void)
    {
        return this->factory_name;
    }

    inline gnomonPluginFactoryBase *pluginFactory(void) {
        return this->factory;
    }

    void setNoAsync() {this->override_async = true;}

public:
    virtual dtkCoreParameters parameters() const = 0;
    virtual void setParameter(const QString& parameter, const QVariant& value) = 0;
    virtual QMap<QString, QString> parameterGroups() const = 0;

    virtual QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs() = 0;
    virtual orderedMap inputTypes() = 0;
    virtual void setInputForm(const QString& name, std::shared_ptr<gnomonAbstractDynamicForm> form) = 0;
    [[deprecated]] virtual void addInputForm(gnomonAbstractDynamicForm *form) {}

    virtual QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs() = 0;
    virtual orderedMap outputTypes() = 0;

protected:
    QString factory_name = "";
    gnomonPluginFactoryBase *factory = nullptr;
    QFutureWatcher<void> *watcher = nullptr;
    bool override_async = false;
};
