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

#include "gnomonAbstractMultiChannelImageData.h"
#include "gnomonAbstractForm.h"

#include <QtCore>

class vtkImageData;

class GNOMONCORE_EXPORT gnomonMultiChannelImage : public gnomonAbstractForm
{
protected:
    gnomonAbstractMultiChannelImageData *m_data;

public:
    explicit gnomonMultiChannelImage(void) : m_data(nullptr) {}
    explicit gnomonMultiChannelImage(gnomonAbstractMultiChannelImageData *data) : m_data(data) {}
    gnomonMultiChannelImage(const gnomonMultiChannelImage& o) : m_data(o.m_data->clone()) {}

    gnomonAbstractForm *clone(void) { return new gnomonMultiChannelImage(*this); }

    ~gnomonMultiChannelImage(void) { if (m_data) { delete m_data; } m_data = nullptr; }

public:
    gnomonMultiChannelImage& operator = (const gnomonMultiChannelImage& o)
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
    const gnomonAbstractMultiChannelImageData *data(void) const { return m_data; }
          gnomonAbstractMultiChannelImageData *data(void)       { return m_data; }

    void setData(gnomonAbstractMultiChannelImageData* data) { m_data = data; }


public:
    virtual int channelCount(void) { return m_data->channelCount(); }
    virtual QList<QString> channelNames(void) { return m_data->channelNames(); }
    
    virtual void addChannelImage(const QString& channelName, vtkImageData *image) { return m_data->addChannelImage(channelName, image); }
    virtual vtkImageData* channelImage(const QString& channelName) const { return m_data->channelImage(channelName); }
};

// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT(gnomonMultiChannelImage *)

//
// gnomonMultiChannelImage.h ends here
