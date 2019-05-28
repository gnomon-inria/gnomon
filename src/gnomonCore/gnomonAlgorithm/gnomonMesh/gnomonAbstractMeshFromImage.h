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
#include "gnomonForm/gnomonMesh/gnomonMesh.h"

class dtkImage;
class gnomonCellImage;
class gnomonImagesSerie;

class GNOMONCORE_EXPORT gnomonAbstractMeshFromImage : public gnomonAbstractAlgorithm
{

    //Inputs
public:
  virtual void setParameter(const QString& parameterName, const QVariant& parameterValue) override = 0;
  virtual QMap<QString, gnomonCoreParameter *> parameters(void) const override = 0;
  virtual void setInput(gnomonImagesSerie *images) = 0;

    // Outputs
public:
    virtual gnomonMeshSeries *output() const = 0;
    virtual gnomonImagesSerie *input() const = 0;

public:
    virtual void run(void) override = 0;
    virtual QString documentation(void) override = 0;

};

DTK_DECLARE_OBJECT(gnomonAbstractMeshFromImage *)

DTK_DECLARE_PLUGIN(gnomonAbstractMeshFromImage, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractMeshFromImage, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractMeshFromImage, GNOMONCORE_EXPORT)


namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractMeshFromImage, GNOMONCORE_EXPORT, meshFromImage);
}

//
// gnomonAbstractMeshFromImage.h ends here
