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

class gnomonCellComplex;

class GNOMONCORE_EXPORT gnomonAbstractCellComplexReader
{
public:
    virtual ~gnomonAbstractCellComplexReader(void) {}

public:
    virtual gnomonCellComplex *read(const QString& path) = 0;

public:
    virtual QStringList types(void) = 0;
};

DTK_DECLARE_OBJECT(gnomonAbstractCellComplexReader *)

DTK_DECLARE_PLUGIN(gnomonAbstractCellComplexReader, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractCellComplexReader, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractCellComplexReader, GNOMONCORE_EXPORT)


namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractCellComplexReader, GNOMONCORE_EXPORT, cellComplexReader);
}

//
// gnomonAbstractCellComplexReader.h ends here