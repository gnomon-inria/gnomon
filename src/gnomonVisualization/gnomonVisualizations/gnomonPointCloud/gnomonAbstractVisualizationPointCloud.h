#pragma once

#include <QtCore>

#include <gnomonVisualizationExport.h>

#include <dtkCore/dtkCorePlugin>
#include <dtkCore/dtkCoreParameters>

#include <gnomonCore/gnomonForm/gnomonPointCloud/gnomonPointCloud.h>
#include "gnomonVisualizations/gnomonAbstractVisualization.h"

class GNOMONVISUALIZATION_EXPORT gnomonAbstractVisualizationPointCloud : public gnomonAbstractVisualization
{
    Q_OBJECT

public:
    gnomonAbstractVisualizationPointCloud(void) : gnomonAbstractVisualization() {}
    virtual ~gnomonAbstractVisualizationPointCloud(void) = default;

public:
	virtual void setPointCloud(std::shared_ptr<gnomonPointCloudSeries> pointCloud) = 0;
	virtual std::shared_ptr<gnomonPointCloudSeries> pointCloud(void) = 0;

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
};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonAbstractVisualizationPointCloud *)
DTK_DECLARE_PLUGIN        (gnomonAbstractVisualizationPointCloud, GNOMONVISUALIZATION_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractVisualizationPointCloud, GNOMONVISUALIZATION_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractVisualizationPointCloud, GNOMONVISUALIZATION_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonVisualization layer
// /////////////////////////////////////////////////////////////////

namespace gnomonVisualization {
    DTK_DECLARE_CONCEPT(gnomonAbstractVisualizationPointCloud, GNOMONVISUALIZATION_EXPORT, visualizationPointCloud);
}

//
// gnomonAbstractVisualizationPointCloud.h ends here
