#pragma once

#include <QtCore>
#include <functional>

//#include <dtkCorePluginFactory>
#include <dtkCore/dtkCorePluginBase.h>

template <typename T> class gnomonPluginFactory
{

public:
    gnomonPluginFactory(const gnomonPluginFactory&) = delete;
    gnomonPluginFactory(gnomonPluginFactory&&) = delete;
    gnomonPluginFactory& operator=(const gnomonPluginFactory&) = delete;
    gnomonPluginFactory& operator=(gnomonPluginFactory&&) = delete;

public:
    typedef T *(*creator) ();

public:
    T *create(const QString& key) const;
    void clear(void);

public:
    virtual void record(const QString& key, creator func, const QString& name="", const QString& doc="");
    virtual void recordPlugin(const QString& key, dtkCorePluginBase *plugin, const QString& name="", const QString& doc="", bool force = false);
    QStringList keys(void) const;

public:
    virtual QString name(const QString& key) const;
    virtual QString doc(const QString& key) const;
    virtual QVariantList dataList() const;

protected:
    QHash<QString, QString> names;
    QHash<QString, QString> docs;
    QHash<QString, creator> creators;
    QHash<QString, dtkCorePluginBase *> creators_plugins;

protected:
    gnomonPluginFactory();
    virtual ~gnomonPluginFactory(void);

};

#include "gnomonPluginFactory.tpp"
