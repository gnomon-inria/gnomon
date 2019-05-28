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

class gnomonCoreParameter;

#include "gnomonForm/gnomonCellImage/gnomonCellImage.h"
#include "gnomonForm/gnomonImage/gnomonImage.h"

class GNOMONCORE_EXPORT gnomonAbstractCellImageFromImage : public gnomonAbstractAlgorithm
{

public:
    virtual void setParameter(const QString& parameterName, const QVariant& parameterValue) override = 0;
    virtual QMap<QString, gnomonCoreParameter *> parameters(void) const override = 0;
    virtual void run(void) override = 0;
    virtual QString documentation(void) override = 0;

public:
    virtual void setInput(gnomonImageSeries *images_serie) = 0;
    virtual void setUseMargins(bool use_margins) = 0;
    virtual void setBackgroundLabel(long label) = 0;
    virtual void setPredefinedProperties(const QStringList& names) = 0;

public:
    virtual gnomonImageSeries *input() = 0;
    virtual gnomonImageSeries *output() = 0;
    virtual gnomonCellImageSeries *computedImage(void) const = 0;
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
