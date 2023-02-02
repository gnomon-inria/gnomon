#pragma once

#include <gnomonCoreExport.h>

#include <dtkCore>

#include "gnomonCore/gnomonCorePlugin.h"
#include "gnomonAlgorithm/gnomonAbstractAlgorithm.h"

#include "gnomonForm/gnomonBinaryImage/gnomonBinaryImage.h"

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonAbstractBinaryImageReader : public gnomonAbstractAlgorithm
{
public:
    virtual ~gnomonAbstractBinaryImageReader(void) = default;

public:
    virtual void setPath(const QString& path) = 0;

public:
    virtual std::shared_ptr<gnomonBinaryImageSeries> binaryImage(void) = 0;

    static inline QString defaultSetter(QString formName) {
        return {};
    };
    static inline QString defaultGetter(QString formName) {
        return {};
    };
    static inline QString defaultOutput(QString formName) {
        if(formName == "gnomonBinaryImage") {
            return {"binaryImage"};
        }
        return {};
    };


public:
    virtual QStringList extensions(void) = 0;
    virtual QString preview(void) {
        return "";
    };
};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT(gnomonAbstractBinaryImageReader *)
DTK_DECLARE_PLUGIN(gnomonAbstractBinaryImageReader, GNOMONCORE_EXPORT)
GNOMON_DECLARE_PLUGIN_FACTORY(gnomonAbstractBinaryImageReader, GNOMONCORE_EXPORT)
//DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractBinaryImageReader, GNOMONCORE_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

namespace gnomonCore {
    GNOMON_DECLARE_CONCEPT(gnomonAbstractBinaryImageReader, GNOMONCORE_EXPORT, binaryImageReader);
}

//
// gnomonAbstractBinaryImageReader.h ends here
