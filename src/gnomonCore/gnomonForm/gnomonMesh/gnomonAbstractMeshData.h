#pragma once

#include <gnomonCoreExport.h>

#include <QtCore>

#include <dtkCore>
#include "gnomonCore/gnomonCorePlugin.h"

#include "gnomonForm/gnomonAbstractFormData"
#include "gnomonMeshAttribute.h"

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

static quint32 nb_points_per_cell_type[25] = {
        0, 1, 0, 2, 0,
        3, 0, 0, 4, 4,
        4, 8, 8, 6, 0,
        0, 0, 0, 0, 0,
        0, 4, 0, 0, 0};


class GNOMONCORE_EXPORT gnomonAbstractMeshData: public gnomonAbstractFormData
{
public:
    typedef long long int IdxType; // index type
    typedef long long int CntType; //counter type ( nb points, nb cells, ..)

    enum CellType : unsigned char {
        Vertex =  1,
        Line =  3,
        Quad = 9,
        QuadLine = 21,
        Triangle =  5,
        Pixel = 8,
        Tetra = 10,
        Voxel = 11,
        Hexahedron = 12,
        Wedge = 13,
        Uncknown = 99
    }; //type unsigned char



public:
             gnomonAbstractMeshData(void) = default;
    virtual ~gnomonAbstractMeshData(void) {};

    virtual gnomonAbstractMeshData* clone(void) const = 0;

    virtual void fromGnomonForm(gnomonAbstractForm* other) = 0;


//  ///////////////////////////////////////////////////////////////////
//  Metadata
//  ///////////////////////////////////////////////////////////////////
public:
    virtual QMap<QString,QString> metadata(void) const = 0;
    virtual QString dataName(void) const = 0;
    virtual const QString pluginName(void) = 0;

    //public:
    //virtual bool isValid(void) const = 0;


//  ///////////////////////////////////////////////////////////////////
//  Geometry related
//  ///////////////////////////////////////////////////////////////////
public:
    virtual int geometricalDimension(void) const = 0;
    virtual void setGeometricalDimension(int geo_dim) = 0;

    virtual int topologicalDimension(void) const  = 0;
    virtual void setTopologicalDimension(int topo_dim) = 0;

    //virtual unsigned int* lower_degree_ele_ids(unsigned int ele_dim, const IdxType ele_id) const = 0;

//  ///////////////////////////////////////////////////////////////////
//  Vertex list concept
//  ///////////////////////////////////////////////////////////////////
public:
    virtual void pointCoordinates(IdxType id, double* point_coordinates) const = 0;
    virtual const std::vector<double> pointCoordinates(IdxType id) const = 0;
    virtual const double* pointsCoordinates(void) const = 0;
    virtual CntType pointsCount(void) const = 0;

    virtual void setPoint(IdxType point_id, const double* points_coordinates) = 0;
    virtual void setPointsCount(CntType point_count) = 0;
    virtual void setPoints(const double* points_coordinates, CntType points_count = 0) = 0;

//  ///////////////////////////////////////////////////////////////////
//  Cells list concept
//  ///////////////////////////////////////////////////////////////////
public:
    virtual std::vector<IdxType> cellPointsIdx(IdxType cell_id) const = 0;

    virtual CellType cellType(IdxType cell_id) const = 0;
    virtual CntType cellsCount(int geo_dimension = 4) const = 0;

    virtual const std::vector<IdxType> cellsIdx(int geo_dimension = 4) const = 0;
    virtual const std::vector<IdxType> cellsPoints(int geo_dimension = 4) const = 0;

    virtual const CellType* cellsType(int geo_dimension = 4) const = 0;
    virtual const IdxType* cellsTopologyLocation(int geo_dimension = 4) const = 0;

    virtual void setCellsCount(CntType cells_count, int geo_dimension) = 0;
    virtual void setCellsType(const CellType* cells_type, CntType cells_count = 0, CntType offset=0 ) = 0;
    virtual void setCellsPoints(const IdxType* cells_points, CntType size_cells_point = 0) = 0;

//  ///////////////////////////////////////////////////////////////////
//  Property concept
//  ///////////////////////////////////////////////////////////////////
public:
    virtual void addAttribute(const gnomonMeshAttribute* attribute) = 0;
    virtual const gnomonMeshAttribute* attribute(const QString& name) const = 0;
    virtual const gnomonMeshAttribute* attributes(void) const = 0;
    virtual CntType attributesCount(void) const = 0;
    virtual QStringList attributesNames(void) const = 0;
};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonAbstractMeshData *)
DTK_DECLARE_PLUGIN        (gnomonAbstractMeshData, GNOMONCORE_EXPORT)
GNOMON_DECLARE_PLUGIN_FACTORY(gnomonAbstractMeshData, GNOMONCORE_EXPORT)
//DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractMeshData, GNOMONCORE_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

namespace gnomonCore {
    GNOMON_DECLARE_CONCEPT(gnomonAbstractMeshData, GNOMONCORE_EXPORT, meshData);
}

//
// gnomonAbstractMeshData.h ends here
