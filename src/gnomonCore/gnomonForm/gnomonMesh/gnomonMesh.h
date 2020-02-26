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

#include "gnomonAbstractMeshData.h"
#include "gnomonForm/gnomonAbstractForm.h"
#include "gnomonForm/gnomonTimeSeries.h"

#include <QtCore>

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonMesh : public gnomonAbstractForm
{
protected:
    gnomonAbstractMeshData *m_data;

public:
    explicit gnomonMesh(void) : m_data(nullptr) {}
    explicit gnomonMesh(gnomonAbstractMeshData *data) : m_data(data) {}
             gnomonMesh(const gnomonMesh& o) : m_data(o.m_data->clone()) {}

    gnomonAbstractForm *clone(void) { return new gnomonMesh(*this); };

    ~gnomonMesh(void) { if (m_data) { delete m_data; } m_data = nullptr; }

public:
    gnomonMesh& operator = (const gnomonMesh& o)
    {
        if (this == &o)
            return *this;

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
    QString name(void) const override { return"gnomonMesh";}
    QMap<QString,QString> metadata(void) const override { return m_data->metadata(); }
    QString dataName(void) const override { return m_data->dataName(); }

public:
    const gnomonAbstractMeshData *data(void) const { return m_data; }
          gnomonAbstractMeshData *data(void)       { return m_data; }

    void setData(gnomonAbstractMeshData* data) { m_data = data; }

public:
    QList<long> vertexIds(void) const { return m_data->vertexIds(); }
    long vertexCount(void) const { return m_data->vertexCount(); }

public:
    QList<long> triangleIds(void) const { return m_data->triangleIds(); }
    long triangleCount(void) const { return m_data->triangleCount(); }

    QList<long> triangleVertexIds(long triangleId) const { return m_data->triangleVertexIds(triangleId); }

public:
    long addVertex(void) { return m_data->addVertex(); }
    long addVertex(long vertexId) { return m_data->addVertex(vertexId); }
    void removeVertex(long vertexId) { return m_data->removeVertex(vertexId); }

    long addTriangle(const QList<long>& vertexIds) { return m_data->addTriangle(vertexIds); }
    long addTriangle(const QList<long>& vertexIds, long triangleId) { return m_data->addTriangle(vertexIds, triangleId); }
    void removeTriangle(long triangleId) { return m_data->removeTriangle(triangleId); }

public:
    QStringList vertexPropertyNames(void) const { return m_data->vertexPropertyNames(); }
    bool hasVertexProperty(const QString& propertyName) const { return m_data->hasVertexProperty(propertyName); }
    QMap<long, QVariant> vertexProperty(const QString& propertyName) const { return m_data->vertexProperty(propertyName); }
    QMap<long, QVariant> addVertexProperty(const QString& propertyName) { return m_data->addVertexProperty(propertyName); }
    void removeVertexProperty(const QString& propertyName) { return m_data->removeVertexProperty(propertyName); }

    QStringList trianglePropertyNames(void) const { return m_data->trianglePropertyNames(); }
    bool hasTriangleProperty(const QString& propertyName) const { return m_data->hasTriangleProperty(propertyName); }
    QMap<long, QVariant> triangleProperty(const QString& propertyName) const { return m_data->triangleProperty(propertyName); }
    QMap<long, QVariant> addTriangleProperty(const QString& propertyName) { return m_data->addTriangleProperty(propertyName); }
    void removeTriangleProperty(const QString& propertyName) { return m_data->removeTriangleProperty(propertyName); }
};

// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT(gnomonMesh *)


// ///////////////////////////////////////////////////////////////////

typedef gnomonTimeSeries<gnomonMesh> gnomonMeshSeries;
Q_DECLARE_METATYPE(gnomonMeshSeries *)

//
// gnomonMesh.h ends here
