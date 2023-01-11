//
// Created by Arthur Luciani on 09/01/2023.
//

#include "gnomonPluginData.h"

gnomonPluginData::gnomonPluginData(QObject *parent) : QObject(parent) {

}

const QString &gnomonPluginData::getKey() const {
    return key;
}

const QString &gnomonPluginData::getName() const {
    return name;
}

const QString &gnomonPluginData::getDoc() const {
    return doc;
}

const QString &gnomonPluginData::getPackage() const {
    return package;
}

gnomonPluginData::gnomonPluginData(const QString &key, const QString &name, const QString &doc,
                                   const QString &package) : QObject(nullptr), key(key), name(name), doc(doc),
                                                              package(package) {}

gnomonPluginData::gnomonPluginData(const gnomonPluginData &o): QObject(nullptr), key(o.key), name(o.name), doc(o.doc),
                                                              package(o.package) {

}

bool gnomonPluginData::operator==(const gnomonPluginData &rhs) const {
    return key == rhs.key &&
           name == rhs.name &&
           doc == rhs.doc &&
           package == rhs.package;
}

bool gnomonPluginData::operator!=(const gnomonPluginData &rhs) const {
    return !(rhs == *this);
}

gnomonPluginData &gnomonPluginData::operator=(const gnomonPluginData &other) {
    if(this!=&other) {
        key = other.key;
        name = other.name;
        doc = other.doc;
        package = other.package;
    }
    return *this;
}
