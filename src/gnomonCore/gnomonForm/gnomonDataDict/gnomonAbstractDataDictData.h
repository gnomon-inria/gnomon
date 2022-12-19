#pragma once

#include <gnomonCoreExport.h>

#include <QtCore>

#include <dtkCore>

#include "gnomonCore/gnomonCorePlugin.h"
#include "gnomonForm/gnomonAbstractFormData"

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonAbstractDataDictData: public gnomonAbstractFormData
{
public:
             gnomonAbstractDataDictData(void) = default;
    virtual ~gnomonAbstractDataDictData(void) {};

    virtual gnomonAbstractDataDictData* clone(void) const = 0;

    virtual void fromGnomonForm(gnomonAbstractForm* other) = 0;

//  ///////////////////////////////////////////////////////////////////
//  Metadata
//  ///////////////////////////////////////////////////////////////////

public:
    virtual QMap<QString,QString> metadata(void) const = 0;
    virtual QString dataName(void) const = 0;
    virtual const QString pluginName(void) = 0;

//  ///////////////////////////////////////////////////////////////////
//  DataDict concept
//  ///////////////////////////////////////////////////////////////////

public:
    virtual QStringList keys() const = 0;

    virtual QVariant get(const QString& key) = 0; 
    //virtual QVariantMap get() const = 0;
};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonAbstractDataDictData *)
DTK_DECLARE_PLUGIN        (gnomonAbstractDataDictData, GNOMONCORE_EXPORT)
GNOMON_DECLARE_PLUGIN_FACTORY(gnomonAbstractDataDictData, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractDataDictData, GNOMONCORE_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractDataDictData, GNOMONCORE_EXPORT, dataDictData);
}

//
// gnomonDataDictData.h ends here
