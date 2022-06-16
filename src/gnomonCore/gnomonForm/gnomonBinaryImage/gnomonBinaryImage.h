#pragma once

#include "gnomonCoreExport.h"

#include "gnomonAbstractBinaryImageData.h"
#include "gnomonForm/gnomonAbstractForm.h"
#include "gnomonForm/gnomonTimeSeries.h"

#include <QtCore>

class dtkImage;

class GNOMONCORE_EXPORT gnomonBinaryImage : public gnomonAbstractForm {
protected:
    gnomonAbstractBinaryImageData* m_data;

public:
    explicit gnomonBinaryImage() : m_data(nullptr) {}
    explicit gnomonBinaryImage(gnomonAbstractBinaryImageData *data) : m_data(data) {}
    explicit gnomonBinaryImage(QJsonObject& serialization) : m_data(nullptr) {
        static_cast<gnomonBinaryImage*>(this)->deserialize(serialization);
    }
    gnomonBinaryImage(const gnomonBinaryImage& other) : m_data(other.m_data->clone()) {}

    gnomonAbstractForm* clone() override { return new gnomonBinaryImage(*this);}

    virtual ~gnomonBinaryImage() {
        delete m_data;
        m_data = nullptr;
    }

public:
    gnomonBinaryImage& operator=(const gnomonBinaryImage& other) {
        if (this == &other) {
            return *this;
        }
        if (m_data != other.m_data) {
            delete m_data;
            if (other.m_data != nullptr) {
                m_data = other.m_data->clone();
            } else {
                m_data = nullptr;
            }
        }
        return *this;
    }

public:
    const gnomonAbstractBinaryImageData* data() const {return m_data;}
    gnomonAbstractBinaryImageData* data() {return m_data;}
    void setData(gnomonAbstractBinaryImageData* data) {
        delete m_data;
        m_data = data;
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
        m_data = gnomonCore::binaryImageData::pluginFactory().create(serialization["pluginName"].toString());
        m_data->deserialize(serialization["data"].toString());
    }

public:
    static inline QString formName(void) { return "gnomonBinaryImage"; }

public:
    virtual void setImage(dtkImage* image) {return m_data->setImage(image);}
    virtual dtkImage* image() {return m_data->image();}
};

// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT(gnomonBinaryImage *)


// ///////////////////////////////////////////////////////////////////

typedef gnomonTimeSeries<gnomonBinaryImage> gnomonBinaryImageSeries;
Q_DECLARE_METATYPE(gnomonBinaryImageSeries *)
