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

#include <tissueCoreExport.h>

#include "tissueAbstractCellImageData.h"

#include <QtCore>

class TISSUECORE_EXPORT tissueCellImage
{
protected:
    tissueAbstractCellImageData *m_data;

public:
    explicit tissueCellImage(void) : m_data(nullptr) {}
    explicit tissueCellImage(tissueAbstractCellImageData *data) : m_data(data) {}
    tissueCellImage(const tissueCellImage& o) : m_data(o.m_data->clone()) {}

    ~tissueCellImage(void) { if (m_data) { delete m_data; } m_data = nullptr; }

public:
    tissueCellImage& operator = (const tissueCellImage& o)
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
    const tissueAbstractCellImageData *data(void) const { return m_data; }
          tissueAbstractCellImageData *data(void)       { return m_data; }

    void setData(tissueAbstractCellImageData* data) { m_data = data; }

public:
    virtual void setImage(dtkImage *image) { return m_data->setImage(image); }
    virtual const dtkImage& image(void) const { return m_data->image(); }

    virtual QList<long> cellIds(void) const { return m_data->cellIds(); }
    virtual long cellCount(void) const { return m_data->cellCount(); }
    virtual QList<long> adjacentCellIds(long cellId) const { return m_data->adjacentCellIds(cellId); }
    virtual long adjacentCellCount(long cellId) const { return m_data->adjacentCellCount(cellId); }

    virtual QList<QString> cellPropertyNames(void) const { return m_data->cellPropertyNames(); };
    virtual bool hasCellProperty(const QString& propertyName) const { return m_data->hasCellProperty(propertyName); }

    virtual const QMap<long, QVariant>& cellProperty(const QString& propertyName) const { return m_data->cellProperty(propertyName); }
    virtual       QMap<long, QVariant>& cellProperty(const QString& propertyName) { return m_data->cellProperty(propertyName); }

    virtual void addCellProperty(const QString& propertyName) { return m_data->addCellProperty(propertyName); }
    virtual void updateCellProperty(const QString& propertyName, const QMap<long, QVariant>& values, bool eraseProperty = true) { return m_data->updateCellProperty(propertyName, values, eraseProperty); }
    virtual void removeCellProperty(const QString& propertyName) { return m_data->removeCellProperty(propertyName); }

    virtual QMap<long, QVariant>& computeCellProperty(const QString& propertyName) { return m_data->computeCellProperty(propertyName); };
};


// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT(tissueCellImage *)

//
// tissueCellImage.h ends here
