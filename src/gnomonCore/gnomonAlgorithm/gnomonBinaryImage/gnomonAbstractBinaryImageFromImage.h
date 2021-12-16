#pragma once

#include <QtCore>

#include <gnomonCoreExport.h> // enables the visibility of the concept

#include <dtkCore>

#include "gnomonAlgorithm/gnomonAbstractAlgorithm.h"
#include "gnomonForm/gnomonBinaryImage/gnomonBinaryImage.h"
#include "gnomonForm/gnomonImage/gnomonImage.h"

class dtkImage;

class GNOMONCORE_EXPORT gnomonAbstractBinaryImageFromImage : public gnomonAbstractAlgorithm
{
    //Inputs
public:
    virtual void setInput(gnomonImageSeries *image) = 0;
    virtual void setInitialization(gnomonBinaryImageSeries *init) = 0;

    // Outputs
public:
    virtual gnomonImageSeries *input(void) = 0;
    virtual gnomonBinaryImageSeries *initialization(void) = 0;
    virtual gnomonBinaryImageSeries *output(void) const = 0;


public:
  virtual void setParameter(const QString& parameterName, const QVariant& parameterValue) override = 0;
  virtual dtkCoreParameters parameters(void) const override = 0;


public:
    virtual void run(void) override = 0;
    virtual QString documentation(void) override = 0;

};

DTK_DECLARE_OBJECT(gnomonAbstractBinaryImageFromImage *)

DTK_DECLARE_PLUGIN(gnomonAbstractBinaryImageFromImage, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractBinaryImageFromImage, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractBinaryImageFromImage, GNOMONCORE_EXPORT)


namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractBinaryImageFromImage, GNOMONCORE_EXPORT, binaryImageFromImage);
}

//
// gnomonAbstractBinaryImageFromImage.h ends here
