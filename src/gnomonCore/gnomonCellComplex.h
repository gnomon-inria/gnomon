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

#include "gnomonAbstractCellComplexData.h"
#include "gnomonAbstractForm.h"

#include <QtCore>

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonCellComplex : public gnomonAbstractForm
{
protected:
    gnomonAbstractCellComplexData *m_data;

public:
    explicit gnomonCellComplex(void) : m_data(nullptr) {}
    explicit gnomonCellComplex(gnomonAbstractCellComplexData *data) : m_data(data) {}
    gnomonCellComplex(const gnomonCellComplex& o) : m_data(o.m_data->clone()) {}

    gnomonAbstractForm *clone(void) { return new gnomonCellComplex(*this); };

    ~gnomonCellComplex(void) { if (m_data) { delete m_data; } m_data = nullptr; }

public:
    gnomonCellComplex& operator = (const gnomonCellComplex& o)
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
    const gnomonAbstractCellComplexData *data(void) const { return m_data; }
          gnomonAbstractCellComplexData *data(void)       { return m_data; }

    void setData(gnomonAbstractCellComplexData* data) { m_data = data; }

public:
    int dimension(void) const { return m_data->dimension(); }
    bool isValid(void) const { return m_data->isValid(); }

    bool hasElement(int dimension, long id) const { return m_data->hasElement(dimension, id); }

    QList<long> elementIds(int dimension) const { return m_data->elementIds(dimension); }
    long elementCount(int dimension) const { return m_data->elementCount(dimension); }

    QList<long> incidentElementIds(int dimension, long id, int incidenceDimension) const { return m_data->incidentElementIds(dimension, id, incidenceDimension); }
    int incidentElementCount(int dimension, long id, int incidenceDimension) const { return m_data->incidentElementCount(dimension, id, incidenceDimension); }

    QList<long> adjacentElementIds(int dimension, long id, int incidenceDimension) const { return m_data->adjacentElementIds(dimension, id, incidenceDimension); }
    int adjacentElementCount(int dimension, long id, int incidenceDimension) const { return m_data->adjacentElementCount(dimension, id, incidenceDimension); }

public:
    void setDimension(int dimension = 3) { m_data->setDimension(dimension); }

    long addElement(int dimension) { return m_data->addElement(dimension); }
    long addElement(int dimension, long id) { return m_data->addElement(dimension, id); }
    void removeElement(int dimension, long id) { m_data->removeElement(dimension, id); }

    void linkElements(int dimension, long id, long incidentId) { m_data->linkElements(dimension, id, incidentId); }
    void unlinkElements(int dimension, long id, long incidentId) { m_data->unlinkElements(dimension, id, incidentId); }

public:
    QStringList elementPropertyNames(int dimension) const { return m_data->elementPropertyNames(dimension); }
    bool hasElementProperty(int dimension, const QString& propertyName) const { return m_data->hasElementProperty(dimension, propertyName); }

    QMap<long, QVariant> elementProperty(int dimension, const QString& propertyName) const { return m_data->elementProperty(dimension, propertyName); }

public:
    void addElementProperty(int dimension, const QString& propertyName) { m_data->addElementProperty(dimension, propertyName); }
    void updateElementProperty(int dimension, const QString& propertyName, const QMap<long, QVariant>& values, bool eraseProperty = true) { m_data->updateElementProperty(dimension, propertyName, values, eraseProperty); }
    void removeElementProperty(int dimension, const QString& propertyName) { m_data->removeElementProperty(dimension, propertyName); }

public:
    QList<long> orientedFaceVertexIds(long faceId) const { return m_data->orientedFaceVertexIds(faceId); }
};

// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT(gnomonCellComplex *)

//
// gnomonCellComplex.h ends here
