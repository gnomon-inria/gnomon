#pragma once

#include <gnomonCoreExport>

#include <dtkCore>

#include "gnomonCore/gnomonCorePlugin.h"
#include "gnomonAlgorithm/gnomonAbstractFormWriter.h"

#include "gnomonForm/gnomonBinaryImage/gnomonBinaryImage.h"

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonAbstractBinaryImageWriter : public gnomonAbstractFormWriter
{
public:
    virtual ~gnomonAbstractBinaryImageWriter(void) = default;

public:
    virtual void setBinaryImage(std::shared_ptr<gnomonBinaryImageSeries> binaryImage) = 0;

    static inline QString defaultSetter(QString formName) {
        if(formName == "gnomonBinaryImage") {
            return {"setBinaryImage"};
        }
        return {};
    };
    static inline QString defaultGetter(QString formName) {
        if(formName == "gnomonBinaryImage") {
            return {"binaryImage"};
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

DTK_DECLARE_OBJECT        (gnomonAbstractBinaryImageWriter *)
DTK_DECLARE_PLUGIN        (gnomonAbstractBinaryImageWriter, GNOMONCORE_EXPORT)
GNOMON_DECLARE_PLUGIN_FACTORY(gnomonAbstractBinaryImageWriter, GNOMONCORE_EXPORT)
//DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractBinaryImageWriter, GNOMONCORE_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

namespace gnomonCore {
    GNOMON_DECLARE_CONCEPT(gnomonAbstractBinaryImageWriter, GNOMONCORE_EXPORT, binaryImageWriter);
}

//
// gnomonAbstractImageWriter.h ends here
