#pragma once

#include <gnomonCoreExport>

#include <dtkCore>

#include "gnomonAlgorithm/gnomonAbstractAlgorithm.h"

#include "gnomonForm/gnomonImage/gnomonImage.h"

class gnomonImage;


// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonAbstractImageReader : public gnomonAbstractAlgorithm
{
public:
    virtual ~gnomonAbstractImageReader(void) = default;

public:
    virtual void setPath(const QString& path) = 0;

public:
    virtual std::shared_ptr<gnomonImageSeries> image() = 0;

public:
    static inline QString defaultSetter(QString formName) {
        return {};
    };
    static inline QString defaultGetter(QString formName) {
        return {};
    };
    static inline QString defaultOutput(QString formName) {
        if(formName == "gnomonImage") {
            return {"image"};
        }
        return {};
    };

public:
    virtual QStringList extensions(void) = 0;
    virtual QString thumbshots(void) {
        return "";
    };

};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonAbstractImageReader *)
DTK_DECLARE_PLUGIN        (gnomonAbstractImageReader, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractImageReader, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractImageReader, GNOMONCORE_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractImageReader, GNOMONCORE_EXPORT, imageReader);
}

//
// gnomonAbstractImageReader.h ends here
