#pragma once

#include <dtkCorePluginManagerHandler.h>

#include <dtkLog>

// ///////////////////////////////////////////////////////////////////
// gnomonPluginManagerPrivate
// ///////////////////////////////////////////////////////////////////

template <typename T> class gnomonPluginManagerPrivate
{
public:
    bool check(const QString& path, bool checkConceptLayer = true);
    bool checkConcept(const QString& path);
    bool checkVersion(const QString ref_version, const QString elem_version);

public:
    bool autoLoading = true;
    QString layerVersion = QString();

public:
    QHash<QString, QPluginLoader *> loaders;
    QHash<QString, QJsonObject > metadatas;
};

// ///////////////////////////////////////////////////////////////////

template <typename T> inline bool gnomonPluginManagerPrivate<T>::checkConcept(const QString& path)
{
    QString conceptName = QMetaType(qMetaTypeId<T *>()).name();
    conceptName.remove("Plugin*");

    QString pluginConcept = dtk::pluginManagerHandler().dtkConcept(path);

    if (conceptName != pluginConcept) {
        dtkDebug() << Q_FUNC_INFO << "skip plugin: not an implementation of the current concept" << conceptName << ", this plugin is for " << pluginConcept;
        return false;
    }

    return true;
}

template <typename T> inline bool gnomonPluginManagerPrivate<T>::check(const QString& path, bool checkConceptLayer)
{
    bool status = true;

    if (checkConceptLayer && !this->checkConcept(path)) {
        return false;
    }

    if (checkConceptLayer && !layerVersion.isNull() && !checkVersion(layerVersion , dtkCorePluginManagerHandler::instance()->pluginsLayerVersion(path))) {
        dtkWarn() << Q_FUNC_INFO << "Version mismatch: layer version " << layerVersion
                  << " plugin compiled for layer version" << dtk::pluginManagerHandler().pluginsLayerVersion(path)
                  << " for plugin " << path;
        return false;
    }

    for (QVariant item : dtk::pluginManagerHandler().dependencies(path)) {

        QVariantMap mitem = item.toMap();
        QString na_mitem = mitem.value("name").toString();
        QString ve_mitem = mitem.value("version").toString();
        QString key = dtk::pluginManagerHandler().pluginPath(na_mitem);

        if (!dtk::pluginManagerHandler().hasName(na_mitem)) {
            dtkWarn() << Q_FUNC_INFO << "Missing dependency:" << na_mitem << "for plugin" << path;
            status = false;
            continue;
        }

        if (!checkVersion(dtk::pluginManagerHandler().version(key), ve_mitem)) {
            dtkWarn() << Q_FUNC_INFO << "Version mismatch:" << na_mitem << "version" << dtk::pluginManagerHandler().version(key) << "but" << ve_mitem << "required for plugin" << path;
            status = false;
            continue;
        }

        if (!check(key, false)) {
            dtkWarn() << Q_FUNC_INFO << "Corrupted dependency:" << na_mitem << "for plugin" << path;
            status = false;
            continue;
        }
    }

    return status;
}

template <typename T> inline bool gnomonPluginManagerPrivate<T>::checkVersion(const QString ref_version, const QString elem_version)
{
    QStringList ve_ref_list = ref_version.split(".");
    QStringList ve_elem_list = elem_version.split(".");

    //if major level different return false
    if (ve_ref_list.at(0).toInt() != ve_elem_list.at(0).toInt()) {
        return false;
    }

    //if minor level of ref < elem return false
    if (ve_ref_list.at(1).toInt() < ve_elem_list.at(1).toInt()) {
        return false;

    } else {
        // no patch level specified in ref
        if (ve_ref_list.size() < 3 || ve_elem_list.size() < 3) {
            return true;
        }

        //if same minor level, compare patch level
        if ((ve_ref_list.at(1).toInt() == ve_elem_list.at(1).toInt()) &&
            (ve_ref_list.at(2).toInt() < ve_elem_list.at(2).toInt())) {
            return false;
        }
        //else minor level of elem < ref , then don't compare patch level
    }

    return true;
}

// ///////////////////////////////////////////////////////////////////
// gnomonPluginManager
// ///////////////////////////////////////////////////////////////////

template <typename T> inline gnomonPluginManager<T>::gnomonPluginManager(void) : d(new gnomonPluginManagerPrivate<T>)
{

}

template <typename T> inline gnomonPluginManager<T>::~gnomonPluginManager(void)
{
    delete d;
}


template <typename T> inline void gnomonPluginManager<T>::setAutoLoading(bool value)
{
    d->autoLoading = value;
}

template <typename T> inline bool gnomonPluginManager<T>::autoLoading(void) const
{
    return d->autoLoading;
}

