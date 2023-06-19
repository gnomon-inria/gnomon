#include "gnomonMeshVtkVisualization.h"
#include "gnomonForm/gnomonAbstractForm.h"
#include "gnomonVisualizations/gnomonAbstractVisualization_p.h"
#include "gnomonVisualizations/gnomonAbstractVtkVisualization_p.h"

#include <dtkLog.h>
#include <gnomonVisualization/gnomonCoreParameterColor.h>

#include "gnomonView/gnomonVtkView.h"

#include <gnomonCore/gnomonForm/gnomonMesh/gnomonAbstractMeshData>
#include <gnomonCore/gnomonForm/gnomonMesh/gnomonMeshAttribute.h>

#include "gnomonActor/gnomonMesh/gnomonPolyDataMesh.h"
#include "gnomonActor/gnomonPolyData/gnomonActorPolyData.h"
#include "gnomonActor/gnomonPolyData/gnomonActor2DPolyData.h"
#include "gnomonVisualizations/gnomonMesh/gnomonVtkDecorator.h"
#include "gnomonVisualizations/gnomonMesh/gnomonVtkDecoratorSurfaceColor.h"
#include "gnomonVisualizations/gnomonMesh/gnomonVtkDecoratorVectorGlyphs.h"

#include <vtkAbstractArray.h>
#include <vtkActor.h>
#include <vtkCellData.h>
#include <vtkDataArray.h>
#include <vtkDataSetMapper.h>
#include <vtkDoubleArray.h>
#include <vtkPlane.h>
#include <vtkPlaneWidget.h>
#include <vtkProperty.h>
#include <vtkPointData.h>
#include <vtkPoints.h>
#include <vtkPointSet.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkSmartPointerBase.h>
#include <vtkUnstructuredGrid.h>

// /////////////////////////////////////////////////////////////////
// gnomonMeshVtkVisualizationPrivate
// /////////////////////////////////////////////////////////////////

class gnomonMeshVtkVisualizationPrivate
{
public:
    std::shared_ptr<gnomonMeshSeries> meshSeries;
    std::shared_ptr<gnomonMesh> mesh;
    QMap<QString, QString> parameters_groups;

public:
    vtkSmartPointer<vtkUnstructuredGrid> grid = nullptr;
    vtkSmartPointer<vtkPlane> clipping_plane = nullptr;
    QList<gnomonVtkDecorator *> decorators;

public:
    void updateGrid(void);
};

void gnomonMeshVtkVisualizationPrivate::updateGrid(void)
{
    const gnomonAbstractMeshData* mesh_data = mesh->data();
    if(!mesh_data || mesh_data->geometricalDimension() != 3) {
        dtkWarn() << "updateGrid, mesh_data NOK" << mesh_data;
        return;
    }

    grid = vtkSmartPointer<vtkUnstructuredGrid>::New();

    // Set points
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    points->SetDataTypeToDouble();
    auto *point_array = static_cast<vtkDataArray*>(points->GetData());
    point_array->SetVoidArray(const_cast<double*>(mesh_data->pointsCoordinates()), mesh_data->pointsCount() * mesh_data->geometricalDimension(), 1);
    grid->SetPoints(points);

    //set cells
    const gnomonAbstractMeshData::IdxType cells_nb = mesh_data->cellsCount();
    grid->Allocate(cells_nb);
    for(gnomonAbstractMeshData::IdxType cell_id = 0; cell_id < cells_nb; ++cell_id) {
        gnomonAbstractMeshData::CellType type = mesh_data->cellType(cell_id);
        int cell_pts_nb;
        const gnomonAbstractMeshData::IdxType* cell_pt_ids;
        mesh_data->cellPoints(cell_id, cell_pts_nb, cell_pt_ids);
        grid->InsertNextCell(type,
                             cell_pts_nb,
                             const_cast<gnomonAbstractMeshData::IdxType*>(cell_pt_ids));
    }

    // Set attributes
    const gnomonAbstractMeshData::IdxType points_nb = mesh_data->pointsCount();
    const gnomonMeshAttribute* attributes = mesh_data->attributes();

    gnomonAbstractMeshData::CntType attr_nb = mesh_data->attributesCount();
    for(gnomonAbstractMeshData::IdxType i = 0; i < attr_nb; ++i) {
        const gnomonMeshAttribute& attr = attributes[i];
        vtkSmartPointer<vtkDoubleArray> vtk_attr = vtkSmartPointer<vtkDoubleArray>::New();
        vtk_attr->SetName(qPrintable(attr.m_name));
        vtk_attr->SetNumberOfComponents(attr.m_kind);

        if(attr.m_support == gnomonMeshAttribute::Cell) {
            vtk_attr->SetNumberOfTuples(cells_nb);
            if((cells_nb-1)*attr.m_kind > attr.m_data.size()) {
                dtkWarn() << "For field " << attr.m_name
                          << "kind " <<  attr.m_kind
                          << "nb values : " << attr.m_data.size()
                          << "but kind*numberOfTuples : " << cells_nb*attr.m_kind;
                continue;
            }
            for(gnomonAbstractMeshData::IdxType cell_id = 0; cell_id < cells_nb; ++cell_id)
                vtk_attr->SetTuple(cell_id, &attr.m_data.data()[cell_id*attr.m_kind]);

            grid->GetCellData()->AddArray(vtk_attr);
        } else if(attr.m_support == gnomonMeshAttribute::Point) {
            vtk_attr->SetNumberOfTuples(points_nb);

            if((points_nb-1)*attr.m_kind > attr.m_data.size()) {
                dtkWarn() << "For field " << attr.m_name
                          << "kind " <<  attr.m_kind
                          << "nb values : " << attr.m_data.size()
                          << "but kind*numberOfTuples : " << points_nb*attr.m_kind;
                continue;
            }

            for(gnomonAbstractMeshData::IdxType point_id = 0; point_id < points_nb; ++point_id)
                vtk_attr->SetTuple(point_id, &attr.m_data.data()[point_id*attr.m_kind]);
            grid->GetPointData()->AddArray(vtk_attr);
        }
    }

    for(auto decorator : decorators) {
        decorator->setGrid(grid);
    }
}

