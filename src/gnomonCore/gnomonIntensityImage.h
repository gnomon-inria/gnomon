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

#include "gnomonAbstractForm.h"

#include <QtCore>
#include <dtkImage>

class GNOMONCORE_EXPORT gnomonIntensityImage : public gnomonAbstractForm
{
protected:
    dtkImage *m_data;

public:
    explicit gnomonIntensityImage(void) : m_data(nullptr) {}
    explicit gnomonIntensityImage(dtkImage *data) : m_data(data) {}
    gnomonIntensityImage(const gnomonIntensityImage& o) : m_data(new dtkImage(*o.m_data)) {}

    gnomonAbstractForm *clone(void) { return new gnomonIntensityImage(*this); }

    ~gnomonIntensityImage(void) { if (m_data) { delete m_data; } m_data = nullptr; }

public:
    gnomonIntensityImage& operator = (const gnomonIntensityImage& o)
    {
        if (this == &o)
            return *this;

        if (m_data != o.m_data) {
            if (m_data != nullptr) {
                delete m_data;
            }
            if(o.m_data != nullptr) {
                m_data = new dtkImage(*o.m_data);
            } else {
                m_data = nullptr;
            }
        }
        return *this;
    }

public:
    QString name(void) const override { return"gnomonIntensityImage";}
    QMap<QString,QString> metadata(void) const override { return QMap<QString,QString>(); }

public:
    const dtkImage *data(void) const { return m_data; }
          dtkImage *data(void)       { return m_data; }

    void setData(dtkImage* data) { m_data = data; }
};

typedef QSharedPointer<gnomonIntensityImage> gnomonIntensityImagePtr;

// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT(gnomonIntensityImage *)

//
// gnomonIntensityImage.h ends here
