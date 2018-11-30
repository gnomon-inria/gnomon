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

class gnomonCoreParameter;
class gnomonImagesSerie;
class gnomonCellImage;

class GNOMONCORE_EXPORT gnomonAbstractCellImageFromImage : public QRunnable
{
public:
    virtual void setInput(gnomonImagesSerie *images_serie) = 0;
    virtual void setParameter(const QString&, QVariant) = 0;
    virtual void setUseMargins(bool use_margins) = 0;
    virtual void setBackgroundLabel(long label) = 0;
    virtual void setPredefinedProperties(const QStringList& names) = 0;

public:
    virtual void run(void) = 0;

public:
    virtual gnomonImagesSerie *input() = 0;
    virtual gnomonImagesSerie *output() = 0;
    virtual QMap<QString, gnomonCoreParameter *> parameters(void) = 0;
    virtual gnomonCellImage *computedImage(void) const = 0;
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
