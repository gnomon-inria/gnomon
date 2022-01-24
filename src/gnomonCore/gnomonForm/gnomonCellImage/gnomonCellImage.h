#pragma once

#include <gnomonCoreExport.h>

#include "gnomonAbstractCellImageData.h"
#include "gnomonForm/gnomonAbstractForm.h"
#include "gnomonForm/gnomonTimeSeries.h"

#include <QtCore>

class GNOMONCORE_EXPORT gnomonCellImage : public gnomonAbstractForm
{
protected:
    gnomonAbstractCellImageData *m_data;

public:
    explicit gnomonCellImage(void) : m_data(nullptr) {}
    explicit gnomonCellImage(gnomonAbstractCellImageData *data) : m_data(data) {}
    explicit gnomonCellImage(QJsonObject& serialization) : m_data(nullptr) {
        deserialize(serialization);
    }
    gnomonCellImage(const gnomonCellImage& o) : m_data(o.m_data->clone()) {}

    gnomonAbstractForm *clone(void) { return new gnomonCellImage(*this); };

    ~gnomonCellImage(void) { if (m_data) { delete m_data; } m_data = nullptr; }


public:
    gnomonCellImage& operator = (const gnomonCellImage& o)
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
    const gnomonAbstractCellImageData *data(void) const { return m_data; }
          gnomonAbstractCellImageData *data(void)       { return m_data; }

    void setData(gnomonAbstractCellImageData* data)
    {
        if (m_data) {
            delete m_data;
        }
        m_data = data;
    }

public:
    QString name(void) const override { return formName(); }
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
        m_data = gnomonCore::cellImageData::pluginFactory().create(serialization["pluginName"].toString());
        m_data->deserialize(serialization["data"].toString());
    }

public:
    static inline QString formName(void) { return "gnomonCellImage"; }

public:
    virtual void setImage(dtkImage *image) { return m_data->setImage(image); }
    virtual dtkImage* image(void) const { return m_data->image(); }

    virtual QList<long> cellIds(void) const { return m_data->cellIds(); }
    virtual long cellCount(void) const { return m_data->cellCount(); }
    virtual QList<long> adjacentCellIds(long cellId) const { return m_data->adjacentCellIds(cellId); }
    virtual long adjacentCellCount(long cellId) const { return m_data->adjacentCellCount(cellId); }

    virtual QStringList cellPropertyNames(void) const { return m_data->cellPropertyNames(); };
    virtual bool hasCellProperty(const QString& propertyName) const { return m_data->hasCellProperty(propertyName); }

    virtual QMap<long, QVariant> cellProperty(const QString& propertyName) const { return m_data->cellProperty(propertyName); }

    virtual void addCellProperty(const QString& propertyName) { return m_data->addCellProperty(propertyName); }
    virtual void updateCellProperty(const QString& propertyName, const QMap<long, QVariant>& values, bool eraseProperty = true) { return m_data->updateCellProperty(propertyName, values, eraseProperty); }
    virtual void removeCellProperty(const QString& propertyName) { return m_data->removeCellProperty(propertyName); }

    virtual QMap<long, QVariant> computeCellProperty(const QString& propertyName) { return m_data->computeCellProperty(propertyName); };
};

// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT(gnomonCellImage *)


// ///////////////////////////////////////////////////////////////////

typedef gnomonTimeSeries<gnomonCellImage> gnomonCellImageSeries;
Q_DECLARE_METATYPE(gnomonCellImageSeries *)


//
// gnomonCellImage.h ends here
