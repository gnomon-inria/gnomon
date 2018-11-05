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

#include <dtkCore>

class vtkImageData;

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonAbstractImageSeriesReader
{
public:
    virtual ~gnomonAbstractImageSeriesReader(void) = default;

public:
    virtual void setPath(const QString& path) = 0;

public:
    virtual vtkImageData *at(double t) = 0;
    virtual vtkImageData *next(void) = 0;
};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonAbstractImageSeriesReader *)
DTK_DECLARE_PLUGIN        (gnomonAbstractImageSeriesReader, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractImageSeriesReader, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractImageSeriesReader, GNOMONCORE_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractImageSeriesReader, GNOMONCORE_EXPORT, imageSeriesReader);
}

//
// gnomonAbstractImageReader.h ends here
