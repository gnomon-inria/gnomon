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

#include <tissueCoreExport.h>

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class TISSUECORE_EXPORT tissueCellComplexData
{

public:
             tissueCellComplexData(void) {};
    virtual ~tissueCellComplexData(void) {};

public:
    virtual int dimension(void) const = 0;
    virtual bool isValid(void) const = 0;

//  ///////////////////////////////////////////////////////////////////
//  Element concept
//  ///////////////////////////////////////////////////////////////////

    virtual bool hasElement(int dimension, unsigned long id) const = 0;

    virtual unsigned long *elementIds(int dimension) const = 0;
    virtual unsigned long elementCount(int dimension) const = 0;

//  ///////////////////////////////////////////////////////////////////
//  Incidence concept
//  ///////////////////////////////////////////////////////////////////

    virtual unsigned long *borders(int dimension, unsigned long id) const = 0;
    virtual int borderCount(int dimension, unsigned long id) const = 0;
    virtual unsigned long *regions(int dimension, unsigned long id) const = 0;
    virtual int regionCount(int dimension, unsigned long id) const = 0;

// OR

    virtual unsigned long *incidentElementIds(int dimension, unsigned long id, int incidenceDimension) const = 0;
    virtual int incidentElementCount(int dimension, unsigned long id, int incidenceDimension) const = 0;

//  ///////////////////////////////////////////////////////////////////
//  Neighborhood concept
//  ///////////////////////////////////////////////////////////////////

};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (tissueCellComplexData *)
DTK_DECLARE_PLUGIN        (tissueCellComplexData, TISSUECORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(tissueCellComplexData, TISSUECORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(tissueCellComplexData, TISSUECORE_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to tissueCore layer
// /////////////////////////////////////////////////////////////////

namespace tissueCore {
    DTK_DECLARE_CONCEPT(tissueCellComplexData, TISSUECORE_EXPORT, cellcomplexdata);
}

//
// tissueCellComplexData.h ends here
