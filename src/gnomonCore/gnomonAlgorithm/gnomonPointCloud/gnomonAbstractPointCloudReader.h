#pragma once

#include <gnomonCoreExport>

#include <dtkCore>
#include "gnomonCore/gnomonCorePlugin.h"

#include "gnomonAlgorithm/gnomonAbstractAlgorithm.h"

#include "gnomonForm/gnomonPointCloud/gnomonPointCloud.h"

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonAbstractPointCloudReader : public gnomonAbstractAlgorithm
{
public:
    virtual ~gnomonAbstractPointCloudReader(void) = default;

public:
    virtual void setPath(const QString& path) = 0;

public:
    virtual std::shared_ptr<gnomonPointCloudSeries> pointCloud(void) = 0;

public:
    static inline QString defaultSetter(QString formName) {
        return {};
    };
    static inline QString defaultGetter(QString formName) {
        return {};
    };
    static inline QString defaultOutput(QString formName) {
        if(formName == "gnomonPointCloud") {
            return {"pointCloud"};
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

DTK_DECLARE_OBJECT        (gnomonAbstractPointCloudReader *)
DTK_DECLARE_PLUGIN        (gnomonAbstractPointCloudReader, GNOMONCORE_EXPORT)
GNOMON_DECLARE_PLUGIN_FACTORY(gnomonAbstractPointCloudReader, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractPointCloudReader, GNOMONCORE_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractPointCloudReader, GNOMONCORE_EXPORT, pointCloudReader);
}

//
// gnomonAbstractPointCloudReader.h ends here
