#pragma once

#include <gnomonCoreExport>

#include <QtCore>

#include <dtkCore>

#include "gnomonCore/gnomonCorePlugin.h"
#include "gnomonAlgorithm/gnomonAbstractAlgorithm.h"

#include "gnomonForm/gnomonCellImage/gnomonCellImage.h"
#include "gnomonForm/gnomonImage/gnomonImage.h"
#include "gnomonForm/gnomonPointCloud/gnomonPointCloud.h"
#include "gnomonForm/gnomonBinaryImage/gnomonBinaryImage.h"

class GNOMONCORE_EXPORT gnomonAbstractCellImageFromImage : public gnomonAbstractAlgorithm
{

public:
    virtual void setInput(std::shared_ptr<gnomonImageSeries> image_series) = 0;
    virtual inline void setCellPoints(std::shared_ptr<gnomonPointCloudSeries> pointCloud_series) {
        dtkWarn()<<Q_FUNC_INFO<<"Not implemented";
    };
    virtual inline void setBinaryImage(std::shared_ptr<gnomonBinaryImageSeries> binary_image_series) {
        dtkWarn()<<Q_FUNC_INFO<<"Not implemented";
    };

public:
    virtual std::shared_ptr<gnomonImageSeries> input(void) const = 0;
    virtual inline std::shared_ptr<gnomonPointCloudSeries> cellPoints(void) const {
        dtkWarn()<<Q_FUNC_INFO<<"Not implemented";
        return nullptr;
    };
    virtual inline std::shared_ptr<gnomonBinaryImageSeries> binaryImage() const {
        dtkWarn()<<Q_FUNC_INFO<<"Not implemented";
        return nullptr;
    };
    virtual std::shared_ptr<gnomonCellImageSeries> output() const = 0;

public:
    static inline QString defaultSetter(QString formName) {
        if(formName == "gnomonImage") {
            return {"setInput"};
        } else if (formName == "gnomonPointCloud") {
            return {"setCellPoints"};
        } else if (formName == "gnomonBinaryImage") {
            return {"setBinaryImage"};
        }
        return {};
    };
    static inline QString defaultGetter(QString formName) {
        if(formName == "gnomonImage") {
            return {"input"};
        } else if (formName == "gnomonPointCloud") {
            return {"cellPoints"};
        } else if (formName == "gnomonBinaryImage") {
            return {"binaryImage"};
        }
        return {};
    };
    static inline QString defaultOutput(QString formName) {
        if(formName == "gnomonCellImage") {
            return {"output"};
        }
        return {};
    };

};

DTK_DECLARE_OBJECT(gnomonAbstractCellImageFromImage *)

DTK_DECLARE_PLUGIN(gnomonAbstractCellImageFromImage, GNOMONCORE_EXPORT)
GNOMON_DECLARE_PLUGIN_FACTORY(gnomonAbstractCellImageFromImage, GNOMONCORE_EXPORT, cellImageFromImage, gnomonCore)


//
// gnomonAbstractCellImageFromImage.h ends here
