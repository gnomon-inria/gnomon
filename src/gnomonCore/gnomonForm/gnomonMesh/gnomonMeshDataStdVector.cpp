#include "gnomonMesh.h"

#include "gnomonMeshDataStdVector.h"


gnomonMeshDataStdVector::gnomonMeshDataStdVector(void) {
    m_geo_dim=-1;//equivalent to unset
    m_topo_dim=-1;//equivalent to unset
    m_nb_points=0;
    m_nb_cells[0]=m_nb_cells[1]=m_nb_cells[2]=m_nb_cells[3]=0;
    //m_attributes.reserve(5);
}

gnomonMeshDataStdVector::gnomonMeshDataStdVector(const gnomonMeshDataStdVector& other) {
    m_nb_cells[0] = other.m_nb_cells[0];
     m_nb_cells[1] = other.m_nb_cells[1];
     m_nb_cells[2] = other.m_nb_cells[2];
     m_nb_cells[3] = other.m_nb_cells[3];
     m_nb_cells[4] = other.m_nb_cells[4];

     m_geo_dim = other.m_geo_dim;
     m_topo_dim = other.m_topo_dim;

     m_points_coordinates = other.m_points_coordinates;
     m_cells_type = other.m_cells_type;
     m_cells_first_point_idx = other.m_cells_first_point_idx;
     m_cells_points = other.m_cells_points;
     m_attributes = other.m_attributes;

     m_nb_points = other.m_nb_points;
}

gnomonMeshDataStdVector:: ~gnomonMeshDataStdVector(void) {

}

gnomonAbstractMeshData* gnomonMeshDataStdVector::clone(void) const
{
    return new gnomonMeshDataStdVector(*this);
}

void gnomonMeshDataStdVector::fromGnomonForm(gnomonAbstractForm* other_f) {
    if(other_f->name() != "gnomonMesh" || other_f->dataName() != "gnomonMeshDataDefaut") {
        dtkWarn() << "cannot make a gnomonMeshDataStdVector from " << other_f->name() << " with data of type" << other_f->dataName();
        return;
    }

    gnomonMesh *other_mesh = dynamic_cast<gnomonMesh *>(other_f);
    if(other_mesh) {
        gnomonMeshDataStdVector *other = dynamic_cast<gnomonMeshDataStdVector *>(other_mesh->data());
        if(other) {
            m_nb_cells[0] = other->m_nb_cells[0];
            m_nb_cells[1] = other->m_nb_cells[1];
            m_nb_cells[2] = other->m_nb_cells[2];
            m_nb_cells[3] = other->m_nb_cells[3];
            m_nb_cells[4] = other->m_nb_cells[4];

            m_geo_dim = other->m_geo_dim;
            m_topo_dim = other->m_topo_dim;

            m_points_coordinates = other->m_points_coordinates;
            m_cells_type = other->m_cells_type;
            m_cells_first_point_idx = other->m_cells_first_point_idx;
            m_cells_points = other->m_cells_points;
            m_attributes = other->m_attributes;

            m_nb_points = other->m_nb_points;
            return;
        }
    }

                                                                                       dtkWarn() << Q_FUNC_INFO << "something went wrong, cast to mesh: " << other_mesh;
}

QMap<QString,QString> gnomonMeshDataStdVector::metadata(void) const
{
    return m_metadatas;
}

int gnomonMeshDataStdVector::geometricalDimension(void) const
{
    return m_geo_dim;
}

void gnomonMeshDataStdVector::setGeometricalDimension(int geo_dim)
{
    if(geo_dim<=0){
        dtkWarn() << "Geometrical Dimension can't be set negative";
        return;
    }

    m_geo_dim = geo_dim;
}

int gnomonMeshDataStdVector::topologicalDimension(void) const
{
    return m_topo_dim;
}

void gnomonMeshDataStdVector::setTopologicalDimension(int topo_dim)
{
    if(topo_dim<0){
        dtkWarn() << "Topological Dimension can't be set negative";
        return;
    }

    m_topo_dim = topo_dim;
}


void gnomonMeshDataStdVector::pointCoordinates(gnomonAbstractMeshData::IdxType id, double* point_coordinates) const
{
    Q_ASSERT(id<m_nb_points);

    //point_coordinates should be an array of size geometrical_dimension
    point_coordinates[0] = m_points_coordinates[3*id];
    point_coordinates[1] = m_points_coordinates[3*id+1];
    if( m_geo_dim==3 ) point_coordinates[2] = m_points_coordinates[3*id+2];
}

const double* gnomonMeshDataStdVector::pointsCoordinates(void) const
{
    return m_points_coordinates.data();
}

gnomonAbstractMeshData::CntType gnomonMeshDataStdVector::pointsCount(void) const
{
    return m_nb_points;
}

