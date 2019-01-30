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

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonAbstractCellGraphData
{
public:
             gnomonAbstractCellGraphData(void) = default;
    virtual ~gnomonAbstractCellGraphData(void) {};

    virtual gnomonAbstractCellGraphData* clone(void) const = 0;

public:
    virtual bool isValid(void) const = 0;


//  ///////////////////////////////////////////////////////////////////
//  Metadata
//  ///////////////////////////////////////////////////////////////////
public:
    virtual QVariantMap metadata(void) const = 0;


//  ///////////////////////////////////////////////////////////////////
//  Graph concept
//  ///////////////////////////////////////////////////////////////////
public:
    // virtual long sourceVertexId(long edgeId) const = 0;
    // virtual long targetVertexId(long edgeId) const = 0;
    virtual QList<long> edgeVertexIds(long edgeId) const = 0;

    // virtual long verticesEdgeId(long sourceId, long targetId) const = 0;
    virtual long verticesEdgeId(const QList<long>& vertexIds) const = 0;

    virtual bool hasVertex(long vertexId) const = 0;
    virtual bool hasEdge(long edgeId) const = 0;


//  ///////////////////////////////////////////////////////////////////
//  Vertex list concept
//  ///////////////////////////////////////////////////////////////////
public:
    virtual QList<long> vertexIds(void) const = 0;
    virtual long vertexCount(void) const = 0;

    // virtual QList<long> incomingAdjacentVertexIds(long vertexId) const = 0;
    // virtual QList<long> outgoingAdjacentVertexIds(long vertexId) const = 0;
    virtual QList<long> adjacentVertexIds(long vertexId) const = 0;

    // virtual long incomingAdjacentVertexCount(long vertexId) const = 0;
    // virtual long outgoingAdjacentVertexCount(long vertexId) const = 0;
    virtual long adjacentVertexCount(long vertexId) const = 0;

//  ///////////////////////////////////////////////////////////////////
//  Edge list concept
//  ///////////////////////////////////////////////////////////////////
public:
    virtual QList<long> edgeIds(void) const = 0;
    virtual long edgeCount(void) const = 0;

    // virtual QList<long> incomingIncidentEdgeIds(long vertexId) const = 0;
    // virtual QList<long> outgoingIncidentEdgeIds(long vertexId) const = 0;
    virtual QList<long> incidentEdgeIds(long vertexId) const = 0;

    // virtual long incomingIncidentEdgeCount(long vertexId) const = 0;
    // virtual long outgoingIncidentEdgeCount(long vertexId) const = 0;
    virtual long incidentEdgeCount(long vertexId) const = 0;

//  ///////////////////////////////////////////////////////////////////
//  Mutation concept
//  ///////////////////////////////////////////////////////////////////
public:
    virtual long addVertex(void) = 0;
    virtual long addVertex(long vertexId) = 0;

    virtual void removeVertex(long vertexId) = 0;

    // virtual long addEdge(long sourceId, long targetId) = 0;
    // virtual long addEdge(long sourceId, long targetId, long edgeId) = 0;
    virtual long addEdge(const QList<long>& vertexIds) = 0;
    virtual long addEdge(const QList<long>& vertexIds, long edgeId) = 0;

    virtual void removeEdge(long edgeId) = 0;

//  ///////////////////////////////////////////////////////////////////
//  Property concept
//  ///////////////////////////////////////////////////////////////////

    virtual QStringList vertexPropertyNames(void) const = 0;
    virtual bool hasVertexProperty(const QString& propertyName) const = 0;
    virtual QMap<long, QVariant> vertexProperty(const QString& propertyName) const = 0;
    virtual QMap<long, QVariant> addVertexProperty(const QString& propertyName) = 0;
    virtual void removeVertexProperty(const QString& propertyName) = 0;

    virtual QStringList edgePropertyNames(void) const = 0;
    virtual bool hasEdgeProperty(const QString& propertyName) const = 0;
    virtual QMap<long, QVariant> edgeProperty(const QString& propertyName) const = 0;
    virtual QMap<long, QVariant> addEdgeProperty(const QString& OrientedPropertyName) = 0;
    virtual void removeEdgeProperty(const QString& OrientedPropertyName) = 0;

    //  ///////////////////////////////////////////////////////////////////
    //  Oriented properties (to implement in Python first)
    //  ///////////////////////////////////////////////////////////////////

    // virtual QMap<long, QVariant> addEdgeOrientedProperty(const QString& OrientedPropertyName) = 0;
    // virtual void removeEdgeOrientedProperty(const QString& OrientedPropertyName) = 0;

    // virtual QStringList edgeOrientedPropertyNames(void) const = 0;
    // virtual bool hasEdgeOrientedProperty(const QString& OrientedPropertyName) const = 0;

    // virtual QMap<long, QVariant> edgeOrientedProperty(const QString& OrientedPropertyName) const = 0;

    virtual QStringList graphPropertyNames(void) const = 0;
    virtual bool hasGraphProperty(const QString& propertyName) const = 0;
    virtual QVariant graphProperty(const QString& propertyName) const = 0;
    virtual QVariant addGraphProperty(const QString& propertyName) = 0;
    virtual void removeGraphProperty(const QString& propertyName) = 0;

};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonAbstractCellGraphData *)
DTK_DECLARE_PLUGIN        (gnomonAbstractCellGraphData, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractCellGraphData, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractCellGraphData, GNOMONCORE_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractCellGraphData, GNOMONCORE_EXPORT, cellGraphData);
}

//
// gnomonAbstractCellGraphData.h ends here
