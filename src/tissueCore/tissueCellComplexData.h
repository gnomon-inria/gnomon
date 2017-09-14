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

#include <QtCore>

#include <dtkCore>

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class TISSUECORE_EXPORT tissueCellComplexData
{
public:
             tissueCellComplexData(void) = default;
    virtual ~tissueCellComplexData(void) {};

public:
    virtual int dimension(void) const = 0;
    virtual bool isValid(void) const = 0;

//  ///////////////////////////////////////////////////////////////////
//  Element concept
//  ///////////////////////////////////////////////////////////////////

    virtual bool hasElement(int dimension, unsigned long id) const = 0;

    virtual std::vector<unsigned long> elementIds(int dimension) const = 0;
    virtual unsigned long elementCount(int dimension) const = 0;

//  ///////////////////////////////////////////////////////////////////
//  Incidence concept
//  ///////////////////////////////////////////////////////////////////

    virtual std::vector<unsigned long> incidentElementIds(int dimension, unsigned long id, int incidenceDimension) const = 0;
    virtual int incidentElementCount(int dimension, unsigned long id, int incidenceDimension) const = 0;

//  ///////////////////////////////////////////////////////////////////
//  Neighborhood concept
//  ///////////////////////////////////////////////////////////////////

    virtual std::vector<unsigned long> adjacentElementIds(int dimension, unsigned long id, int incidenceDimension) const = 0;
    virtual int adjacentElementCount(int dimension, unsigned long id, int incidenceDimension) const = 0;

//  ///////////////////////////////////////////////////////////////////
//  Mutation concept
//  ///////////////////////////////////////////////////////////////////

    virtual unsigned long addElement(int dimension, unsigned long *id = NULL) const = 0;
    virtual void removeElement(int dimension, unsigned long id) const = 0;

    virtual void linkElements(int dimension, unsigned long id, unsigned long incidentId) const = 0;
    virtual void unlinkElements(int dimension, unsigned long id, unsigned long incidentId) const = 0;

//  ///////////////////////////////////////////////////////////////////
//  Property concept
//  ///////////////////////////////////////////////////////////////////

    virtual std::vector<QString> elementPropertyNames(int dimension) const = 0;
    virtual bool hasElementProperty(int dimension, const QString& propertyName) const = 0;
    virtual QMap<unsigned long, QVariant>* elementProperty(int dimension, const QString& propertyName) const = 0;

    virtual void addElementProperty(int dimension, const QString& propertyName) const = 0;
    virtual void updateElementProperty(int dimension, const QString& propertyName, const QMap<unsigned long, QVariant>& values) const = 0;
    virtual void removeElementProperty(int dimension, const QString& propertyName) const = 0;
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
