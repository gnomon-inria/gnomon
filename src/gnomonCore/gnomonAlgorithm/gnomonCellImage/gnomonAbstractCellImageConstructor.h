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
#include "gnomonForm/gnomonCellImage/gnomonCellImage.h"

class dtkImage;

class GNOMONCORE_EXPORT gnomonAbstractCellImageConstructor : public gnomonAbstractAlgorithm
{

    //Inputs
public:
  virtual void setParameter(const QString& parameterName, const QVariant& parameterValue) override = 0;
  virtual QMap<QString, dtkCoreParameter *> parameters(void) const override = 0;

    // Outputs
public:
    virtual gnomonCellImageSeries *output() const = 0;

public:
    virtual void run(void) override = 0;
    virtual QString documentation(void) override = 0;

};

DTK_DECLARE_OBJECT(gnomonAbstractCellImageConstructor *)

DTK_DECLARE_PLUGIN(gnomonAbstractCellImageConstructor, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractCellImageConstructor, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractCellImageConstructor, GNOMONCORE_EXPORT)


namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractCellImageConstructor, GNOMONCORE_EXPORT, cellImageConstructor);
}

//
// gnomonAbstractCellImageConstructor.h ends here
