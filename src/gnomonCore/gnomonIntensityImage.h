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

#include "gnomonAbstractIntensityImageData.h"
#include "gnomonAbstractForm.h"

#include <QtCore>

class GNOMONCORE_EXPORT gnomonIntensityImage : public gnomonAbstractForm
{
protected:
    gnomonAbstractIntensityImageData *m_data;

public:
    explicit gnomonIntensityImage(void) : m_data(nullptr) {}
    explicit gnomonIntensityImage(gnomonAbstractIntensityImageData *data) : m_data(data) {}
    gnomonIntensityImage(const gnomonIntensityImage& o) : m_data(o.m_data->clone()) {}

    ~gnomonIntensityImage(void) { if (m_data) { delete m_data; } m_data = nullptr; }

public:
    gnomonIntensityImage& operator = (const gnomonIntensityImage& o)
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
    const gnomonAbstractIntensityImageData *data(void) const { return m_data; }
          gnomonAbstractIntensityImageData *data(void)       { return m_data; }

    void setData(gnomonAbstractIntensityImageData* data) { m_data = data; }

public:
    virtual void setImage(dtkImage *image) { return m_data->setImage(image); }
    virtual dtkImage* image(void) const { return m_data->image(); }
};


// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT(gnomonIntensityImage *)

//
// gnomonIntensityImage.h ends here