// /////////////////////////////////////////////////////////////////
// gnomonMeshVtkVisualization
// /////////////////////////////////////////////////////////////////

gnomonMeshVtkVisualization::gnomonMeshVtkVisualization(void) : gnomonAbstractMeshVtkVisualization(), ddd(new gnomonMeshVtkVisualizationPrivate)
{
    ddd->decorators.append(new gnomonVtkDecoratorSurfaceColor());
    //ddd->decorators.append(new gnomonVtkDecoratorVectorGlyphs());

    for(auto decorator : ddd->decorators) {
        auto params = decorator->parameters();
        QString dec_name = decorator->name();
        for(auto param_key : params.keys()) {

            QString param_name = dec_name + "_" + param_key;
            d->parameters.insert(param_name, params[param_key]);
            ddd->parameters_groups[param_name] = dec_name;
        }
    }
}

gnomonMeshVtkVisualization::~gnomonMeshVtkVisualization(void)
{
    for(auto decorator : ddd->decorators) {
        delete decorator;
    }
    ddd->decorators.clear();

    delete ddd;
}

const QString gnomonMeshVtkVisualization::pluginName(void)
{
    return  "gnomonMeshVtkVisualization";
}

void gnomonMeshVtkVisualization::clear(void)
{
    auto view = dynamic_cast<gnomonVtkView *>(d->view);
    if (view) {
        for(auto decorator : ddd->decorators) {
            decorator->removeActor();
        }
    }
}

void gnomonMeshVtkVisualization::fill(void)
{
    auto view = dynamic_cast<gnomonVtkView *>(d->view);
    if (view) {
        for(auto decorator : ddd->decorators) {
            decorator->AddActorIfPossible();
        }
    }
}

void gnomonMeshVtkVisualization::setVisible(bool visible)
{
    for(auto decorator : ddd->decorators) {
        decorator->AddActorIfPossible();
    }
}

void gnomonMeshVtkVisualization::setView(gnomonAbstractView *view)
{
    gnomonAbstractVtkVisualization::setView(view);
    gnomonVtkView *vtk_view = dynamic_cast<gnomonVtkView *>(view);
    if(!vtk_view) {
        dtkWarn() << Q_FUNC_INFO << "view not a vtk view " << view;
        return;
    }

    for(auto decorator : ddd->decorators) {
        decorator->setView(vtk_view);
    }
}

void gnomonMeshVtkVisualization::setMesh(std::shared_ptr<gnomonMeshSeries> mesh)
{
    ddd->meshSeries = mesh;
    ddd->mesh = mesh->current();

    //this->setParameter("alpha",1.0);
    ddd->updateGrid();
}

