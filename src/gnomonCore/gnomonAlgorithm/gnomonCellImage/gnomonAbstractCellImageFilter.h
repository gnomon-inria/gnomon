#pragma once

#include <QtCore>

#include <gnomonCoreExport.h> // enables the visibility of the concept

#include <dtkCore>

#include "gnomonCore/gnomonCorePlugin.h"
#include "gnomonAlgorithm/gnomonAbstractAlgorithm.h"
#include "gnomonForm/gnomonCellImage/gnomonCellImage.h"

class GNOMONCORE_EXPORT gnomonAbstractCellImageFilter : public gnomonAbstractAlgorithm
{

    //Inputs
public:
  virtual void setInput(std::shared_ptr<gnomonCellImageSeries> images) = 0;

    // Outputs
public:
    virtual std::shared_ptr<gnomonCellImageSeries> output() const = 0;
    virtual std::shared_ptr<gnomonCellImageSeries> input() const = 0;

public:
    static inline QString defaultSetter(QString formName) {
        if(formName == "gnomonCellImage") {
            return {"setInput"};
        }
        return {};
    };
    static inline QString defaultGetter(QString formName) {
        if(formName == "gnomonCellImage") {
            return {"input"};
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

DTK_DECLARE_OBJECT(gnomonAbstractCellImageFilter *)

DTK_DECLARE_PLUGIN(gnomonAbstractCellImageFilter, GNOMONCORE_EXPORT)
GNOMON_DECLARE_PLUGIN_FACTORY(gnomonAbstractCellImageFilter, GNOMONCORE_EXPORT, cellImageFilter, gnomonCore)


//
// gnomonAbstractCellImageFilter.h ends here
