#pragma once

#include <QtCore>

#include <gnomonCoreExport.h> // enables the visibility of the concept

#include <dtkCore>

#include "gnomonCore/gnomonCorePlugin.h"
#include "gnomonAlgorithm/gnomonAbstractAlgorithm.h"

#include "gnomonForm/gnomonCellImage/gnomonCellImage.h"
#include "gnomonForm/gnomonDataFrame/gnomonDataFrame.h"
#include "gnomonForm/gnomonImage/gnomonImage.h"

class GNOMONCORE_EXPORT gnomonAbstractCellImageQuantification : public gnomonAbstractAlgorithm
{
    //Inputs
public:
  virtual void setImage(std::shared_ptr<gnomonImageSeries> image) {dtkWarn()<<Q_FUNC_INFO<< "not implemented";};
  virtual void setCellImage(std::shared_ptr<gnomonCellImageSeries> cellimage) = 0;

    // Outputs
public:
    virtual std::shared_ptr<gnomonCellImageSeries> cellImage() const = 0;
    virtual std::shared_ptr<gnomonDataFrameSeries> dataFrame() const = 0;

public:
    static inline QString defaultSetter(QString formName) {
        if(formName == "gnomonImage") {
            return {"setImage"};
        } else if(formName == "gnomonCellImage") {
            return {"setCellImage"};
        }
        return {};
    };
    static inline QString defaultGetter(QString formName) {
        if(formName == "gnomonImage") {
            return {"getImageInput"};
        } else if(formName == "gnomonCellImage") {
            return {"getCellImageInput"};
        }
        return {};
    };
    static inline QString defaultOutput(QString formName) {
        if(formName == "gnomonCellImage") {
            return {"cellImage"};
        } else if(formName == "gnomonDataFrame") {
            return {"dataFrame"};
        }
        return {};
    };
};

DTK_DECLARE_OBJECT(gnomonAbstractCellImageQuantification *)

DTK_DECLARE_PLUGIN(gnomonAbstractCellImageQuantification, GNOMONCORE_EXPORT)
GNOMON_DECLARE_PLUGIN_FACTORY(gnomonAbstractCellImageQuantification, GNOMONCORE_EXPORT)
//DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractCellImageQuantification, GNOMONCORE_EXPORT)


namespace gnomonCore {
    GNOMON_DECLARE_CONCEPT(gnomonAbstractCellImageQuantification, GNOMONCORE_EXPORT, cellImageQuantification);
}

//
// gnomonAbstractCellImageQuantification.h ends here
