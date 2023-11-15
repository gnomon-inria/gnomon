#pragma once

#include <QtCore>

#include <gnomonVisualizationExport.h>

#include "gnomonCore/gnomonCorePlugin.h"
#include <dtkCore/dtkCoreParameters>

#include <gnomonCore/gnomonForm/gnomonPointCloud/gnomonPointCloud.h>
#include "gnomonVisualizations/gnomonAbstractVtkVisualization.h"

class GNOMONVISUALIZATION_EXPORT gnomonAbstractPointCloudVtkVisualization : public gnomonAbstractVtkVisualization
{
    Q_OBJECT

public:
    gnomonAbstractPointCloudVtkVisualization(void) : gnomonAbstractVtkVisualization() {}
    virtual ~gnomonAbstractPointCloudVtkVisualization(void) = default;

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

DTK_DECLARE_OBJECT        (gnomonAbstractPointCloudVtkVisualization *)
DTK_DECLARE_PLUGIN        (gnomonAbstractPointCloudVtkVisualization, GNOMONVISUALIZATION_EXPORT)
GNOMON_DECLARE_PLUGIN_FACTORY(gnomonAbstractPointCloudVtkVisualization, GNOMONVISUALIZATION_EXPORT, pointCloudVtkVisualization, gnomonVisualization)


//
// gnomonAbstractPointCloudVtkVisualization.h ends here
