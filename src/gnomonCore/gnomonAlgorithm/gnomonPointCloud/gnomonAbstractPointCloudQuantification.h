#pragma once

#include <QtCore>

#include <gnomonCoreExport.h> // enables the visibility of the concept

#include <dtkCore>
#include "gnomonCore/gnomonCorePlugin.h"

#include "gnomonAlgorithm/gnomonAbstractAlgorithm.h"

#include "gnomonForm/gnomonPointCloud/gnomonPointCloud.h"
#include "gnomonForm/gnomonDataFrame/gnomonDataFrame.h"
#include "gnomonForm/gnomonImage/gnomonImage.h"

class GNOMONCORE_EXPORT gnomonAbstractPointCloudQuantification : public gnomonAbstractAlgorithm
{
    //Inputs
public:
  virtual void setImage(std::shared_ptr<gnomonImageSeries> image) = 0;
  virtual void setPointCloud(std::shared_ptr<gnomonPointCloudSeries> pointCloud) = 0;

    // Outputs
public:
    virtual std::shared_ptr<gnomonPointCloudSeries> pointCloud() const = 0;
    virtual std::shared_ptr<gnomonDataFrameSeries> dataFrame() const = 0;

public:
    static inline QString defaultSetter(QString formName) {
        if(formName == "gnomonImage") {
            return {"setImage"};
        } else if(formName == "gnomonPointCloud") {
            return {"setPointCloud"};
        }
        return {};
    };
    static inline QString defaultGetter(QString formName) {
        if(formName == "gnomonImage") {
            return {"getImageInput"};
        } else if(formName == "gnomonPointCloud") {
            return {"getPointCloudInput"};
        }
        return {};
    };
    static inline QString defaultOutput(QString formName) {
        if(formName == "gnomonPointCloud") {
            return {"pointCloud"};
        } else if(formName == "gnomonDataFrame") {
            return {"dataFrame"};
        }
        return {};
    };


public:
    virtual int run(void) override = 0;
    virtual QString documentation(void) override = 0;

};

DTK_DECLARE_OBJECT(gnomonAbstractPointCloudQuantification *)

DTK_DECLARE_PLUGIN(gnomonAbstractPointCloudQuantification, GNOMONCORE_EXPORT)
GNOMON_DECLARE_PLUGIN_FACTORY(gnomonAbstractPointCloudQuantification, GNOMONCORE_EXPORT)
//DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractPointCloudQuantification, GNOMONCORE_EXPORT)


namespace gnomonCore {
    GNOMON_DECLARE_CONCEPT(gnomonAbstractPointCloudQuantification, GNOMONCORE_EXPORT, pointCloudQuantification);
}

//
// gnomonAbstractPointCloudQuantification.h ends here
