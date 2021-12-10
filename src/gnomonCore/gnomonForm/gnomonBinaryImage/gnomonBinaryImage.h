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
    gnomonBinaryImage(const gnomonBinaryImage& other) : m_data(other.m_data->clone()) {}

    gnomonAbstractForm* clone() { return new gnomonBinaryImage(*this);}

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
    QString name(void) const override { return"gnomonBinaryImage";}
    QMap<QString,QString> metadata(void) const override { return m_data->metadata(); }
    QString dataName(void) const override { return m_data->dataName(); }

public:
    virtual void setImage(dtkImage* image) {return m_data->setImage(image);}
    virtual dtkImage* image() {return m_data->image();}
};

// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT(gnomonBinaryImage *)


// ///////////////////////////////////////////////////////////////////

typedef gnomonTimeSeries<gnomonBinaryImage> gnomonBinaryImageSeries;
Q_DECLARE_METATYPE(gnomonBinaryImageSeries *)