void gnomonMeshDataStdVector::setPoint(gnomonAbstractMeshData::IdxType point_id, const double* point_coordinates)
{
    Q_ASSERT(point_id<m_nb_points);

    m_points_coordinates[3*point_id]=point_coordinates[0];
    m_points_coordinates[3*point_id+1]=point_coordinates[1];
    m_points_coordinates[3*point_id+2]= ((m_geo_dim==3)? point_coordinates[2] : 0.0);
}

void gnomonMeshDataStdVector::setPointsCount(gnomonAbstractMeshData::CntType point_count)
{
    // if new size greater, do a resize
    if(point_count> m_nb_points)
        m_points_coordinates.resize(3*point_count);

    m_nb_points = point_count;
}

void gnomonMeshDataStdVector::setPoints(const double* points_coordinates, gnomonAbstractMeshData::CntType points_count)
{
    if(points_count) {
        setPointsCount(points_count);
        gnomonAbstractMeshData::CntType coordiantes_count = points_count*((m_geo_dim==3)? 3 : 2);
        m_points_coordinates.resize(coordiantes_count);
        std::copy(points_coordinates, points_coordinates + coordiantes_count, m_points_coordinates.begin());
    }
    else
        std::copy(points_coordinates, points_coordinates + m_nb_points*((m_geo_dim==3)? 3 : 2), m_points_coordinates.begin());
}

void gnomonMeshDataStdVector::cellPoints(gnomonAbstractMeshData::IdxType cell_id, int &out_nb_points_cell, const gnomonAbstractMeshData::IdxType*& out_first_point) const // out nb points of cell , out_first_point pointer to the first point of the cell
{
    Q_ASSERT(cell_id <  m_nb_cells[4]);
    out_nb_points_cell = m_cells_points[m_cells_first_point_idx[cell_id]];
    out_first_point = &m_cells_points[m_cells_first_point_idx[cell_id]];
    ++out_first_point;
}

QList<long> gnomonMeshDataStdVector::cellPointIdx(gnomonAbstractMeshData::IdxType cell_id) const // out nb points of cell
{
    Q_ASSERT(cell_id <  m_nb_cells[4]);
    auto nb_points_cell = m_cells_points[m_cells_first_point_idx[cell_id]];

    const gnomonAbstractMeshData::IdxType* first_point = &m_cells_points[m_cells_first_point_idx[cell_id]];
    ++first_point;
    QList<long> cell_points;
    for (auto i=0; i<nb_points_cell; i++) {
        cell_points.append(*first_point);
        ++first_point;
    }
    return cell_points;
}

gnomonAbstractMeshData::CellType gnomonMeshDataStdVector::cellType(gnomonAbstractMeshData::IdxType cell_id) const
{
    return  m_cells_type[cell_id];
}

gnomonAbstractMeshData::CntType gnomonMeshDataStdVector::cellsCount(int geo_dimension) const
{
    return  m_nb_cells[geo_dimension];
}

const gnomonAbstractMeshData::IdxType* gnomonMeshDataStdVector::cellsIdx(gnomonAbstractMeshData::CntType& out_nb_cells, int geo_dimension) const
{
    Q_ASSERT(geo_dimension>=0 && geo_dimension<5);

    gnomonAbstractMeshData::CntType idx = 0;

    if( geo_dimension > 0)
        idx += m_nb_cells[0];

    if( geo_dimension > 1)
        idx += m_nb_cells[1];

    if(geo_dimension > 2)
        idx += m_nb_cells[2];

    if(geo_dimension > 3)
        idx=0;

    out_nb_cells = m_nb_cells[geo_dimension];

    return &m_cells_first_point_idx[idx];
}

//return format  (nb_points, point_0, point_1, .., point_{nb_points-1} ) for each cel . out_nb_points = out_size = nb_points + 1*nb_cells_out
const gnomonAbstractMeshData::IdxType* gnomonMeshDataStdVector::cellsPoints(gnomonAbstractMeshData::CntType& out_nb_points, int geo_dimension) const
{
    Q_ASSERT(geo_dimension>=0 && geo_dimension<5);

    gnomonAbstractMeshData::CntType out_idx = 0; //index in m_cells_first_point_idx of the first cell of queried dimension
    for (int i=0; i<geo_dimension; ++i)
    {
        out_idx += m_nb_cells[i];
    }

    gnomonAbstractMeshData::CntType idx_dim_supp; //index in m_cells_first_point)idx of the first cell of (queried dimension +1)

    //compute idx dim_supp
    if(geo_dimension<3) {
        idx_dim_supp = out_idx + m_nb_cells[geo_dimension];
        out_nb_points = m_cells_first_point_idx[idx_dim_supp] - m_cells_first_point_idx[out_idx] ;
    }
    else {
        //for dimension 3 or for the total
        idx_dim_supp =  m_cells_first_point_idx.size()-1;
        if(geo_dimension == 4)
            out_idx=0;

        out_nb_points = m_cells_first_point_idx[idx_dim_supp] - m_cells_first_point_idx[out_idx] ;
    }
    return &m_cells_points[m_cells_first_point_idx[out_idx]];
}