std::shared_ptr<gnomonMeshSeries> gnomonMeshVtkVisualization::mesh(void)
{
    return ddd->meshSeries;
}


QImage gnomonMeshVtkVisualization::imageRendering(void)
{
    qDebug() << Q_FUNC_INFO;
    double bounds[6];
    ddd->grid->GetBounds(bounds);

    if (bounds[4]==bounds[5]) {
        double size = ((bounds[1]-bounds[0])+(bounds[3]-bounds[2]))/4;
        bounds[4] = bounds[4] - size/2.;
        bounds[5] = bounds[5] + size/2.;
    }

    this->updateOffscreenRenderer(bounds[0],bounds[1],bounds[2],bounds[3],bounds[4],bounds[5]);

    for(auto *decorator : ddd->decorators) {
        this->offscreenRenderer()->AddActor(decorator->actor());
    }

    return this->offscreenImageRendering();
}

void gnomonMeshVtkVisualization::update(void)
{
    this->render();
}

void gnomonMeshVtkVisualization::render(void)
{
    ((gnomonVtkView *) d->view)->render();
}

dtkCoreParameters gnomonMeshVtkVisualization::parameters(void) const
{
    return d->parameters;
}

void gnomonMeshVtkVisualization::setParameter(const QString& parameter, const QVariant& value)
{
    if (d->parameters.contains(parameter)) {
        d->parameters[parameter]->setValue(value);
    }
    else
        qWarning()<<parameter<<"is not a valid parameter!";
}

void gnomonMeshVtkVisualization::setParameters(const dtkCoreParameters& parameters)
{
    for (const auto& param : parameters.keys()) {
        if (d->parameters.contains(param)) {
            d->parameters[param] = parameters[param];
        }
    }
}

void gnomonMeshVtkVisualization::refreshParameters(void)
{

}

QMap<QString, QString> gnomonMeshVtkVisualization::parameterGroups(void)
{
    return ddd->parameters_groups;;
}

void gnomonMeshVtkVisualization::onSliceOrientationChanged(int value)
{
    if(!ddd->clipping_plane) {
        ddd->clipping_plane = vtkSmartPointer<vtkPlane>::New();
        ddd->clipping_plane->SetDebug(true);

        for(auto decorator : ddd->decorators) {
            decorator->set2DClippingPlane(ddd->clipping_plane);
        }
    }

    if(!ddd->grid) {
        dtkTrace() << Q_FUNC_INFO << "Grid not set";
        return;
    }

    ddd->clipping_plane->SetOrigin(ddd->grid->GetCenter());
    switch(value) {
    case 0:
        ddd->clipping_plane->SetNormal(1., 0., 0.);
        break;
    case 1:
        ddd->clipping_plane->SetNormal(0., 1., 0.);
        break;
    case 2:
        ddd->clipping_plane->SetNormal(0., 0., 1.);
        break;
    default:
        dtkWarn() << Q_FUNC_INFO << "value is not set : " << value;
        return;
    }
}

void gnomonMeshVtkVisualization::onSliceChanged(int value)
{
    if(!d->view)
        return;

    double origin[3];
    ddd->clipping_plane->GetOrigin(origin);
    auto ori = (dynamic_cast<gnomonVtkView *>(d->view))->orientation();
    if(ori == -1) {
        dtkWarn() << Q_FUNC_INFO << "bad orientation: " << ori;
        return;
    }

    origin[ori] = value;
    ddd->clipping_plane->SetOrigin(origin);
    this->render();
}

void gnomonMeshVtkVisualization::on3D(void)
{
    this->render();
}

void gnomonMeshVtkVisualization::on2D(void)
{
    this->render();
}

void gnomonMeshVtkVisualization::onXY(void)
{
    this->render();
}

void gnomonMeshVtkVisualization::onYZ(void)
{
    this->render();
}

void gnomonMeshVtkVisualization::onXZ(void)
{
    this->render();
}

void gnomonMeshVtkVisualization::onTimeChanged(double value)
{
    if (ddd->meshSeries->times().contains(value)) {
        ddd->mesh = ddd->meshSeries->at(value);
        ddd->updateGrid();
        this->update();
    }
    this->render();
}

const QString gnomonMeshVtkVisualization::name(void) {
    return "Mesh Visualization";
}

//
// gnomonMeshVtkVisualization.cpp ends here