template <typename T> inline void gnomonPluginManager<T>::loadFromName(const QString& plugin_name)
{
    QString full_name = plugin_name % "Plugin";

    auto i = dtk::pluginManagerHandler().namesBegin();
    auto e = dtk::pluginManagerHandler().namesEnd();

    while (i != e) {
        if (QString::compare(i.value(), full_name) == 0) {
            this->load(i.key());
            return;
        }
        ++i;
    }
    dtkWarn() << Q_FUNC_INFO << plugin_name << " not found ";
    dtkWarn() << Q_FUNC_INFO << "keys" << dtk::pluginManagerHandler().pluginPaths() << dtk::pluginManagerHandler().names();
}

template <typename T> inline void gnomonPluginManager<T>::setLayerVersion(const QString& layer_version)
{
    d->layerVersion = layer_version;
}

template <typename T> void gnomonPluginManager<T>::initialize(const QString& path)
{
    for (QString path2 : path.split(":", Qt::SkipEmptyParts)) {
        if (path2.startsWith ("~/")) {
            path2.replace (0, 1, QDir::homePath());
        }

        QDir dir(path2);

        for (QFileInfo info : dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot))
            dtk::pluginManagerHandler().scan(info.absoluteFilePath(), true);

        if (d->autoLoading) {
            for (QFileInfo info : dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot)) {
                if (d->checkConcept(info.absoluteFilePath())) {
                    dtkInfo() << Q_FUNC_INFO << "load" << info.absoluteFilePath();
                    this->load(info.absoluteFilePath());
                }
            }
        } else {
            dtkInfo() << Q_FUNC_INFO << "auto loading disabled";
        }
    }
}

template <typename T> inline void gnomonPluginManager<T>::uninitialize(void)
{
    for (const QString& path : d->loaders.keys()) {
        this->unload(path);
    }
}

template <typename T> inline void gnomonPluginManager<T>::load(const QString& path)
{
    if (!QLibrary::isLibrary(path)) {
        QString error = "Unable to load non library file " + path;
        dtkWarn() << Q_FUNC_INFO << error;
        return;
    }

    if (!d->check(path)) {
        QString error = "check failure for plugin file " + path;
        dtkWarn() << Q_FUNC_INFO << error;
        return;
    }

    QPluginLoader *loader = new QPluginLoader(path); // should not set this as parent to avoid bad deallocation

    if (!loader) {
        QString error = "Empty loader for file " + path;
        dtkWarn() << Q_FUNC_INFO << error;
        return;
    }

    loader->setLoadHints(QLibrary::ExportExternalSymbolsHint);

    dtkTrace() << Q_FUNC_INFO << "Loading plugin from " << path;

    if (!loader->load()) {
        QString error = "Unable to load ";
        error += path;
        error += " - ";
        error += loader->errorString();
        dtkWarn() << Q_FUNC_INFO << error;
        delete loader;
        return;
    }

    T *plugin = qobject_cast<T *>(loader->instance());

    if (!plugin) {
        QString error = "Unable to retrieve ";
        error += path;
        dtkWarn() << Q_FUNC_INFO << error;
        loader->unload();
        delete loader;
        return;
    }

    plugin->initialize();

    d->loaders.insert(path, loader);

    QString name =  loader->metaData().value("MetaData").toObject().value("name").toString();
    d->metadatas.insert(name, loader->metaData().value("MetaData").toObject());
    dtkTrace() << Q_FUNC_INFO << "Loaded plugin " <<  name  << " from " << path;
}

template <typename T> inline void gnomonPluginManager<T>::unload(const QString& path)
{
    QPluginLoader *loader = d->loaders.value(path);

    T *plugin = qobject_cast<T *>(loader->instance());

    if (plugin)
        plugin->uninitialize();

    if (loader->unload()) {
        d->loaders.remove(path);
        delete loader;
    } else {
        dtkWarn() << Q_FUNC_INFO << loader->errorString();
    }
}

template <typename T> inline QStringList gnomonPluginManager<T>::pluginsPath(void) const
{
    return d->loaders.keys();
}

template <typename T> inline QMap<QString, QString> gnomonPluginManager<T>::metadatas(const QString& name) const
{
    QVariantMap v_map = d->metadatas[name].toVariantMap();
    QMap<QString, QString> res;
    QMap<QString, QVariant>::const_iterator it = v_map.constBegin();
    while (it != v_map.constEnd()) {
        res.insert(it.key(), it.value().toString());
        ++it;
    }

    return res;
}

template <typename T> inline QStringList gnomonPluginManager<T>::availablePlugins(void) const
{
    return dtk::pluginManagerHandler().names();
}
