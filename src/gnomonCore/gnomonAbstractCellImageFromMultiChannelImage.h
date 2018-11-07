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

#include <gnomonCoreExport.h> // enables the visibility of the concept

#include <dtkCore>

class gnomonMultiChannelImage;
class gnomonCellImage;

class GNOMONCORE_EXPORT gnomonAbstractCellImageFromMultiChannelImage
{
public:
    virtual ~gnomonAbstractCellImageFromMultiChannelImage(void) {}

public:
    virtual void setImage(gnomonMultiChannelImage* image) = 0;

public:
    virtual void run(void) = 0;

public:
    virtual gnomonCellImage *segmentedImage(void) = 0;

public:
	virtual void setParameter(const QString& parameterName, const QVariant& parameterValue) = 0;
	virtual const QMap<QString, QVariant>& parameters(void) const = 0;
	virtual       QMap<QString, QVariant>& parameters(void) = 0;

};

DTK_DECLARE_OBJECT(gnomonAbstractCellImageFromMultiChannelImage *)

DTK_DECLARE_PLUGIN(gnomonAbstractCellImageFromMultiChannelImage, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractCellImageFromMultiChannelImage, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractCellImageFromMultiChannelImage, GNOMONCORE_EXPORT)


namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractCellImageFromMultiChannelImage, GNOMONCORE_EXPORT, cellimageFromMultichannelimage);
}

//
// gnomonAbstractCellImageFromMultiChannelImage.h ends here