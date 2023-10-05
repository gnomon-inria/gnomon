#pragma once

#include <gnomonCoreExport>

#include <dtkCore>
#include "gnomonCore/gnomonCorePlugin.h"

#include "gnomonAlgorithm/gnomonAbstractFormWriter.h"

#include "gnomonForm/gnomonPointCloud/gnomonPointCloud.h"

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonAbstractPointCloudWriter : public gnomonAbstractFormWriter
{
public:
    virtual ~gnomonAbstractPointCloudWriter(void) = default;

public:
    virtual void setPointCloud(std::shared_ptr<gnomonPointCloudSeries> pointCloud) = 0;

public:
    static inline QString defaultSetter(QString formName) {
        if(formName == "gnomonPointCloud") {
            return {"setPointCloud"};
        }
        return {};
    };
    static inline QString defaultGetter(QString formName) {
        if(formName == "gnomonPointCloud") {
            return {"pointCloud"};
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

DTK_DECLARE_OBJECT        (gnomonAbstractPointCloudWriter *)
DTK_DECLARE_PLUGIN        (gnomonAbstractPointCloudWriter, GNOMONCORE_EXPORT)
GNOMON_DECLARE_PLUGIN_FACTORY(gnomonAbstractPointCloudWriter, GNOMONCORE_EXPORT, pointCloudWriter, gnomonCore)


//
// gnomonAbstractImageWriter.h ends here
