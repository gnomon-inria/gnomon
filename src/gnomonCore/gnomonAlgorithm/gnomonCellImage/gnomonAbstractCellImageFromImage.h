#pragma once

#include <gnomonCoreExport>

#include <QtCore>

#include <dtkCore>
#include "gnomonAlgorithm/gnomonAbstractAlgorithm.h"

#include "gnomonForm/gnomonCellImage/gnomonCellImage.h"
#include "gnomonForm/gnomonImage/gnomonImage.h"
#include "gnomonForm/gnomonPointCloud/gnomonPointCloud.h"
#include "gnomonForm/gnomonBinaryImage/gnomonBinaryImage.h"

class GNOMONCORE_EXPORT gnomonAbstractCellImageFromImage : public gnomonAbstractAlgorithm
{

public:
    virtual void setInput(gnomonImageSeries *image_series) = 0;
    virtual inline void setCellPoints(gnomonPointCloudSeries *pointCloud_series) {
        dtkWarn()<<Q_FUNC_INFO<<"Not implemented";
    };
    virtual inline void setBinaryImage(gnomonBinaryImageSeries *binary_image_series) {
        dtkWarn()<<Q_FUNC_INFO<<"Not implemented";
    };

public:
    virtual gnomonImageSeries *input(void) const = 0;
    virtual inline gnomonPointCloudSeries *cellPoints(void) const {
        dtkWarn()<<Q_FUNC_INFO<<"Not implemented";
        return nullptr;
    };
    virtual inline gnomonBinaryImageSeries *binaryImageSeries() const {
        dtkWarn()<<Q_FUNC_INFO<<"Not implemented";
        return nullptr;
    };
    virtual gnomonCellImageSeries *output() const = 0;

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
            return {"binaryImageSeries"};
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
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractCellImageFromImage, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractCellImageFromImage, GNOMONCORE_EXPORT)

namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractCellImageFromImage, GNOMONCORE_EXPORT, cellImageFromImage);
}

//
// gnomonAbstractCellImageFromImage.h ends here
