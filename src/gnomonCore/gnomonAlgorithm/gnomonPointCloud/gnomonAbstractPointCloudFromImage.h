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

class dtkImage;
class gnomonPointCloud;
class gnomonCellImage;
class gnomonImagesSerie;

class GNOMONCORE_EXPORT gnomonAbstractPointCloudFromImage : public gnomonAbstractAlgorithm
{

    //Inputs
public:
  virtual void setParameter(const QString& parameterName, const QVariant& parameterValue) override = 0;
  virtual QMap<QString, gnomonCoreParameter *> parameters(void) const override = 0;
  virtual void setInput(gnomonImagesSerie *images) = 0;

    // Outputs
public:
    virtual gnomonPointCloud *output() const = 0;
    virtual gnomonImagesSerie *input() const = 0;

public:
    virtual void run(void) override = 0;
    virtual QString documentation(void) override = 0;

};

DTK_DECLARE_OBJECT(gnomonAbstractPointCloudFromImage *)

DTK_DECLARE_PLUGIN(gnomonAbstractPointCloudFromImage, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractPointCloudFromImage, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractPointCloudFromImage, GNOMONCORE_EXPORT)


namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractPointCloudFromImage, GNOMONCORE_EXPORT, pointCloudFromImage);
}

//
// gnomonAbstractPointCloudFromImage.h ends here
