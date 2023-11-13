#pragma once

#include <QtCore>

#include <gnomonCoreExport.h> // enables the visibility of the concept

#include <dtkCore>

#include "gnomonCore/gnomonCorePlugin.h"
#include "gnomonAlgorithm/gnomonAbstractAlgorithm.h"

#include "gnomonForm/gnomonDataDict/gnomonDataDict.h"
#include "gnomonForm/gnomonCellImage/gnomonCellImage.h"
#include "gnomonForm/gnomonTree/gnomonTree.h"
#include "gnomonForm/gnomonImage/gnomonImage.h"

class GNOMONCORE_EXPORT gnomonAbstractCellImageTracking : public gnomonAbstractAlgorithm
{
    //Inputs
public:
  virtual void setCellImage(std::shared_ptr<gnomonCellImageSeries> cellimage) = 0;
  virtual void setImage(std::shared_ptr<gnomonImageSeries> image) = 0;
  virtual inline void setTransformation(std::shared_ptr<gnomonDataDictSeries> data_dict_series) {
      dtkWarn()<<Q_FUNC_INFO<<"Not implemented";
  };

    // Outputs
public:
    virtual std::shared_ptr<gnomonCellImageSeries> cellImage() const = 0;
    virtual std::shared_ptr<gnomonTreeSeries> tree() const { return nullptr; };

public:
    static inline QString defaultSetter(QString formName) {
        if(formName == "gnomonImage") {
            return {"setImage"};
        } else if(formName == "gnomonCellImage") {
            return {"setCellImage"};
        } else if(formName == "gnomonDataDict") {
            return {"setTransformation"};
        }
        return {};
    };
    static inline QString defaultGetter(QString formName) {
        if(formName == "gnomonImage") {
            return {"image"};
        } else if(formName == "gnomonCellImage") {
            return {"cellImageInput"};
        } else if(formName == "gnomonDataDict") {
            return {"transformation"};
        }
        return {};
    };
    static inline QString defaultOutput(QString formName) {
        if(formName == "gnomonCellImage") {
            return {"cellImage"};
        } else if(formName == "gnomonTree") {
            return {"tree"};
        }
        return {};
    };
};

DTK_DECLARE_OBJECT(gnomonAbstractCellImageTracking *)

DTK_DECLARE_PLUGIN(gnomonAbstractCellImageTracking, GNOMONCORE_EXPORT)
GNOMON_DECLARE_PLUGIN_FACTORY(gnomonAbstractCellImageTracking, GNOMONCORE_EXPORT, cellImageTracking, gnomonCore)


//
// gnomonAbstractCellImageTracking.h ends here
