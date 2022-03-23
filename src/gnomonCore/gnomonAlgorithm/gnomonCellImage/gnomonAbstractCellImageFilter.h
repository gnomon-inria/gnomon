#pragma once

#include <QtCore>

#include <gnomonCoreExport.h> // enables the visibility of the concept

#include <dtkCore>

#include "gnomonAlgorithm/gnomonAbstractAlgorithm.h"
#include "gnomonForm/gnomonCellImage/gnomonCellImage.h"

class dtkImage;

class GNOMONCORE_EXPORT gnomonAbstractCellImageFilter : public gnomonAbstractAlgorithm
{

    //Inputs
public:
  virtual void setParameter(const QString& parameterName, const QVariant& parameterValue) override = 0;
  virtual dtkCoreParameters parameters(void) const override = 0;
  virtual void setInput(gnomonCellImageSeries *images) = 0;

    // Outputs
public:
    virtual gnomonCellImageSeries *output() const = 0;
    virtual gnomonCellImageSeries *input() const = 0;

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


public:
    virtual void run(void) override = 0;
    virtual QString documentation(void) override = 0;

};

DTK_DECLARE_OBJECT(gnomonAbstractCellImageFilter *)

DTK_DECLARE_PLUGIN(gnomonAbstractCellImageFilter, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractCellImageFilter, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractCellImageFilter, GNOMONCORE_EXPORT)


namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractCellImageFilter, GNOMONCORE_EXPORT, cellImageFilter);
}

//
// gnomonAbstractCellImageFilter.h ends here
