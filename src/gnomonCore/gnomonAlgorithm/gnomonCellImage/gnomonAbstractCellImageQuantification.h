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

class gnomonImagesSerie;
class gnomonCellImage;
class gnomonDataFrame;

class GNOMONCORE_EXPORT gnomonAbstractCellImageQuantification : public gnomonAbstractAlgorithm
{

    //Inputs
public:
  virtual void setParameter(const QString& parameterName, const QVariant& parameterValue) override = 0;
  virtual QMap<QString, gnomonCoreParameter *> parameters(void) const override = 0;

  virtual void setImage(gnomonImagesSerie *image) = 0;
  virtual void setCellImage(gnomonCellImage *cellimage) = 0;

    // Outputs
public:
    virtual gnomonCellImage *cellImage() const = 0;
    virtual gnomonDataFrame *dataFrame() const = 0;

public:
    virtual void run(void) override = 0;

};

DTK_DECLARE_OBJECT(gnomonAbstractCellImageQuantification *)

DTK_DECLARE_PLUGIN(gnomonAbstractCellImageQuantification, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractCellImageQuantification, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractCellImageQuantification, GNOMONCORE_EXPORT)


namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractCellImageQuantification, GNOMONCORE_EXPORT, cellImageQuantification);
}

//
// gnomonAbstractCellImageQuantification.h ends here
