#pragma once

#include <gnomonCoreExport.h>
#include <dtkCore>

#include "gnomonCore/gnomonCorePlugin.h"
#include "gnomonAlgorithm/gnomonAbstractFormWriter.h"
#include "gnomonForm/gnomonDataDict/gnomonDataDict.h"

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonAbstractDataDictWriter : public gnomonAbstractFormWriter
{
public:
    virtual ~gnomonAbstractDataDictWriter(void) = default;

public:
    virtual void setDataDict(std::shared_ptr<gnomonDataDictSeries> datadict) = 0;

    static inline QString defaultSetter(QString formName) {
        if(formName == "gnomonDataDict") {
            return {"setDataDict"};
        }
        return {};
    };
    static inline QString defaultGetter(QString formName) {
        if(formName == "gnomonDataDict") {
            return {"dataDict"};
        }
        return {};
    };
    static inline QString defaultOutput(QString formName) {
        return {};
    };
};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT(gnomonAbstractDataDictWriter *)
DTK_DECLARE_PLUGIN(gnomonAbstractDataDictWriter, GNOMONCORE_EXPORT)
GNOMON_DECLARE_PLUGIN_FACTORY(gnomonAbstractDataDictWriter, GNOMONCORE_EXPORT)
//DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractDataDictWriter, GNOMONCORE_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

namespace gnomonCore {
    GNOMON_DECLARE_CONCEPT(gnomonAbstractDataDictWriter, GNOMONCORE_EXPORT, dataDictWriter);
}

//
// gnomonAbstractDataDictWriter.h ends here
