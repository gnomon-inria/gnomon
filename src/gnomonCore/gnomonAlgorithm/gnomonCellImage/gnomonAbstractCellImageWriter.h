#pragma once

#include <gnomonCoreExport>

#include <dtkCore>

#include "gnomonCore/gnomonCorePlugin.h"
#include "gnomonAlgorithm/gnomonAbstractFormWriter.h"

#include "gnomonForm/gnomonCellImage/gnomonCellImage.h"

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonAbstractCellImageWriter : public gnomonAbstractFormWriter
{
public:
    virtual ~gnomonAbstractCellImageWriter(void) = default;

public:
    virtual void setCellImage(std::shared_ptr<gnomonCellImageSeries> cellimage) = 0;

public:
    static inline QString defaultSetter(QString formName) {
        if(formName == "gnomonCellImage") {
            return {"setCellImage"};
        }
        return {};
    };
    static inline QString defaultGetter(QString formName) {
        if(formName == "gnomonCellImage") {
            return {"cellImage"};
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

DTK_DECLARE_OBJECT        (gnomonAbstractCellImageWriter *)
DTK_DECLARE_PLUGIN        (gnomonAbstractCellImageWriter, GNOMONCORE_EXPORT)
GNOMON_DECLARE_PLUGIN_FACTORY(gnomonAbstractCellImageWriter, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractCellImageWriter, GNOMONCORE_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractCellImageWriter, GNOMONCORE_EXPORT, cellImageWriter);
}

//
// gnomonAbstractImageWriter.h ends here
