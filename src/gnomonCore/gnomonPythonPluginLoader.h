#pragma once

#include <gnomonCoreExport.h>

#include <QtCore>


GNOMONCORE_EXPORT void loadPluginGroup(const QString&);
GNOMONCORE_EXPORT QStringList availablePluginsFromGroup(const QString&);
GNOMONCORE_EXPORT QMap<QString, QString> pluginMetadata(const QString &group, const QString &plugin_name);
