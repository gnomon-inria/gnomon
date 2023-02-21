#pragma once

#include <gnomonCoreExport.h>
#include "gnomonCore/gnomonCorePlugin.h"

#include "gnomonAbstractTreeData.h"
#include "gnomonForm/gnomonAbstractForm.h"
#include "gnomonForm/gnomonTimeSeries.h"

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
    explicit gnomonTree(QJsonObject& serialization) : m_data(nullptr) {
        static_cast<gnomonTree*>(this)->deserialize(serialization);
    }
    gnomonTree(const gnomonTree& o) : m_data(o.m_data->clone()) {}

    gnomonAbstractForm *clone(void) override { return new gnomonTree(*this); };

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
        m_data = gnomonCore::treeData::pluginFactory().create(serialization["pluginName"].toString());
        m_data->deserialize(serialization["data"].toString());
    }

public:
    static inline QString formName(void) { return "gnomonTree"; }

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


// ///////////////////////////////////////////////////////////////////

typedef gnomonTimeSeries<gnomonTree> gnomonTreeSeries;
Q_DECLARE_METATYPE(gnomonTreeSeries *)

//
// gnomonTree.h ends here
