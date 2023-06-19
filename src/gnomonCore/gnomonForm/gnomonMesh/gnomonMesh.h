#pragma once

#include <gnomonCoreExport.h>
#include "gnomonCore/gnomonCorePlugin.h"

#include "gnomonAbstractMeshData.h"
#include "gnomonForm/gnomonAbstractForm.h"
#include "gnomonForm/gnomonTimeSeries.h"

#include <QtCore>

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonMesh : public gnomonAbstractForm
{
protected:
    gnomonAbstractMeshData *m_data;

public:
    explicit gnomonMesh(void) : m_data(nullptr) {}
    explicit gnomonMesh(gnomonAbstractMeshData *data) : m_data(data) {}
    explicit gnomonMesh(QJsonObject& serialization) : m_data(nullptr) {
        static_cast<gnomonMesh*>(this)->deserialize(serialization);
    }
    gnomonMesh(const gnomonMesh& o) : m_data(o.m_data->clone()) {}

    gnomonAbstractForm *clone(void) override { return new gnomonMesh(*this); };

    ~gnomonMesh(void) { if (m_data) { delete m_data; } m_data = nullptr; }

public:
    gnomonMesh& operator = (const gnomonMesh& o)
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
        m_data = gnomonCore::meshData::pluginFactory().create(serialization["pluginName"].toString());
        m_data->deserialize(serialization["data"].toString());
    }


public:
    int geometricalDimension(void) const {return m_data->geometricalDimension();};
    int topologicalDimension(void) const {return m_data->topologicalDimension();};

    void pointCoordinates(gnomonAbstractMeshData::IdxType id, double* point_coordinates) const {
        return m_data->pointCoordinates(id, point_coordinates); };
    const double* pointsCoordinates(void) const {
        return m_data->pointsCoordinates();};
    gnomonAbstractMeshData::CntType pointsCount(void) const {
        return m_data->pointsCount();};
    void  cellPoints(gnomonAbstractMeshData::IdxType cell_id, int &out_nb_points_cell, const gnomonAbstractMeshData::IdxType*& out_first_point) const {
        return m_data->cellPoints(cell_id, out_nb_points_cell, out_first_point);}; // out nb points of cell , out_first_point pointer to the first point of the cell
    gnomonAbstractMeshData::CellType cellType(gnomonAbstractMeshData::IdxType cell_id) const {return m_data->cellType(cell_id);};
    gnomonAbstractMeshData::CntType cellsCount(int geo_dimension = 4) const {return m_data->cellsCount(geo_dimension);};
    const gnomonAbstractMeshData::IdxType* cellsIdx(gnomonAbstractMeshData::CntType& out_nb_cells, int geo_dimension = 4) const {return m_data->cellsIdx(out_nb_cells, geo_dimension);};
    const gnomonAbstractMeshData::IdxType* cellsPoints(gnomonAbstractMeshData::CntType& out_nb_points, int geo_dimension = 4) const {return m_data->cellsPoints(out_nb_points, geo_dimension);};

    const gnomonAbstractMeshData::CellType* cellsType(int geo_dimension = 4) const {return m_data->cellsType(geo_dimension);};
    const gnomonAbstractMeshData::IdxType* cellsTopologyLocation(int geo_dimension = 4) const {return m_data->cellsTopologyLocation(geo_dimension);};

    const gnomonMeshAttribute* attribute(const QString& name) const {return m_data->attribute(name);};
    const gnomonMeshAttribute* attributes(void) const {return m_data->attributes();};
    gnomonAbstractMeshData::CntType attributesCount(void) const {return m_data->attributesCount();};

public:
    static inline QString formName(void) { return "gnomonMesh"; }

public:
    const gnomonAbstractMeshData *data(void) const { return m_data; }
          gnomonAbstractMeshData *data(void)       { return m_data; }

    void setData(gnomonAbstractMeshData* data) { m_data = data; }
};


// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT(gnomonMesh *)


// ///////////////////////////////////////////////////////////////////

typedef gnomonTimeSeries<gnomonMesh> gnomonMeshSeries;
Q_DECLARE_METATYPE(gnomonMeshSeries *)

//
// gnomonMesh.h ends here
