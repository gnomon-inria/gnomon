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
#include "gnomonForm/gnomonTree/gnomonTree.h"

class dtkImage;

class GNOMONCORE_EXPORT gnomonAbstractTreeConstructor : public gnomonAbstractAlgorithm
{

    //Inputs
public:
  virtual void setParameter(const QString& parameterName, const QVariant& parameterValue) override = 0;
  virtual QMap<QString, gnomonCoreParameter *> parameters(void) const override = 0;

    // Outputs
public:
    virtual gnomonTreeSeries *output() const = 0;

public:
    virtual void run(void) override = 0;
    virtual QString documentation(void) override = 0;

};

DTK_DECLARE_OBJECT(gnomonAbstractTreeConstructor *)

DTK_DECLARE_PLUGIN(gnomonAbstractTreeConstructor, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractTreeConstructor, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractTreeConstructor, GNOMONCORE_EXPORT)


namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractTreeConstructor, GNOMONCORE_EXPORT, treeConstructor);
}

//
// gnomonAbstractTreeConstructor.h ends here
