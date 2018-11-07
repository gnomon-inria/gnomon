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

class GNOMONCORE_EXPORT gnomonAbstractMultiChannelImageReader
{
public:
    virtual ~gnomonAbstractMultiChannelImageReader(void) {}

public:
    virtual void setPath(const QString& path) = 0;

public:
    virtual void run(void) = 0;

public:
    virtual gnomonMultiChannelImage *image(void) = 0;

public:
    virtual QStringList types(void) = 0;
};

DTK_DECLARE_OBJECT(gnomonAbstractMultiChannelImageReader *)

DTK_DECLARE_PLUGIN(gnomonAbstractMultiChannelImageReader, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractMultiChannelImageReader, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractMultiChannelImageReader, GNOMONCORE_EXPORT)


namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractMultiChannelImageReader, GNOMONCORE_EXPORT, multiChannelImageReader);
}

//
// gnomonAbstractMultiChannelImageReader.h ends here
