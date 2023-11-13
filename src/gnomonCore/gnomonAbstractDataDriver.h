#pragma once

#include <QtCore>
#include <dtkCore>
#include <gnomonCoreExport>
#include "gnomonCore/gnomonCorePlugin.h"

class GNOMONCORE_EXPORT gnomonAbstractDataDriver
{
public:
         gnomonAbstractDataDriver(void) = default;
virtual ~gnomonAbstractDataDriver(void) = default;

public:
    virtual QString name(void) = 0;
    virtual QString insert(const QString& doc) = 0;
    virtual bool delete_one(const QString& key) = 0;
    virtual bool protect(const QString& key) = 0;
    virtual QStringList find(const QString& query) = 0;
};

DTK_DECLARE_OBJECT        (gnomonAbstractDataDriver *)
DTK_DECLARE_PLUGIN        (gnomonAbstractDataDriver, GNOMONCORE_EXPORT)
GNOMON_DECLARE_PLUGIN_FACTORY(gnomonAbstractDataDriver, GNOMONCORE_EXPORT, dataDriver, gnomonCore)
