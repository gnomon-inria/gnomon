#pragma once

#include <QtCore>

#include <gnomonCoreExport.h> // enables the visibility of the concept

#include <dtkCore>
#include "gnomonCore/gnomonCorePlugin.h"

#include "gnomonAlgorithm/gnomonAbstractAlgorithm.h"

#include "gnomonForm/gnomonCellImage/gnomonCellImage.h"
#include "gnomonForm/gnomonPointCloud/gnomonPointCloud.h"
#include "gnomonForm/gnomonImage/gnomonImage.h"

class GNOMONCORE_EXPORT gnomonAbstractPointCloudFromImage : public gnomonAbstractAlgorithm
{
    //Inputs
public:
  virtual void setInput(std::shared_ptr<gnomonImageSeries> image) = 0;

    // Outputs
public:
    virtual std::shared_ptr<gnomonPointCloudSeries> output() const = 0;
    virtual std::shared_ptr<gnomonImageSeries> input() const = 0;

public:
    static inline QString defaultSetter(QString formName) {
        if(formName == "gnomonImage") {
            return {"setInput"};
        }
        return {};
    };
    static inline QString defaultGetter(QString formName) {
        if(formName == "gnomonImage") {
            return {"input"};
        }
        return {};
    };
    static inline QString defaultOutput(QString formName) {
        if(formName == "gnomonPointCloud") {
            return {"output"};
        }
        return {};
    };
};

DTK_DECLARE_OBJECT(gnomonAbstractPointCloudFromImage *)

DTK_DECLARE_PLUGIN(gnomonAbstractPointCloudFromImage, GNOMONCORE_EXPORT)
GNOMON_DECLARE_PLUGIN_FACTORY(gnomonAbstractPointCloudFromImage, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractPointCloudFromImage, GNOMONCORE_EXPORT)


namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractPointCloudFromImage, GNOMONCORE_EXPORT, pointCloudFromImage);
}

//
// gnomonAbstractPointCloudFromImage.h ends here
