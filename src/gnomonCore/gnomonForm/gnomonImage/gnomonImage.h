#pragma once

#include <gnomonCoreExport>

#include "gnomonAbstractImageData.h"
#include "gnomonForm/gnomonAbstractForm.h"
#include "gnomonForm/gnomonTimeSeries.h"

#include <QtCore>

class dtkImage;

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonImage : public gnomonAbstractForm
{
protected:
    gnomonAbstractImageData *m_data;

public:
    explicit gnomonImage(void) : m_data(nullptr) {}
    explicit gnomonImage(gnomonAbstractImageData *data) : m_data(data) {}
    explicit gnomonImage(QJsonObject& serialization) : m_data(nullptr) {
        deserialize(serialization);
    }
             gnomonImage(const gnomonImage& o) : m_data(o.m_data->clone()) {}

    gnomonAbstractForm *clone(void) { qDebug()<<Q_FUNC_INFO<<this->data()<<this->data()->channels(); return new gnomonImage(*this); };

    ~gnomonImage(void) { if (m_data) { delete m_data; } m_data = nullptr; }

public:
    gnomonImage& operator = (const gnomonImage& o)
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
    const gnomonAbstractImageData *data(void) const { return m_data; }
          gnomonAbstractImageData *data(void)       { return m_data; }

    void setData(gnomonAbstractImageData* data)
    {
        if (m_data) {
            delete m_data;
        }
        m_data = data;
    }

public:
    QString name(void) const override { return"gnomonImage";}
    QMap<QString,QString> metadata(void) const override { return m_data->metadata(); }
    QString dataName(void) const override { return m_data->dataName(); }
    const QString pluginName(void) override {
        return m_data->pluginName();
    }

    QJsonObject serialize(void) final {
        QJsonObject out;
        out["pluginName"] = pluginName();
        out["data"] = m_data->serialize();
        return out;
    }
    void deserialize(QJsonObject &serialization) final {
        m_data = gnomonCore::imageData::pluginFactory().create(serialization["pluginName"].toString());
        m_data->deserialize(serialization["data"].toString());
    }

public:
    dtkImage *image(QString channel="") const { return m_data->image(channel); };
    void setImage(dtkImage *image, QString channel="") { return m_data->setImage(image,channel); };

public:
    void setChannelName(const QString& channel, const QString& name) { return m_data->setChannelName(channel,name); };

public:
    QStringList channels(void) const { return m_data->channels(); };
};


// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT(gnomonImage *)


// ///////////////////////////////////////////////////////////////////

typedef gnomonTimeSeries<gnomonImage> gnomonImageSeries;
Q_DECLARE_METATYPE(gnomonImageSeries *)

//
// gnomonImage.h ends here
