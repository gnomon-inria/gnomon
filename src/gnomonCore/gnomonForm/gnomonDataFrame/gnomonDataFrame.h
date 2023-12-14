#pragma once

#include <gnomonCoreExport.h>
#include "gnomonCore/gnomonCorePlugin.h"

#include "gnomonAbstractDataFrameData.h"
#include "gnomonForm/gnomonAbstractForm.h"
#include "gnomonForm/gnomonTimeSeries.h"

#include <QtCore>

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonDataFrame : public gnomonAbstractForm
{
protected:
    gnomonAbstractDataFrameData *m_data;

public:
    explicit gnomonDataFrame(void) : m_data(nullptr) {}
    explicit gnomonDataFrame(gnomonAbstractDataFrameData *data) : m_data(data) {}
    explicit gnomonDataFrame(QJsonObject& serialization) : m_data(nullptr) {
        static_cast<gnomonDataFrame*>(this)->deserialize(serialization);
    }
    gnomonDataFrame(const gnomonDataFrame& o) : m_data(o.m_data->clone()) {}

    gnomonAbstractForm *clone(void) override { return new gnomonDataFrame(*this); };

    ~gnomonDataFrame(void) { if (m_data) { delete m_data; } m_data = nullptr; }

public:
    gnomonDataFrame& operator = (const gnomonDataFrame& o)
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
        delete m_data;
        loadPluginGroup("dataFrameData");
        m_data = gnomonCore::dataFrameData::pluginFactory().create(serialization["pluginName"].toString());
        m_data->deserialize(serialization["data"].toString());
    }

public:
    static inline QString formName(void) { return "gnomonDataFrame"; }

public:
    const gnomonAbstractDataFrameData *data(void) const { return m_data; }
          gnomonAbstractDataFrameData *data(void)       { return m_data; }

    void setData(gnomonAbstractDataFrameData* data) { m_data = data; }

public:
    QList<long> index(void) const { return m_data->index(); }

    QStringList columnNames(void) const { return m_data->columnNames(); }
    QMap<long, QVariant>& column(QString columnName) const { return m_data->column(columnName); }
};

// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT(gnomonDataFrame *)


// ///////////////////////////////////////////////////////////////////

typedef gnomonTimeSeries<gnomonDataFrame> gnomonDataFrameSeries;
Q_DECLARE_METATYPE(gnomonDataFrameSeries *)

//
// gnomonDataFrame.h ends here
