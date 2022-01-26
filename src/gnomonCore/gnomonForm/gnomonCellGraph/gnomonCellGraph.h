#pragma once

#include <gnomonCoreExport.h>

#include "gnomonAbstractCellGraphData.h"
#include "gnomonForm/gnomonAbstractForm.h"
#include "gnomonForm/gnomonTimeSeries.h"

#include <QtCore>

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonCellGraph : public gnomonAbstractForm
{
protected:
    gnomonAbstractCellGraphData *m_data;

public:
    explicit gnomonCellGraph(void) : m_data(nullptr) {}
    explicit gnomonCellGraph(gnomonAbstractCellGraphData *data) : m_data(data) {}
    explicit gnomonCellGraph(QJsonObject& serialization) : m_data(nullptr) {
        static_cast<gnomonCellGraph*>(this)->deserialize(serialization);
    }
             gnomonCellGraph(const gnomonCellGraph& o) : m_data(o.m_data->clone()) {}

    gnomonAbstractForm *clone(void) { return new gnomonCellGraph(*this); };

    virtual ~gnomonCellGraph(void) { if (m_data) { delete m_data; } m_data = nullptr; }

public:
    gnomonCellGraph& operator = (const gnomonCellGraph& o)
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
    QString name(void) const override { return formName(); }
    QMap<QString,QString> metadata(void) const override { return m_data->metadata(); }
    QString dataName(void) const override { return m_data->dataName(); }
    const QString pluginName(void) override {
        return m_data->pluginName();
    }

    QJsonObject serialize(void) override {
        QJsonObject out;
        out["pluginName"] = pluginName();
        out["data"] = m_data->serialize();
        return out;
    }
    void deserialize(QJsonObject &serialization) override {
        delete m_data;
        m_data = gnomonCore::cellGraphData::pluginFactory().create(serialization["pluginName"].toString());
        m_data->deserialize(serialization["data"].toString());
    }

public:
    static inline QString formName(void) { return "gnomonCellGraph"; }

public:
    const gnomonAbstractCellGraphData *data(void) const { return m_data; }
          gnomonAbstractCellGraphData *data(void)       { return m_data; }

    void setData(gnomonAbstractCellGraphData* data) { m_data = data; }

public:
    virtual bool isValid(void) const { return m_data->isValid(); }

public:
    virtual QList<long> edgeVertexIds(long edgeId) const { return m_data->edgeVertexIds(edgeId); }
    virtual long verticesEdgeId(const QList<long>& vertexIds) const { return m_data->verticesEdgeId(vertexIds); }

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

    virtual long addEdge(const QList<long>& vertexIds) { return m_data->addEdge(vertexIds); }
    virtual long addEdge(const QList<long>& vertexIds, long edgeId) { return m_data->addEdge(vertexIds, edgeId); }
    virtual void removeEdge(long edgeId) { return m_data->removeEdge(edgeId); }

public:
    virtual QStringList vertexPropertyNames(void) const { return m_data->vertexPropertyNames(); }
    virtual bool hasVertexProperty(const QString& propertyName) const { return m_data->hasVertexProperty(propertyName); }
    virtual QMap<long, QVariant> vertexProperty(const QString& propertyName) const { return m_data->vertexProperty(propertyName); }
    virtual QMap<long, QVariant> addVertexProperty(const QString& propertyName) { return m_data->addVertexProperty(propertyName); }
    virtual void removeVertexProperty(const QString& propertyName) { return m_data->removeVertexProperty(propertyName); }

    virtual QStringList edgePropertyNames(void) const { return m_data->edgePropertyNames(); }
    virtual bool hasEdgeProperty(const QString& propertyName) const { return m_data->hasEdgeProperty(propertyName); }
    virtual QMap<long, QVariant> edgeProperty(const QString& propertyName) const { return m_data->edgeProperty(propertyName); }
    virtual QMap<long, QVariant> addEdgeProperty(const QString& propertyName) { return m_data->addEdgeProperty(propertyName); }
    virtual void removeEdgeProperty(const QString& propertyName) { return m_data->removeEdgeProperty(propertyName); }

    virtual QStringList graphPropertyNames(void) const { return m_data->graphPropertyNames(); }
    virtual bool hasGraphProperty(const QString& propertyName) const { return m_data->hasGraphProperty(propertyName); }
    virtual QVariant graphProperty(const QString& propertyName) const { return m_data->graphProperty(propertyName); }
    virtual QVariant addGraphProperty(const QString& propertyName) { return m_data->addGraphProperty(propertyName); }
    virtual void removeGraphProperty(const QString& propertyName) { return m_data->removeGraphProperty(propertyName); }

};


// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT(gnomonCellGraph *)


// ///////////////////////////////////////////////////////////////////

typedef gnomonTimeSeries<gnomonCellGraph> gnomonCellGraphSeries;
Q_DECLARE_METATYPE(gnomonCellGraphSeries *)

//
// gnomonCellGraph.h ends here
