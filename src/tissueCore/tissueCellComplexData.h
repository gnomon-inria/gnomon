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
    virtual void setDimension(int dimension = 3) = 0;
    virtual int dimension(void) const = 0;
    virtual bool isValid(void) const = 0;

//  ///////////////////////////////////////////////////////////////////
//  Element concept
//  ///////////////////////////////////////////////////////////////////

    virtual bool hasElement(int dimension, long id) const = 0;

    virtual QList<long> elementIds(int dimension) const = 0;
    virtual long elementCount(int dimension) const = 0;

//  ///////////////////////////////////////////////////////////////////
//  Incidence concept
//  ///////////////////////////////////////////////////////////////////

    virtual QList<long> incidentElementIds(int dimension, long id, int incidenceDimension) const = 0;
    virtual int incidentElementCount(int dimension, long id, int incidenceDimension) const = 0;

//  ///////////////////////////////////////////////////////////////////
//  Neighborhood concept
//  ///////////////////////////////////////////////////////////////////

    virtual QList<long> adjacentElementIds(int dimension, long id, int incidenceDimension) const = 0;
    virtual int adjacentElementCount(int dimension, long id, int incidenceDimension) const = 0;

//  ///////////////////////////////////////////////////////////////////
//  Mutation concept
//  ///////////////////////////////////////////////////////////////////

    virtual long addElement(int dimension) = 0;
    virtual long addElement(int dimension, long id) = 0;
    virtual void removeElement(int dimension, long id) = 0;

    virtual void linkElements(int dimension, long id, long incidentId) = 0;
    virtual void unlinkElements(int dimension, long id, long incidentId) = 0;

//  ///////////////////////////////////////////////////////////////////
//  Property concept
//  ///////////////////////////////////////////////////////////////////

    virtual QList<QString> elementPropertyNames(int dimension) const = 0;
    virtual bool hasElementProperty(int dimension, const QString& propertyName) const = 0;

    virtual const QMap<long, QVariant>& elementProperty(int dimension, const QString& propertyName) const = 0;
    virtual       QMap<long, QVariant>& elementProperty(int dimension, const QString& propertyName) = 0;

    virtual void addElementProperty(int dimension, const QString& propertyName) = 0;
    virtual void updateElementProperty(int dimension, const QString& propertyName, const QMap<long, QVariant>& values, bool eraseProperty = true) = 0;
    virtual void removeElementProperty(int dimension, const QString& propertyName) = 0;
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
