#pragma once

#include <QtCore>

#include <gnomonCoreExport.h> // enables the visibility of the concept

#include <dtkCore>

#include "gnomonAlgorithm/gnomonAbstractAlgorithm.h"

#include "gnomonForm/gnomonDataDict/gnomonDataDict.h"
#include "gnomonForm/gnomonCellImage/gnomonCellImage.h"
#include "gnomonForm/gnomonTree/gnomonTree.h"
#include "gnomonForm/gnomonImage/gnomonImage.h"

class GNOMONCORE_EXPORT gnomonAbstractCellImageTracking : public gnomonAbstractAlgorithm
{

    //Inputs
public:
  virtual void setParameter(const QString& parameterName, const QVariant& parameterValue) override = 0;
  virtual dtkCoreParameters parameters(void) const override = 0;

  virtual void setCellImage(gnomonCellImageSeries *cellimage) = 0;
  virtual void setImage(gnomonImageSeries *image) = 0;
  virtual inline void setTransformation(gnomonDataDictSeries *data_dict_series) {
      dtkWarn()<<Q_FUNC_INFO<<"Not implemented";
  };

    // Outputs
public:
    virtual gnomonCellImageSeries *cellImage() const = 0;
    virtual gnomonTreeSeries *tree() const { return nullptr; };

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


public:
    virtual void run(void) override = 0;
    virtual QString documentation(void) override = 0;

};

DTK_DECLARE_OBJECT(gnomonAbstractCellImageTracking *)

DTK_DECLARE_PLUGIN(gnomonAbstractCellImageTracking, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractCellImageTracking, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractCellImageTracking, GNOMONCORE_EXPORT)


namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractCellImageTracking, GNOMONCORE_EXPORT, cellImageTracking);
}

//
// gnomonAbstractCellImageTracking.h ends here
