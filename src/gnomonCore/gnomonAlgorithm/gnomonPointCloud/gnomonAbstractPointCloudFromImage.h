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
#include "gnomonForm/gnomonPointCloud/gnomonPointCloud.h"
#include "gnomonForm/gnomonImage/gnomonImage.h"

class GNOMONCORE_EXPORT gnomonAbstractPointCloudFromImage : public gnomonAbstractAlgorithm
{

    //Inputs
public:
  virtual void setParameter(const QString& parameterName, const QVariant& parameterValue) override = 0;
  virtual QMap<QString, dtkCoreParameter *> parameters(void) const override = 0;
  virtual void setInput(gnomonImageSeries *image) = 0;

    // Outputs
public:
    virtual gnomonPointCloudSeries *output() const = 0;
    virtual gnomonImageSeries *input() const = 0;

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