const gnomonAbstractMeshData::CellType* gnomonMeshDataStdVector::cellsType(int geo_dimension) const
{
    Q_ASSERT(geo_dimension>=0 && geo_dimension < 5);

    if(geo_dimension == 4) {
        return  m_cells_type.data();
    }


    gnomonAbstractMeshData::CntType out_idx = 0;
    for (int i=0; i<geo_dimension; ++i)
    {
        out_idx += m_nb_cells[i];
    }

    return &m_cells_type[out_idx];
}

const gnomonAbstractMeshData::IdxType* gnomonMeshDataStdVector::cellsTopologyLocation(int geo_dimension) const
{
    Q_ASSERT(geo_dimension>=0 && geo_dimension < 5);

    if(geo_dimension == 4) {
        return  m_cells_first_point_idx.data();
    }


    gnomonAbstractMeshData::CntType out_idx = 0;
    for (int i=0; i<geo_dimension; ++i)
    {
        out_idx += m_nb_cells[i];
    }

    return &m_cells_first_point_idx[out_idx];
}

unsigned int gnomonMeshDataStdVector::facet_corner_id(const gnomonAbstractMeshData::IdxType face_id, const gnomonAbstractMeshData::IdxType corner_id) const
{
    return m_cells_points[2*m_nb_cells[0]+3*m_nb_cells[1]+4*(face_id)+1+corner_id];
}


void gnomonMeshDataStdVector::setCellsCount(gnomonAbstractMeshData::CntType cells_count, int geo_dimension)
{
    Q_ASSERT(geo_dimension >= 0  && geo_dimension<4);

    if( m_nb_cells[geo_dimension] < cells_count) {
        m_nb_cells[geo_dimension] = cells_count;
        m_nb_cells[4] =  m_nb_cells[0] + m_nb_cells[1] + m_nb_cells[2] + m_nb_cells[3];

        //do a realloc
        m_cells_type.resize(m_nb_cells[4]);
        m_cells_first_point_idx.resize( m_nb_cells[4]+1);
    }
    else {
        m_nb_cells[geo_dimension] = cells_count;
        m_nb_cells[4] = m_nb_cells[0] + m_nb_cells[1] + m_nb_cells[2] + m_nb_cells[3];
    }
}

void gnomonMeshDataStdVector::setCellsType(const gnomonAbstractMeshData::CellType* cells_type, gnomonAbstractMeshData::CntType cells_count, gnomonAbstractMeshData::CntType offset)
{
    if(m_nb_cells[4] == 0) {
        dtkInfo() << "Mesh doesn't have cells!";
        return;
    }
    if(cells_count == 0) {
        std::copy(cells_type, cells_type + m_nb_cells[4], m_cells_type.begin());
    }
    else {
        if((cells_count+offset) <= m_nb_cells[4]) {
            for(long long int i=0; i<cells_count; ++i) {
                 m_cells_type[i+offset] = cells_type[i];
            }
        }
        else
            dtkError() << __func__ << "nb_cells[4]=" << m_nb_cells[4] << "cells_count+offset=" << cells_count+offset;
    }

    //fill m_cells_first_point_idx
    gnomonAbstractMeshData::IdxType current_index=m_cells_first_point_idx[offset];
    for(gnomonAbstractMeshData::CntType idx=offset; idx<m_nb_cells[4]; ++idx)
    {
        m_cells_first_point_idx[idx] = current_index;
        current_index += nb_points_per_cell_type[m_cells_type[idx]] + 1; //+1 since for each cell, there is (nb_points, point_0, point_1, .., point{nb_points-1} )
    }
    // extra entry for last value
    m_cells_first_point_idx[m_nb_cells[4]] = current_index-1;
}

void gnomonMeshDataStdVector::setCellsPoints(const gnomonAbstractMeshData::IdxType* cells_points, gnomonAbstractMeshData::CntType size_cells_points)
{
    m_cells_points.resize(size_cells_points);
    std::copy(cells_points, cells_points + size_cells_points, m_cells_points.begin());
}

void gnomonMeshDataStdVector::addAttribute(const gnomonMeshAttribute * attribute)
{
    m_attributes.resize(m_attributes.size()+1);
    m_attributes[m_attributes.size()-1]  =*attribute;
}

const gnomonMeshAttribute* gnomonMeshDataStdVector::attribute(const QString& name) const
{
    for (auto i=0u; i< m_attributes.size(); ++i)
    {
        if(m_attributes[i].m_name == name)
            return &m_attributes[i];
    }

    return nullptr;
}

const gnomonMeshAttribute* gnomonMeshDataStdVector::attributes(void) const
{
    return m_attributes.data();
}


gnomonAbstractMeshData::CntType gnomonMeshDataStdVector::attributesCount(void) const
{
    return m_attributes.size();
}
