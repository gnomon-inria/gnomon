#pragma once

#include <gnomonCoreExport>

#include <dtkCore>
#include "gnomonCore/gnomonCorePlugin.h"

#include "gnomonAlgorithm/gnomonAbstractFormWriter.h"

#include "gnomonForm/gnomonImage/gnomonImage.h"

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonAbstractImageWriter : public gnomonAbstractFormWriter
{
public:
    virtual ~gnomonAbstractImageWriter(void) = default;

public:
    virtual void setImage(std::shared_ptr<gnomonImageSeries> image_series) = 0;

public:
    static inline QString defaultSetter(QString formName) {
        if(formName == "gnomonImage") {
            return {"setImage"};
        }
        return {};
    };
    static inline QString defaultGetter(QString formName) {
        if(formName == "gnomonImage") {
            return {"image"};
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

DTK_DECLARE_OBJECT        (gnomonAbstractImageWriter *)
DTK_DECLARE_PLUGIN        (gnomonAbstractImageWriter, GNOMONCORE_EXPORT)
GNOMON_DECLARE_PLUGIN_FACTORY(gnomonAbstractImageWriter, GNOMONCORE_EXPORT, imageWriter, gnomonCore)


//
// gnomonAbstractImageWriter.h ends here
