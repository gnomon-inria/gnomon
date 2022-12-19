#pragma once
#include <dtkCorePluginFactory>

template <typename T> class gnomonPluginFactory: public dtkCorePluginFactory<T> {

public:
    typedef T *(*creator) ();
    typedef std::function<QWidget *()> widget_creator;

public:
    virtual void record(const QString& key, creator func);
    virtual void record(const QString& key, creator func, const QString& name, const QString& doc);
    virtual void recordPlugin(const QString& key, class dtkCorePluginBase *plugin, const QString& name, const QString& doc, bool force = false);
    virtual void record(const QString& key, widget_creator func);
    virtual void record(const QString& key, widget_creator func, const QString& name, const QString& doc);

public:
    virtual QString name(const QString& key) const;
    virtual QString doc(const QString& key) const;

protected:
    QHash<QString, QString> names;
    QHash<QString, QString> docs;
};

#include "gnomonPluginFactory.tpp"
