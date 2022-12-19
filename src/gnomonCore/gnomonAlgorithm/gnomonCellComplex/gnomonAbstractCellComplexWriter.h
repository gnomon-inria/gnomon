#pragma once

#include <gnomonCoreExport>

#include <dtkCore>

#include "gnomonCore/gnomonCorePlugin.h"
#include "gnomonAlgorithm/gnomonAbstractFormWriter.h"

#include "gnomonForm/gnomonCellComplex/gnomonCellComplex.h"

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonAbstractCellComplexWriter : public gnomonAbstractFormWriter
{
public:
    virtual ~gnomonAbstractCellComplexWriter(void) = default;

public:
    virtual void setCellComplex(std::shared_ptr<gnomonCellComplexSeries> cellcomplex) = 0;

public:
    static inline QString defaultSetter(QString formName) {
        if(formName == "gnomonCellComplex") {
            return {"setCellComplex"};
        }
        return {};
    };
    static inline QString defaultGetter(QString formName) {
        if(formName == "gnomonCellComplex") {
            return {"cellComplex"};
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

DTK_DECLARE_OBJECT        (gnomonAbstractCellComplexWriter *)
DTK_DECLARE_PLUGIN        (gnomonAbstractCellComplexWriter, GNOMONCORE_EXPORT)
GNOMON_DECLARE_PLUGIN_FACTORY(gnomonAbstractCellComplexWriter, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractCellComplexWriter, GNOMONCORE_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractCellComplexWriter, GNOMONCORE_EXPORT, cellComplexWriter);
}

//
// gnomonAbstractImageWriter.h ends here
