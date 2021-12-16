// Version: $Id$
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#pragma once

#include <QtCore>

#include <gnomonCoreExport.h> // enables the visibility of the concept

#include <dtkCore>

#include "gnomonAlgorithm/gnomonAbstractAlgorithm.h"
#include "gnomonForm/gnomonBinaryImage/gnomonBinaryImage.h"

class dtkImage;

class GNOMONCORE_EXPORT gnomonAbstractBinaryImageFromImageConstructor : public gnomonAbstractAlgorithm
{

    //Inputs
public:
  virtual void setParameter(const QString& parameterName, const QVariant& parameterValue) override = 0;
  virtual dtkCoreParameters parameters(void) const override = 0;

    // Outputs
public:
    virtual gnomonBinaryImageSeries *output() const = 0;

public:
    virtual void run(void) override = 0;
    virtual QString documentation(void) override = 0;

};

DTK_DECLARE_OBJECT(gnomonAbstractBinaryImageFromImageConstructor *)

DTK_DECLARE_PLUGIN(gnomonAbstractBinaryImageFromImageConstructor, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractBinaryImageFromImageConstructor, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractBinaryImageFromImageConstructor, GNOMONCORE_EXPORT)


namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractBinaryImageFromImageConstructor, GNOMONCORE_EXPORT, binaryImageFromImageConstructor);
}

//
// gnomonAbstractBinaryImageFromImageConstructor.h ends here
