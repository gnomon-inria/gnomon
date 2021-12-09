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

#include <gnomonCoreExport.h>

#include <QtCore>

#include <dtkCore>

class dtkImage;

class GNOMONCORE_EXPORT gnomonAbstractBinaryImageData {
public:
    gnomonAbstractBinaryImageData(void) = default;
    virtual ~gnomonAbstractBinaryImageData(void) = default;
    virtual gnomonAbstractBinaryImageData* clone(void) const = 0;

/*
 *  METADATA
 */
public:
    virtual QMap<QString,QString> metadata(void) const = 0;
    virtual QString dataName(void) const = 0;

public:
    virtual void setImage(dtkImage* image) = 0;
    virtual dtkImage* image(void) = 0;

};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonAbstractBinaryImageData *)
DTK_DECLARE_PLUGIN        (gnomonAbstractBinaryImageData, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractBinaryImageData, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractBinaryImageData, GNOMONCORE_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractBinaryImageData, GNOMONCORE_EXPORT, binaryImageData);
}


