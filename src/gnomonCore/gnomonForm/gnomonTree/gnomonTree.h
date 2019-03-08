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

#include "gnomonAbstractTreeData.h"
#include "gnomonForm/gnomonAbstractForm.h"

#include <QtCore>

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonTree : public gnomonAbstractForm
{
protected:
    gnomonAbstractTreeData *m_data;

public:
    explicit gnomonTree(void) : m_data(nullptr) {}
    explicit gnomonTree(gnomonAbstractTreeData *data) : m_data(data) {}
    gnomonTree(const gnomonTree& o) : m_data(o.m_data->clone()) {}

    gnomonAbstractForm *clone(void) { return new gnomonTree(*this); };

    ~gnomonTree(void) { if (m_data) { delete m_data; } m_data = nullptr; }

public:
    gnomonTree& operator = (const gnomonTree& o)
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
    QString name(void) const override { return"gnomonTree"; }
    QMap<QString,QString> metadata(void) const override { return m_data->metadata(); }

public:
    const gnomonAbstractTreeData *data(void) const { return m_data; }
          gnomonAbstractTreeData *data(void)       { return m_data; }

    void setData(gnomonAbstractTreeData* data) { m_data = data; }

public:
    QList<long> vertexIds(void) const { return m_data->vertexIds(); };
    long vertexCount(void) const { return m_data->vertexCount(); };

    long rootId(void) const { return m_data->rootId(); };

public:
    bool hasChildren(long vertexId) const { return m_data->hasChildren(vertexId); };
    QList<long> childrenIds(long vertexId) const { return m_data->childrenIds(vertexId); };
    long childrenCount(long vertexId) const { return m_data->childrenCount(vertexId); };

    bool hasParent(long vertexId) const { return m_data->hasParent(vertexId); };
    long parentId(long vertexId) const { return m_data->parentId(vertexId); };

    QStringList vertexPropertyNames(void) const { return m_data->vertexPropertyNames(); };
    bool hasVertexProperty(const QString& propertyName) const { return m_data->hasVertexProperty(propertyName); };
    QMap<long, QVariant> vertexProperty(const QString& propertyName) const { return m_data->vertexProperty(propertyName); };

    bool isIsomorphicTo(gnomonTree *other) const { return m_data->isIsomorphicTo(other); };
};

// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT(gnomonTree *)

//
// gnomonTree.h ends here
