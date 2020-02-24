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
#include "gnomonForm/gnomonImage/gnomonImage.h"

class dtkImage;

class GNOMONCORE_EXPORT gnomonAbstractImageConstructor : public gnomonAbstractAlgorithm
{

    //Inputs
public:
  virtual void setParameter(const QString& parameterName, const QVariant& parameterValue) override = 0;
  virtual QMap<QString, gnomonCoreParameter *> parameters(void) const override = 0;

    // Outputs
public:
    virtual gnomonImageSeries *output() const = 0;

public:
    virtual void run(void) override = 0;
    virtual QString documentation(void) override = 0;

};

DTK_DECLARE_OBJECT(gnomonAbstractImageConstructor *)

DTK_DECLARE_PLUGIN(gnomonAbstractImageConstructor, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractImageConstructor, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractImageConstructor, GNOMONCORE_EXPORT)


namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractImageConstructor, GNOMONCORE_EXPORT, imageConstructor);
}

//
// gnomonAbstractImageConstructor.h ends here
