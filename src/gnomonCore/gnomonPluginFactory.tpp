//
// Created by Arthur Luciani on 14/12/2022.
//
#pragma once
#include "gnomonPluginFactory.h"

template<typename T>
void gnomonPluginFactory<T>::record(const QString &key, gnomonPluginFactory::creator func, const QString &name,
                                    const QString &doc) {
    if(!this->names.contains(key)) {
        this->names.insert(key, name);
        this->docs.insert(key, doc);
    }
    dtkCorePluginFactory<T>::record(key, func);
}

template<typename T>
void gnomonPluginFactory<T>::recordPlugin(const QString &key, struct dtkCorePluginBase *plugin, const QString &name,
                                          const QString &doc, bool force) {
    if(!this->names.contains(key)) {
        this->names.insert(key, name);
        this->docs.insert(key, doc);
    } else if(force) {
        this->names.remove(key);
        this->docs.remove(key);
        this->names.insert(key, name);
        this->docs.insert(key, doc);
    }
    dtkCorePluginFactory<T>::recordPlugin(key, plugin, force);
}

template<typename T>
void gnomonPluginFactory<T>::record(const QString &key, gnomonPluginFactory::widget_creator func, const QString &name,
                                    const QString &doc) {
    if(!this->names.contains(key)) {
        this->names.insert(key, name);
        this->docs.insert(key, doc);
    }
    dtkCorePluginFactory<T>::record(key, func);
}

template<typename T>
QString gnomonPluginFactory<T>::name(const QString &key) const {
    return this->names.value(key, "");
}

template<typename T>
QString gnomonPluginFactory<T>::doc(const QString &key) const {
    return this->docs.value(key, "");
}

template<typename T>
void gnomonPluginFactory<T>::record(const QString &key, gnomonPluginFactory::creator func) {
    dtkCorePluginFactory<T>::record(key, func);
}

template<typename T>
void gnomonPluginFactory<T>::record(const QString &key, gnomonPluginFactory::widget_creator func) {
    dtkCorePluginFactory<T>::record(key, func);
}
