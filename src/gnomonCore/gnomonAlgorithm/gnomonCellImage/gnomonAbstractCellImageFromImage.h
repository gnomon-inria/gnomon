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

#include <gnomonCoreExport>

#include <QtCore>

#include <dtkCore>
#include "gnomonAlgorithm/gnomonAbstractAlgorithm.h"

class dtkCoreParameter;

#include "gnomonForm/gnomonCellImage/gnomonCellImage.h"
#include "gnomonForm/gnomonImage/gnomonImage.h"
#include "gnomonForm/gnomonPointCloud/gnomonPointCloud.h"

class GNOMONCORE_EXPORT gnomonAbstractCellImageFromImage : public gnomonAbstractAlgorithm
{

public:
    virtual void setParameter(const QString& parameterName, const QVariant& parameterValue) override = 0;
    virtual dtkCoreParameters parameters(void) const override = 0;
    virtual void run(void) override = 0;
    virtual QString documentation(void) override = 0;

public:
    virtual void setInput(gnomonImageSeries *image_series) = 0;
    virtual void setCellPoints(gnomonPointCloudSeries *pointCloud_series) {dtkWarn()<<Q_FUNC_INFO<< "not implemented";};

public:
    virtual gnomonImageSeries *input(void) const = 0;
    virtual gnomonPointCloudSeries *cellPoints(void) const { dtkWarn()<<Q_FUNC_INFO<< "not implemented";
                                                             return nullptr;};
    virtual gnomonCellImageSeries *output(void) const = 0;
};

DTK_DECLARE_OBJECT(gnomonAbstractCellImageFromImage *)

DTK_DECLARE_PLUGIN(gnomonAbstractCellImageFromImage, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractCellImageFromImage, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractCellImageFromImage, GNOMONCORE_EXPORT)

namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractCellImageFromImage, GNOMONCORE_EXPORT, cellImageFromImage);
}

//
// gnomonAbstractCellImageFromImage.h ends here
