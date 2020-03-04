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

#include "gnomonForm/gnomonPointCloud/gnomonPointCloud.h"
#include "gnomonForm/gnomonDataFrame/gnomonDataFrame.h"
#include "gnomonForm/gnomonImage/gnomonImage.h"

class gnomonDataFrame;

class GNOMONCORE_EXPORT gnomonAbstractPointCloudQuantification : public gnomonAbstractAlgorithm
{

    //Inputs
public:
  virtual void setParameter(const QString& parameterName, const QVariant& parameterValue) override = 0;
  virtual QMap<QString, gnomonCoreParameter *> parameters(void) const override = 0;

  virtual void setImage(gnomonImageSeries *image) = 0;
  virtual void setPointCloud(gnomonPointCloudSeries *pointCloud) = 0;

    // Outputs
public:
    virtual gnomonPointCloudSeries *pointCloud() const = 0;
    virtual gnomonDataFrameSeries *dataFrame() const = 0;

public:
    virtual void run(void) override = 0;
    virtual QString documentation(void) override = 0;

};

DTK_DECLARE_OBJECT(gnomonAbstractPointCloudQuantification *)

DTK_DECLARE_PLUGIN(gnomonAbstractPointCloudQuantification, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractPointCloudQuantification, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractPointCloudQuantification, GNOMONCORE_EXPORT)


namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractPointCloudQuantification, GNOMONCORE_EXPORT, pointCloudQuantification);
}

//
// gnomonAbstractPointCloudQuantification.h ends here
