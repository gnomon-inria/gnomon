#pragma once

#include <QtCore>
#include <dtkCore>
#include <gnomonCoreExport>

class GNOMONCORE_EXPORT gnomonAbstractDataDriver
{
public:
         gnomonAbstractDataDriver(void) = default;
virtual ~gnomonAbstractDataDriver(void) = default;

public:
    virtual const QString& name(void) = 0;
    virtual bool insert(const QString& doc) = 0;
    virtual bool remove(const QString& key) = 0;
    virtual bool protect(const QString& key) = 0;
    virtual const QString& find(const QString& query) = 0;
};

DTK_DECLARE_OBJECT        (gnomonAbstractDataDriver *)
DTK_DECLARE_PLUGIN        (gnomonAbstractDataDriver, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractDataDriver, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractDataDriver, GNOMONCORE_EXPORT)

namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractDataDriver, GNOMONCORE_EXPORT, dataDriver);
}
