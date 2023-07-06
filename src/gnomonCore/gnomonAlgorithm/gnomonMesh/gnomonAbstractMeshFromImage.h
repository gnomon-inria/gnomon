#pragma once

#include <QtCore>

#include <gnomonCoreExport.h> // enables the visibility of the concept

#include <dtkCore>
#include "gnomonCore/gnomonCorePlugin.h"

#include "gnomonAlgorithm/gnomonAbstractAlgorithm.h"
#include "gnomonForm/gnomonMesh/gnomonMesh.h"
#include "gnomonForm/gnomonImage/gnomonImage.h"
#include "gnomonForm/gnomonCellImage/gnomonCellImage.h"
#include "gnomonForm/gnomonBinaryImage/gnomonBinaryImage.h"


class GNOMONCORE_EXPORT gnomonAbstractMeshFromImage : public gnomonAbstractAlgorithm
{
public:
    virtual inline void setImage(std::shared_ptr<gnomonImageSeries> image) {
        dtkWarn()<<Q_FUNC_INFO<<"Not implemented";
    };
    virtual inline void setCellImage(std::shared_ptr<gnomonCellImageSeries> cellImage) {
        dtkWarn()<<Q_FUNC_INFO<<"Not implemented";
    };
    virtual inline void setBinaryImage(std::shared_ptr<gnomonBinaryImageSeries> binaryImage) {
        dtkWarn()<<Q_FUNC_INFO<<"Not implemented";
    };

public:
    virtual inline std::shared_ptr<gnomonImageSeries> image(void) {
        dtkWarn()<<Q_FUNC_INFO<<"Not implemented";
        return nullptr;
    };
    virtual inline std::shared_ptr<gnomonCellImageSeries> cellImage(void) const {
        dtkWarn()<<Q_FUNC_INFO<<"Not implemented";
        return nullptr;
    };
    virtual inline std::shared_ptr<gnomonBinaryImageSeries> binaryImage() const {
        dtkWarn()<<Q_FUNC_INFO<<"Not implemented";
        return nullptr;
    };

public:
    virtual std::shared_ptr<gnomonMeshSeries> output() const = 0;

public:
    static inline QString defaultSetter(QString formName) {
        if (formName == "gnomonImage") {
            return {"setImage"};
        } else if (formName == "gnomonCellImage") {
            return {"setCellImage"};
        } else if (formName == "gnomonBinaryImage") {
            return {"setBinaryImage"};
        }
        return {};
    };
    static inline QString defaultGetter(QString formName) {
        if (formName == "gnomonImage") {
            return {"image"};
        } else if (formName == "gnomonCellImage") {
            return {"cellImage"};
        } else if (formName == "gnomonBinaryImage") {
            return {"binaryImage"};
        }
        return {};
    };
    static inline QString defaultOutput(QString formName) {
        if(formName == "gnomonMesh") {
            return {"output"};
        }
        return {};
    };
};

DTK_DECLARE_OBJECT(gnomonAbstractMeshFromImage *)

DTK_DECLARE_PLUGIN(gnomonAbstractMeshFromImage, GNOMONCORE_EXPORT)
GNOMON_DECLARE_PLUGIN_FACTORY(gnomonAbstractMeshFromImage, GNOMONCORE_EXPORT)
//DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractMeshFromImage, GNOMONCORE_EXPORT)


namespace gnomonCore {
    GNOMON_DECLARE_CONCEPT(gnomonAbstractMeshFromImage, GNOMONCORE_EXPORT, meshFromImage);
}

//
// gnomonAbstractMeshFromImage.h ends here
