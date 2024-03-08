#pragma once

#include <gnomonCoreExport>

#include <memory>
#include <QtCore>

#include <dtkCore/dtkCoreParameter>

#include <gnomonCore/gnomonAlgorithm/gnomonAbstractAlgorithm>
#include <gnomonCore/gnomonForm/gnomonAbstractDynamicForm>
#include "gnomonCore/gnomonPluginFactory.h"

#define GNOMON_REGISTER_TYPE(ClassName) \
    namespace ClassName##Registering { \
        static bool ClassName##Register() { \
            int id = qRegisterMetaType<ClassName>(); \
            return true; \
        } \
    const bool is_registered = ClassName##Register(); \
}

struct gnomonAbstractCommandTraits {
    bool isEmpty() {
        return availablePlugins().empty();
    };
    virtual QStringList availablePlugins() = 0;
};

template <typename T>
struct GNOMONCORE_EXPORT gnomonAbstractCommandTraitsBase : public gnomonAbstractCommandTraits
{    
    QStringList availablePlugins() override {
        return availablePluginsFromGroup(T::groupName);
    }
};

#define GNOMON_COMMAND_TRAITS(BaseClass) \
    struct BaseClass##Traits : public gnomonAbstractCommandTraitsBase<BaseClass> { \
}; 

#define GNOMON_COMMANDS_INIT(BaseClass) \
    this->factory_name = groupName; \
    this->m_traits = new BaseClass##Traits(); \
    loadPluginGroup(this->factoryName()); 


class GNOMONCORE_EXPORT gnomonAbstractCommand : public QObject
{
    Q_OBJECT

public:
    using orderedMap = std::vector<std::pair <QString, QString>>; // to respect the order of inserting

public:
             gnomonAbstractCommand(void) = default;
    virtual ~gnomonAbstractCommand(void) {
        if(m_traits) 
            delete m_traits;
        m_traits = nullptr;
        };

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
    virtual QString progressMessage(void) = 0;

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
    inline bool isEmpty() {
        if(!m_traits) {
            qWarning() << Q_FUNC_INFO << "gnomonAbstractAlgorithmCommandTraits is not set";
            return true;
        }
        return m_traits->isEmpty();
    }
    
    inline QStringList availablePlugins() {
        if(!m_traits) {
            qWarning() << Q_FUNC_INFO << "gnomonAbstractAlgorithmCommandTraits is not set";
            return QStringList();
        }

        return m_traits->availablePlugins();
    }


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
    gnomonAbstractCommandTraits *m_traits = nullptr;

    QFutureWatcher<void> *watcher = nullptr;
    bool override_async = false;
};