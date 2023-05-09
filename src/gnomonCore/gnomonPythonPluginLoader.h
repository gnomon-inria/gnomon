#pragma once

#include <dtkCorePluginManager.h>
#include "gnomonPluginFactory.h"
#include "gnomonPluginManager.h"
#include <gnomonCoreExport.h>

#include <QtCore>

QMap<QString, gnomonPluginManagerBase*>& pluginsManagers(void);

GNOMONCORE_EXPORT void loadPluginGroup(const QString&);

//list available plugins from a group.
GNOMONCORE_EXPORT QStringList availablePluginsFromGroup(const QString&);
GNOMONCORE_EXPORT QMap<QString, QString> pluginMetadata(const QString &group, const QString &plugin_name);
