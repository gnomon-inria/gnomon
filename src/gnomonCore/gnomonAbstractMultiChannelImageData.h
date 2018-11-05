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

class vtkImageData;

#include <QtCore>

#include <dtkCore>

#include <gnomonCoreExport.h>

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonAbstractMultiChannelImageData
{
public:
             gnomonAbstractMultiChannelImageData(void) = default;
    virtual ~gnomonAbstractMultiChannelImageData(void) {};

    virtual gnomonAbstractMultiChannelImageData* clone(void) const = 0;

public:
    virtual QList<QString> channelNames(void) = 0;

    virtual void addChannelImage(const QString& channelName, vtkImageData *image) = 0;
    virtual vtkImageData* channelImage(const QString& channelName) const = 0;
};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonAbstractMultiChannelImageData *)
DTK_DECLARE_PLUGIN        (gnomonAbstractMultiChannelImageData, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractMultiChannelImageData, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractMultiChannelImageData, GNOMONCORE_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractMultiChannelImageData, GNOMONCORE_EXPORT, multichannelimagedata);
}

// gnomonAbstractMultiChannelImageData.h ends here
