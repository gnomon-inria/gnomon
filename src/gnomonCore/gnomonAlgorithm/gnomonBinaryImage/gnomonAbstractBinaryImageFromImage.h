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
    virtual gnomonImageSeries *input() = 0;
    virtual gnomonBinaryImageSeries *initialization() = 0;
    virtual gnomonBinaryImageSeries *output() const = 0;


public:
    void setParameter(const QString& parameterName, const QVariant& parameterValue) override = 0;
    virtual dtkCoreParameters parameters() const override = 0;


public:
    void run() override = 0;
    virtual QString documentation() override = 0;

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
