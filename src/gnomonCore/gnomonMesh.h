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
#include "gnomonAbstractForm.h"

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

    virtual ~gnomonMesh(void) { if (m_data) { delete m_data; } m_data = nullptr; }

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
    QString name(void) const { return"gnomonMesh";}
    QVariantMap metadata(void) const override { return m_data->metadata(); }

public:
    const gnomonAbstractMeshData *data(void) const { return m_data; }
          gnomonAbstractMeshData *data(void)       { return m_data; }

    void setData(gnomonAbstractMeshData* data) { m_data = data; }

public:
    virtual QList<long> vertexIds(void) const { return m_data->vertexIds(); }
    virtual long vertexCount(void) const { return m_data->vertexCount(); }

public:
    virtual QList<long> triangleIds(void) const { return m_data->triangleIds(); }
    virtual long triangleCount(void) const { return m_data->triangleCount(); }

    virtual QList<long> triangleVertexIds(long triangleId) const { return m_data->triangleVertexIds(triangleId); }

public:
    virtual long addVertex(void) { return m_data->addVertex(); }
    virtual long addVertex(long vertexId) { return m_data->addVertex(vertexId); }
    virtual void removeVertex(long vertexId) { return m_data->removeVertex(vertexId); }

    virtual long addTriangle(const QList<long>& vertexIds) { return m_data->addTriangle(vertexIds); }
    virtual long addTriangle(const QList<long>& vertexIds, long triangleId) { return m_data->addTriangle(vertexIds, triangleId); }
    virtual void removeTriangle(long triangleId) { return m_data->removeTriangle(triangleId); }

public:
    virtual QStringList vertexPropertyNames(void) const { return m_data->vertexPropertyNames(); }
    virtual bool hasVertexProperty(const QString& propertyName) const { return m_data->hasVertexProperty(propertyName); }
    virtual QMap<long, QVariant> vertexProperty(const QString& propertyName) const { return m_data->vertexProperty(propertyName); }
    virtual QMap<long, QVariant> addVertexProperty(const QString& propertyName) { return m_data->addVertexProperty(propertyName); }
    virtual void removeVertexProperty(const QString& propertyName) { return m_data->removeVertexProperty(propertyName); }

    virtual QStringList trianglePropertyNames(void) const { return m_data->trianglePropertyNames(); }
    virtual bool hasTriangleProperty(const QString& propertyName) const { return m_data->hasTriangleProperty(propertyName); }
    virtual QMap<long, QVariant> triangleProperty(const QString& propertyName) const { return m_data->triangleProperty(propertyName); }
    virtual QMap<long, QVariant> addTriangleProperty(const QString& propertyName) { return m_data->addTriangleProperty(propertyName); }
    virtual void removeTriangleProperty(const QString& propertyName) { return m_data->removeTriangleProperty(propertyName); }
};

// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT(gnomonMesh *)

//
// gnomonMesh.h ends here
