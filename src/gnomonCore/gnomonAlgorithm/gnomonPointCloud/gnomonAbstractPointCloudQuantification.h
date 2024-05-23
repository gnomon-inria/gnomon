#pragma once

#include <QtCore>

#include <gnomonCoreExport.h> // enables the visibility of the concept

#include <dtkCore>
#include "gnomonCore/gnomonCorePlugin.h"

#include "gnomonAlgorithm/gnomonAbstractAlgorithm.h"

#include "gnomonForm/gnomonPointCloud/gnomonPointCloud.h"
#include "gnomonForm/gnomonDataFrame/gnomonDataFrame.h"
#include "gnomonForm/gnomonImage/gnomonImage.h"
#include "gnomonForm/gnomonMesh/gnomonMesh.h"

class GNOMONCORE_EXPORT gnomonAbstractPointCloudQuantification : public gnomonAbstractAlgorithm
{
    //Inputs
public:
    virtual void setImage(std::shared_ptr<gnomonImageSeries> image) {dtkWarn()<<Q_FUNC_INFO<< "not implemented";};
    virtual void setMesh(std::shared_ptr<gnomonMeshSeries> mesh) {dtkWarn()<<Q_FUNC_INFO<< "not implemented";};
    virtual void setPointCloud(std::shared_ptr<gnomonPointCloudSeries> pointCloud) = 0;

    virtual inline std::shared_ptr<gnomonPointCloudSeries> pointCloud() = 0;
    virtual inline std::shared_ptr<gnomonImageSeries> image() const {
        dtkWarn()<<Q_FUNC_INFO<<"Not implemented";
        return nullptr;
    };
    virtual inline std::shared_ptr<gnomonMeshSeries> mesh() const {
        dtkWarn()<<Q_FUNC_INFO<<"Not implemented";
        return nullptr;
    };

    // Outputs
public:
    virtual std::shared_ptr<gnomonPointCloudSeries> outputPointCloud() const = 0;
    virtual std::shared_ptr<gnomonDataFrameSeries> outputDataFrame() const = 0;

public:
    static inline QString defaultSetter(QString formName) {
        if (formName == "gnomonPointCloud") {
            return {"setPointCloud"};
        } else if (formName == "gnomonImage") {
            return {"setImage"};
        } else if (formName == "gnomonMesh") {
            return {"setMesh"};
        }
        return {};
    };
    static inline QString defaultGetter(QString formName) {
        if (formName == "gnomonPointCloud") {
            return {"pointCloud"};
        } else if (formName == "gnomonImage") {
            return {"image"};
        } else if (formName == "gnomonMesh") {
            return {"mesh"};
        }
        return {};
    };
    static inline QString defaultOutput(QString formName) {
        if (formName == "gnomonPointCloud") {
            return {"outputPointCloud"};
        } else if (formName == "gnomonDataFrame") {
            return {"outputDataFrame"};
        }
        return {};
    };

public:
    virtual int run(void) override = 0;
    virtual QString documentation(void) override = 0;

};

DTK_DECLARE_OBJECT(gnomonAbstractPointCloudQuantification *)

DTK_DECLARE_PLUGIN(gnomonAbstractPointCloudQuantification, GNOMONCORE_EXPORT)
GNOMON_DECLARE_PLUGIN_FACTORY(gnomonAbstractPointCloudQuantification, GNOMONCORE_EXPORT, pointCloudQuantification, gnomonCore)


//
// gnomonAbstractPointCloudQuantification.h ends here
