#pragma once

#include <gnomonCoreExport>

#include <QtCore>

// /////////////////////////////////////////////////////////////////
// gnomonPluginManagerBase interface
// /////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonPluginManagerBase
{
public:
    virtual ~gnomonPluginManagerBase(void) = default;

    virtual void   initialize(const QString& path) = 0;
    virtual void uninitialize(void) = 0;

    virtual void setAutoLoading(bool value) = 0;
    virtual bool autoLoading(void) const = 0;

    virtual void   load(const QString& path) = 0;
    virtual void unload(const QString& path) = 0;

    virtual void loadFromName(const QString& plugin_name) = 0;
    virtual void setLayerVersion(const QString& layer_version) = 0;

    virtual QMap<QString, QString> metadatas(const QString& name) const = 0;
    //return plugins path
    virtual QStringList pluginsPath(void) const = 0;
    //return plugins names (defined in the json)
    virtual QStringList availablePlugins(void) const = 0;
};

// ///////////////////////////////////////////////////////////////////
// gnomonPluginManager declaration
// ///////////////////////////////////////////////////////////////////

template <typename T>
class gnomonPluginManagerPrivate;

template <typename T>
class gnomonPluginManager : public gnomonPluginManagerBase
{
public:
     gnomonPluginManager(void);
    ~gnomonPluginManager(void);

    void   initialize(const QString& path) override;
    void uninitialize(void) override;

    void setAutoLoading(bool value) override;
    bool autoLoading(void) const override;

    void   load(const QString& path) override;
    void unload(const QString& path) override;

    void loadFromName(const QString& plugin_name) override;
    void setLayerVersion(const QString& layer_version) override;

    QMap<QString, QString> metadatas(const QString& name) const override;
    //return plugins path
    QStringList pluginsPath(void) const override;
    //return plugins names (defined in the json)
    QStringList availablePlugins(void) const override;

protected:
    gnomonPluginManagerPrivate<T> *d;
};

#include "gnomonPluginManager.tpp"

//
// gnomonPluginManager.h ends here
