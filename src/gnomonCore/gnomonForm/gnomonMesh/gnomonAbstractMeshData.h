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
#include "gnomonForm/gnomonAbstractFormData"

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonAbstractMeshData: public gnomonAbstractFormData
{
public:
             gnomonAbstractMeshData(void) = default;
    virtual ~gnomonAbstractMeshData(void) {};

    virtual gnomonAbstractMeshData* clone(void) const = 0;

    virtual void fromGnomonForm(gnomonAbstractForm* other) = 0;


//  ///////////////////////////////////////////////////////////////////
//  Metadata
//  ///////////////////////////////////////////////////////////////////
public:
    virtual QMap<QString,QString> metadata(void) const = 0;
    virtual QString dataName(void) const = 0;
    virtual const QString pluginName(void) = 0;

public:
    virtual bool isValid(void) const = 0;


//  ///////////////////////////////////////////////////////////////////
//  Vertex list concept
//  ///////////////////////////////////////////////////////////////////
public:
    virtual QList<long> vertexIds(void) const = 0;
    virtual long vertexCount(void) const = 0;


//  ///////////////////////////////////////////////////////////////////
//  Triangle list concept
//  ///////////////////////////////////////////////////////////////////
public:
    virtual QList<long> triangleIds(void) const = 0;
    virtual long triangleCount(void) const = 0;

    virtual QList<long> triangleVertexIds(long triangleId) const = 0;

//  ///////////////////////////////////////////////////////////////////
//  Mutation concept
//  ///////////////////////////////////////////////////////////////////
public:
    virtual long addVertex(void) = 0;
    virtual long addVertex(long vertexId) = 0;

    virtual void removeVertex(long vertexId) = 0;

    // virtual long addTriangle(long sourceId, long targetId) = 0;
    // virtual long addTriangle(long sourceId, long targetId, long triangleId) = 0;
    virtual long addTriangle(const QList<long>& vertexIds) = 0;
    virtual long addTriangle(const QList<long>& vertexIds, long triangleId) = 0;

    virtual void removeTriangle(long triangleId) = 0;

//  ///////////////////////////////////////////////////////////////////
//  Property concept
//  ///////////////////////////////////////////////////////////////////

    virtual QStringList vertexPropertyNames(void) const = 0;
    virtual bool hasVertexProperty(const QString& propertyName) const = 0;
    virtual QMap<long, QVariant> vertexProperty(const QString& propertyName) const = 0;
    virtual QMap<long, QVariant> addVertexProperty(const QString& propertyName) = 0;
    virtual void removeVertexProperty(const QString& propertyName) = 0;


    virtual QStringList trianglePropertyNames(void) const = 0;
    virtual bool hasTriangleProperty(const QString& propertyName) const = 0;
    virtual QMap<long, QVariant> triangleProperty(const QString& propertyName) const = 0;
    virtual QMap<long, QVariant> addTriangleProperty(const QString& OrientedPropertyName) = 0;
    virtual void removeTriangleProperty(const QString& OrientedPropertyName) = 0;
};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonAbstractMeshData *)
DTK_DECLARE_PLUGIN        (gnomonAbstractMeshData, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractMeshData, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractMeshData, GNOMONCORE_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractMeshData, GNOMONCORE_EXPORT, meshData);
}

//
// gnomonAbstractMeshData.h ends here
