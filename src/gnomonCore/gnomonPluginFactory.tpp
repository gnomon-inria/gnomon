//
// Created by Arthur Luciani on 14/12/2022.
//
#pragma once
#include "gnomonPluginFactory.h"

template <typename T> inline gnomonPluginFactory<T>::gnomonPluginFactory(void)
{
}

template <typename T> inline gnomonPluginFactory<T>::~gnomonPluginFactory(void)
{
    this->clear();
}


template<typename T>
void gnomonPluginFactory<T>::record(const QString &key, gnomonPluginFactory::creator func, const QString &name, const QString &doc) {
    if(!this->names.contains(key)) {
        this->names.insert(key, name);
        this->docs.insert(key, doc);
    }

    if (this->creators.contains(key)) {
        qInfo() << Q_FUNC_INFO << "Factory already contains key" << key << ". Nothing is done";
        return;
    }
    this->creators.insert(key, func);

}

template<typename T>
void gnomonPluginFactory<T>::recordPlugin(const QString &key, struct dtkCorePluginBase *plugin, const QString &name, const QString &doc, bool force) {
    if(!this->names.contains(key)) {
        this->names.insert(key, name);
        this->docs.insert(key, doc);
    } else if(force) {
        this->names.remove(key);
        this->docs.remove(key);
        this->names.insert(key, name);
        this->docs.insert(key, doc);
    }

    if (this->creators_plugins.contains(key)) {
        if (!force) {
            qInfo() << Q_FUNC_INFO << "Factory already contains key" << key << ". Nothing is done";
            return;
        } else {
            qInfo() << Q_FUNC_INFO << "Factory already contains key" << key << ". replace old plugin";
            delete this->creators_plugins[key];
        }
    }

    this->creators_plugins.insert(key, plugin);
}

template <typename T> inline QStringList gnomonPluginFactory<T>::keys(void) const
{
    QStringList lkeys = this->creators.keys() ;
    lkeys << this->creators_plugins.keys();
    lkeys.sort(Qt::CaseInsensitive);
    return lkeys;
}

template<typename T>
QString gnomonPluginFactory<T>::name(const QString &key) const {
    return this->names.value(key, "");
}

template<typename T>
QString gnomonPluginFactory<T>::doc(const QString &key) const {
    return this->docs.value(key, "");
}

template <typename T> inline T *gnomonPluginFactory<T>::create(const QString& key) const
{
    T *obj = nullptr;
    if (this->creators.contains(key)) {
        obj = this->creators.value(key)();

    } else if (this->creators_plugins.contains(key)) {
        void *o = this->creators_plugins.value(key)->create();
        if (!o) {
            qWarning() << Q_FUNC_INFO << "Nullptr is returned by plugin creator.";
            return nullptr;
        }
        obj = static_cast<T*>(o);
    }

    return obj;
}

template <typename T> inline void gnomonPluginFactory<T>::clear(void)
{
    this->creators.clear();
    for (QString key: this->creators_plugins.keys()) {
        delete this->creators_plugins[key];
    }
    this->creators_plugins.clear();
}

template <typename T> inline QVariantList gnomonPluginFactory<T>::dataList(void) const
{

    QVariantList res;
    int i=0;
    for (const QString &key : this->keys()) {
        QVariantMap var;
        var["key"] = key;
        var["name"] = this->name(key);
        var["doc"] = this->doc(key);
        var["counter"] = i++;
        res.append(var);
    }

    return res;
}
