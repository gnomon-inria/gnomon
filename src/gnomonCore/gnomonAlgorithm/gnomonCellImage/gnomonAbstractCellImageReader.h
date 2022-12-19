#pragma once

#include <gnomonCoreExport>

#include <dtkCore>

#include "gnomonCore/gnomonCorePlugin.h"
#include "gnomonAlgorithm/gnomonAbstractAlgorithm.h"

#include "gnomonForm/gnomonCellImage/gnomonCellImage.h"

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonAbstractCellImageReader : public gnomonAbstractAlgorithm
{
public:
    virtual ~gnomonAbstractCellImageReader(void) = default;

public:
    virtual void setPath(const QString& path) = 0;

public:
    virtual std::shared_ptr<gnomonCellImageSeries> cellImage(void) = 0;

public:
    static inline QString defaultSetter(QString formName) {
        return {};
    };
    static inline QString defaultGetter(QString formName) {
        return {};
    };
    static inline QString defaultOutput(QString formName) {
        if(formName == "gnomonCellImage") {
            return {"cellImage"};
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

DTK_DECLARE_OBJECT        (gnomonAbstractCellImageReader *)
DTK_DECLARE_PLUGIN        (gnomonAbstractCellImageReader, GNOMONCORE_EXPORT)
GNOMON_DECLARE_PLUGIN_FACTORY(gnomonAbstractCellImageReader, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractCellImageReader, GNOMONCORE_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractCellImageReader, GNOMONCORE_EXPORT, cellImageReader);
}

//
// gnomonAbstractCellImageReader.h ends here
