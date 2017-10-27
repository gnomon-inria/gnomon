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
    const tissueAbstractCellGraphData *data(void) const { return m_data; }
          tissueAbstractCellGraphData *data(void)       { return m_data; }

    void setData(tissueAbstractCellGraphData* data) { m_data = data; }

public:
    virtual bool isValid(void) const { return m_data->isValid(); }

public:
    virtual QList<long> edgeVertexIds(long edgeId) const { return m_data->edgeVertexIds(edgeId); }
    virtual long verticesEdgeId(QList<long> vertexIds) const { return m_data->verticesEdgeId(vertexIds); }

    virtual bool hasVertex(long vertexId) const  { return m_data->hasVertex(vertexId); }
    virtual bool hasEdge(long edgeId) const { return m_data->hasEdge(edgeId); }

public:
    virtual QList<long> vertexIds(void) const { return m_data->vertexIds(); }
    virtual long vertexCount(void) const { return m_data->vertexCount(); }
    
    virtual QList<long> adjacentVertexIds(long vertexId) const { return m_data->adjacentVertexIds(vertexId); }
    virtual long adjacentVertexCount(long vertexId) const { return m_data->adjacentVertexCount(vertexId); }

public:
    virtual QList<long> edgeIds(void) const { return m_data->edgeIds(); }
    virtual long edgeCount(void) const { return m_data->edgeCount(); }
    
    virtual QList<long> incidentEdgeIds(long vertexId) const { return m_data->incidentEdgeIds(vertexId); }
    virtual long incidentEdgeCount(long vertexId) const { return m_data->incidentEdgeCount(vertexId); }

public:
    virtual long addVertex(void) { return m_data->addVertex(); }
    virtual long addVertex(long vertexId) { return m_data->addVertex(vertexId); }
    virtual void removeVertex(long vertexId) { return m_data->removeVertex(vertexId); }

    virtual long addEdge(QList<long> vertexIds) { return m_data->addEdge(vertexIds); }
    virtual long addEdge(QList<long> vertexIds, long edgeId) { return m_data->addEdge(vertexIds, edgeId); }
    virtual void removeEdge(long edgeId) { return m_data->removeEdge(edgeId); }

public:
    virtual QList<QString> vertexPropertyNames(void) const { return m_data->vertexPropertyNames(); }
    virtual bool hasVertexProperty(const QString& propertyName) const { return m_data->hasVertexProperty(propertyName); }
    virtual const QMap<long, QVariant>& vertexProperty(const QString& propertyName) const { return m_data->vertexProperty(propertyName); }
    virtual       QMap<long, QVariant>& vertexProperty(const QString& propertyName) { return m_data->vertexProperty(propertyName); }
    virtual QMap<long, QVariant>& addVertexProperty(const QString& propertyName) { return m_data->addVertexProperty(propertyName); }
    virtual void removeVertexProperty(const QString& propertyName) { return m_data->removeVertexProperty(propertyName); }

    virtual QList<QString> edgePropertyNames(void) const { return m_data->edgePropertyNames(); }
    virtual bool hasEdgeProperty(const QString& propertyName) const { return m_data->hasEdgeProperty(propertyName); }
    virtual const QMap<long, QVariant>& edgeProperty(const QString& propertyName) const { return m_data->edgeProperty(propertyName); }
    virtual       QMap<long, QVariant>& edgeProperty(const QString& propertyName) { return m_data->edgeProperty(propertyName); }
    virtual QMap<long, QVariant>& addEdgeProperty(const QString& propertyName) { return m_data->addEdgeProperty(propertyName); }
    virtual void removeEdgeProperty(const QString& propertyName) { return m_data->removeEdgeProperty(propertyName); }

    virtual QList<QString> graphPropertyNames(void) const { return m_data->graphPropertyNames(); }
    virtual bool hasGraphProperty(const QString& propertyName) const { return m_data->hasGraphProperty(propertyName); }
    virtual const QVariant& graphProperty(const QString& propertyName) const { return m_data->graphProperty(propertyName); }
    virtual       QVariant& graphProperty(const QString& propertyName) { return m_data->graphProperty(propertyName); }
    virtual QVariant& addGraphProperty(const QString& propertyName) { return m_data->addGraphProperty(propertyName); }
    virtual void removeGraphProperty(const QString& propertyName) { return m_data->removeGraphProperty(propertyName); }

};


// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT(tissueCellGraph *)

//
// tissueCellGraph.h ends here