#pragma once

#include <gnomonCoreExport.h>

#include "gnomonAbstractPointCloudData.h"
#include "gnomonForm/gnomonAbstractForm.h"
#include "gnomonForm/gnomonTimeSeries.h"

#include <QtCore>

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonPointCloud : public gnomonAbstractForm
{
protected:
    gnomonAbstractPointCloudData *m_data;

public:
    explicit gnomonPointCloud(void) : m_data(nullptr) {}
    explicit gnomonPointCloud(gnomonAbstractPointCloudData *data) : m_data(data) {}
    explicit gnomonPointCloud(QJsonObject& serialization) : m_data(nullptr) {
        static_cast<gnomonPointCloud*>(this)->deserialize(serialization);
    }
             gnomonPointCloud(const gnomonPointCloud& o) : m_data(o.m_data->clone()) {}

    gnomonAbstractForm *clone(void) { return new gnomonPointCloud(*this); };

    ~gnomonPointCloud(void) { if (m_data) { delete m_data; } m_data = nullptr; }

public:
    gnomonPointCloud& operator = (const gnomonPointCloud& o)
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
        m_data = gnomonCore::pointCloudData::pluginFactory().create(serialization["pluginName"].toString());
        m_data->deserialize(serialization["data"].toString());
    }

public:
    static inline QString formName(void) { return "gnomonPointCloud"; }

public:
    const gnomonAbstractPointCloudData *data(void) const { return m_data; }
          gnomonAbstractPointCloudData *data(void)       { return m_data; }

    void setData(gnomonAbstractPointCloudData* data) { m_data = data; }

public:
    QList<long> pointIds(void) const { return m_data->pointIds(); }
    long pointCount(void) const { return m_data->pointCount(); }

public:
    void setPointPosition(long pointId, double x, double y, double z) { return m_data->setPointPosition(pointId,x,y,z); };
    long addPoint(double x, double y, double z) { return m_data->addPoint(x,y,z); };
    void removePoint(long pointId) { return m_data->removePoint(pointId); };

public:
    QStringList pointPropertyNames(void) const { return m_data->pointPropertyNames(); }
    bool hasPointProperty(const QString& propertyName) const { return m_data->hasPointProperty(propertyName); }
    QMap<long, QVariant> pointProperty(const QString& propertyName) const { return m_data->pointProperty(propertyName); }
    QMap<long, double> pointX(void) const { return m_data->pointX(); }
    QMap<long, double> pointY(void) const { return m_data->pointY(); }
    QMap<long, double> pointZ(void) const { return m_data->pointZ(); }

};

// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT(gnomonPointCloud *)


// ///////////////////////////////////////////////////////////////////

typedef gnomonTimeSeries<gnomonPointCloud> gnomonPointCloudSeries;
Q_DECLARE_METATYPE(gnomonPointCloudSeries *)

//
// gnomonPointCloud.h ends here
