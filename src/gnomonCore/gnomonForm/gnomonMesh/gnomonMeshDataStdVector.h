#pragma once

#include <gnomonCoreExport.h>

#include <QtCore>

#include "gnomonAbstractMeshData.h"

class GNOMONCORE_EXPORT gnomonMeshDataStdVector final: public gnomonAbstractMeshData
{
public:
    gnomonMeshDataStdVector(void);
    gnomonMeshDataStdVector(const gnomonMeshDataStdVector& mesh_data);
    ~gnomonMeshDataStdVector(void) final ;

public:
    gnomonAbstractMeshData* clone(void) const override;
    void fromGnomonForm(gnomonAbstractForm* other) override; //TODO

 public:
    QMap<QString,QString> metadata(void) const override;
    QString dataName(void) const override { return "gnomonMesh";};
    const QString pluginName(void) override { return "gnomonMeshDataStdVector";};

public:
    int geometricalDimension(void) const override;
    void setGeometricalDimension(int geo_dim) override;
    int topologicalDimension(void) const override;
    void setTopologicalDimension(int topo_dim) override;

public:
    void pointCoordinates(gnomonAbstractMeshData::IdxType id, double* point_coordinates) const override;
    const double* pointsCoordinates(void) const override;
    gnomonAbstractMeshData::CntType pointsCount(void) const override;

    void setPoint(gnomonAbstractMeshData::IdxType point_id, const double* points_coordinates) override;
    void setPointsCount(gnomonAbstractMeshData::CntType point_count) override;
    void setPoints(const double* points_coordinates, gnomonAbstractMeshData::CntType points_count = 0) override;

public:
    void cellPoints(gnomonAbstractMeshData::IdxType cell_id, int &out_nb_points_cell, const gnomonAbstractMeshData::IdxType*& out_first_point) const override; // out nb points of cell , out_first_point pointer to the first point of the cell
    QList<long> cellPointIdx(gnomonAbstractMeshData::IdxType cell_id) const override; // out nb points of cell
    gnomonAbstractMeshData::CellType cellType(gnomonAbstractMeshData::IdxType cell_id) const override;
    gnomonAbstractMeshData::CntType cellsCount(int geo_dimension = 4) const override;
    const gnomonAbstractMeshData::IdxType* cellsIdx(gnomonAbstractMeshData::CntType& out_nb_cells, int geo_dimension = 4) const override;
    const gnomonAbstractMeshData::IdxType* cellsPoints(gnomonAbstractMeshData::CntType& out_nb_cells, int geo_dimension = 4) const override;
    const gnomonAbstractMeshData::CellType* cellsType(int geo_dimension = 4) const override;
    const gnomonAbstractMeshData::IdxType* cellsTopologyLocation(int geo_dimension = 4) const override;
    unsigned int facet_corner_id(const gnomonAbstractMeshData::IdxType face_id, const gnomonAbstractMeshData::IdxType corner_id) const;

    void setCellsCount(gnomonAbstractMeshData::CntType cells_count, int geo_dimension) override;
    void setCellsType(const gnomonAbstractMeshData::CellType* cells_type, gnomonAbstractMeshData::CntType cells_count = 0, gnomonAbstractMeshData::CntType offset=0) override;
    void setCellsPoints(const gnomonAbstractMeshData::IdxType* cells_points, gnomonAbstractMeshData::CntType size_cells_points = 0) override;

public:
    void addAttribute(const gnomonMeshAttribute * attribute) override;
    const gnomonMeshAttribute* attribute(const QString& name) const override;
    const gnomonMeshAttribute* attributes(void) const override;
    gnomonAbstractMeshData::CntType attributesCount(void) const override;

private:
    int m_geo_dim;
    int m_topo_dim;

    gnomonAbstractMeshData::CntType m_nb_points;
    gnomonAbstractMeshData::CntType m_nb_cells[5]; // 5 : total size, {0,1,2,3} nb of cells od DIM {0,1,2,3}

    std::vector<double> m_points_coordinates; //3D, size = 3*nb_points
    std::vector<gnomonAbstractMeshData::CellType> m_cells_type;
    std::vector<gnomonAbstractMeshData::IdxType> m_cells_first_point_idx; //index of first point in m_cell_points per cell
    std::vector<gnomonAbstractMeshData::IdxType> m_cells_points;
    std::vector<gnomonMeshAttribute> m_attributes;
    QMap<QString,QString> m_metadatas;

};


// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

inline gnomonAbstractMeshData *gnomonMeshDataStdVectorCreator(void)
{
    return new gnomonMeshDataStdVector();
}
