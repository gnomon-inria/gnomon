#pragma once

#include <gnomonCoreExport.h>
#include "gnomonCore/gnomonCorePlugin.h"

#include "gnomonAbstractDataDictData.h"
#include "gnomonForm/gnomonAbstractForm.h"
#include "gnomonForm/gnomonTimeSeries.h"

#include <QtCore>

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonDataDict : public gnomonAbstractForm
{
protected:
    gnomonAbstractDataDictData *m_data;

public:
    explicit gnomonDataDict(void) : m_data(nullptr) {}
    explicit gnomonDataDict(gnomonAbstractDataDictData *data) : m_data(data) {}
    explicit gnomonDataDict(QJsonObject& serialization) : m_data(nullptr) {
        static_cast<gnomonDataDict*>(this)->deserialize(serialization);
    }
    gnomonDataDict(const gnomonDataDict& o) : m_data(o.m_data->clone()) {}

    gnomonAbstractForm *clone(void) override { return new gnomonDataDict(*this); };

    ~gnomonDataDict(void) { if (m_data) { delete m_data; } m_data = nullptr; }

public:
    gnomonDataDict& operator = (const gnomonDataDict& o)
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
        m_data = gnomonCore::dataDictData::pluginFactory().create(serialization["pluginName"].toString());
        m_data->deserialize(serialization["data"].toString());
    }

public:
    static inline QString formName(void) { return "gnomonDataDict"; }

public:
    const gnomonAbstractDataDictData *data(void) const { return m_data; }
          gnomonAbstractDataDictData *data(void)       { return m_data; }

    void setData(gnomonAbstractDataDictData* data) { m_data = data; }

public:
    QStringList keys() const { return m_data->keys(); }
    //QVariantMap get() { return m_data->get(); }
    QVariant get(const QString& key) { return m_data->get(key); }

};

// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT(gnomonDataDict *)


// ///////////////////////////////////////////////////////////////////

typedef gnomonTimeSeries<gnomonDataDict> gnomonDataDictSeries;
Q_DECLARE_METATYPE(gnomonDataDictSeries *)

//
// gnomonDataDict.h ends here
