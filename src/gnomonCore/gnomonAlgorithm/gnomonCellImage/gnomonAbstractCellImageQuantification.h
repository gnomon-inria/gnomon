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
#include "gnomonForm/gnomonDataFrame/gnomonDataFrame.h"
#include "gnomonForm/gnomonImage/gnomonImage.h"

class gnomonDataFrame;

class GNOMONCORE_EXPORT gnomonAbstractCellImageQuantification : public gnomonAbstractAlgorithm
{

    //Inputs
public:
  virtual void setParameter(const QString& parameterName, const QVariant& parameterValue) override = 0;
  virtual dtkCoreParameters parameters(void) const override = 0;

  virtual void setImage(gnomonImageSeries *image) {dtkWarn()<<Q_FUNC_INFO<< "not implemented";};
  virtual void setCellImage(gnomonCellImageSeries *cellimage) = 0;

    // Outputs
public:
    virtual gnomonCellImageSeries *cellImage() const = 0;
    virtual gnomonDataFrameSeries *dataFrame() const = 0;

public:
    virtual void run(void) override = 0;
    virtual QString documentation(void) override = 0;

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
