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

#include "tissueAbstractCellGraphData.h"

#include <QtCore>

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class TISSUECORE_EXPORT tissueCellGraph
{
protected:
    tissueAbstractCellGraphData *m_data;

public:
    explicit tissueCellGraph(void) : m_data(nullptr) {}
    explicit tissueCellGraph(tissueAbstractCellGraphData *data) : m_data(data) {}
    tissueCellGraph(const tissueCellGraph& o) : m_data(o.m_data->clone()) {}

    ~tissueCellGraph(void) { if (m_data) { delete m_data; } m_data = nullptr; }

public:
    tissueCellGraph& operator = (const tissueCellGraph& o)
    {
        if (m_data != o.m_data) {
            if (m_data != nullptr) {
                delete m_data;
            }
            if(o.m_data != nullptr) {
                m_data = o.m_data->clone();
            } else {
                m_data = nullptr;
            }
        }
        return *this;
    }

public:
    const tissueCellGraphData *data(void) const { return m_data; }
          tissueCellGraphData *data(void)       { return m_data; }

    void setData(tissueAbstractCellGraphData* data) { m_data = data; }

public:
    virtual bool isValid(void) const { return m_data->isValid(); }

public:
    virtual QList<long> edgeVertexIds(long edgeId) const { return m_data->edgeVertexIds(edgeId); }
    virtual long verticesEdgeId(QList<long> vertexIds) const { return m_data->verticesEdgeId(vertexIds); }

    virtual bool hasVertex(long vertexId) const  { return m_data->hasVertex(vertexId); }
    virtual bool hasEdge(long edgeId) const { return m_data->hasEdge(edgeId); }

public:
    virtual QList<long> vertexIds(void) const { m_data->vertexIds(); }
    virtual long vertexCount(void) const { m_data->vertexCount(); }
    
    virtual QList<long> adjacentVertexIds(long vertexId) const { m_data->adjacentVertexIds(vertexId); }
    virtual long adjacentVertexCount(long vertexId) const { m_data->adjacentVertexCount(vertexId); }

public:
    virtual QList<long> edgeIds(void) const { m_data->edgeIds(); }
    virtual long edgeCount(void) const { m_data->edgeCount(); }
    
    virtual QList<long> adjacentEdgeIds(long edgeId) const { m_data->adjacentedgeIds(edgeId); }
    virtual long adjacentEdgeCount(long edgeId) const { m_data->adjacentedgeCount(edgeId); }

public:
    virtual long addVertex(void) { m_data->addVertex(); }
    virtual long addVertex(long vertexId) { m_data->addVertex(vertexId); }
    virtual void removeVertex(long vertexId) { m_data->removeVertex(vertexId); }

    virtual long addEdge(QList<long> vertexIds) { m_data->addEdge(vertexIds); }
    virtual long addEdge(QList<long> vertexIds, long edgeId) { m_data->addEdge(vertexIds, edgeId); }
    virtual void removeEdge(long edgeId) {m_data->removeEdge(edgeId); }

public:
    virtual QList<QString> vertexPropertyNames(void) const { m_data->vertexPropertyNames(); }
    virtual bool hasVertexProperty(const QString& propertyName) const { m_data->hasVertexProperty(propertyName); }
    virtual const QMap<long, QVariant>& vertexProperty(const QString& propertyName) const { m_data->vertexProperty(propertyName); }
    virtual       QMap<long, QVariant>& vertexProperty(const QString& propertyName) { m_data->vertexProperty(propertyName); }
    virtual QMap<long, QVariant>& addVertexProperty(const QString& propertyName) { m_data->addVertexProperty(propertyName); }
    virtual void removeVertexProperty(const QString& propertyName) { m_data->removeVertexProperty(propertyName); }

    virtual QList<QString> edgePropertyNames(void) const { m_data->edgePropertyNames(); }
    virtual bool hasEdgeProperty(const QString& propertyName) const { m_data->hasEdgeProperty(propertyName); }
    virtual const QMap<long, QVariant>& edgeProperty(const QString& propertyName) const { m_data->edgeProperty(propertyName); }
    virtual       QMap<long, QVariant>& edgeProperty(const QString& propertyName) { m_data->edgeProperty(propertyName); }
    virtual QMap<long, QVariant>& addEdgeProperty(const QString& propertyName) { m_data->addEdgeProperty(propertyName); }
    virtual void removeEdgeProperty(const QString& propertyName) { m_data->removeEdgeProperty(propertyName); }

    virtual QList<QString> graphPropertyNames(void) const { m_data->graphPropertyNames(); }
    virtual bool hasGraphProperty(const QString& propertyName) const { m_data->hasGraphProperty(propertyName); }
    virtual const QVariant& graphProperty(const QString& propertyName) const { m_data->graphProperty(propertyName); }
    virtual       QVariant& graphProperty(const QString& propertyName) { m_data->graphProperty(propertyName); }
    virtual QVariant& addGraphProperty(const QString& propertyName) { m_data->addGraphProperty(propertyName); }
    virtual void removeGraphProperty(const QString& propertyName) { m_data->removeGraphProperty(propertyName); }

};


// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT(tissueCellGraph *)

//
// tissueCellGraph.h ends here