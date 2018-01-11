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

#include "gnomonActorMesh.h"
#include "gnomonActorMesh_p.h"
#include "gnomonActorMeshCellGraph.h"

#include <gnomonCellGraph.h>

#include <QtWidgets>

#include <vtkActor.h>
#include <vtkCommand.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkColorTransferFunction.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkDoubleArray.h>
#include <vtkFloatArray.h>
#include <vtkGlyph3D.h>
#include <vtkCellData.h>
#include <vtkPointData.h>
#include <vtkProperty.h>

// /////////////////////////////////////////////////////////////////
// gnomonActorMeshCellGraphPrivate
// /////////////////////////////////////////////////////////////////

class gnomonActorMeshCellGraphPrivate
{
public:
    gnomonCellGraph *cellgraph;

    vtkSmartPointer<vtkSphereSource> sphere;

    vtkSmartPointer<vtkPolyData> point_mesh;
    vtkSmartPointer<vtkGlyph3D> point_glyph;
    vtkSmartPointer<vtkPolyDataMapper> point_mapper;
    vtkSmartPointer<vtkActor> point_actor;


public:

    QString vertexPropertyName;
    double range_min;
    double range_max;
    QList<double> vertexPropertyRange;

    double vertexSize;
    double edgeOpacity;
    double edgeLinewidth;

    QMap<double, QColor> colormap;

    QMap<QString,QList<double>> slice;

    QString filterPropertyName;
    double filter_range_min;
    double filter_range_max;
    QList<double> filterPropertyRange;
};

// /////////////////////////////////////////////////////////////////
// gnomonActorMeshCellGraph
// /////////////////////////////////////////////////////////////////

vtkStandardNewMacro(gnomonActorMeshCellGraph);

gnomonCellGraph *gnomonActorMeshCellGraph::cellGraph(void)
{
    return dd->cellgraph;
}

void gnomonActorMeshCellGraph::update(void)
{
    if(!dd->cellgraph)
        return;

    if(!d->interactor)
        return;

    vtkSmartPointer<vtkPoints> polydataPoints = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkCellArray> polydataLines = vtkSmartPointer<vtkCellArray>::New();

    vtkSmartPointer<vtkDoubleArray> polydataPointData = vtkSmartPointer<vtkDoubleArray>::New();

    QMap<long, QVariant> positions_x = dd->cellgraph->vertexProperty("barycenter_x");
    QMap<long, QVariant> positions_y = dd->cellgraph->vertexProperty("barycenter_y");
    QMap<long, QVariant> positions_z = dd->cellgraph->vertexProperty("barycenter_z");

    QMap<QString, QMap<long, QVariant>> positions;
    positions["x"] = positions_x;
    positions["y"] = positions_y;
    positions["z"] = positions_z;

    for (const auto& dim : positions.keys()) {
        if (dd->slice[dim].size() == 0) {
            QList<double> points;
            for (const auto& vertexId : dd->cellgraph->vertexIds()) {
                points<<positions[dim][vertexId].value<double>();
            }
            double pointMin = floor(*std::min_element(points.begin(),points.end()));
            double pointMax = ceil(*std::max_element(points.begin(),points.end()));
            dd->slice[dim]<<pointMin<<pointMax;
        }
    }

    QMap<long,long> vertexPoint;

    QList<long> vertices = dd->cellgraph->vertexIds();

    QMap<long, double> vertexScalarProperty;
    for (const auto& vertexId : vertices) {
        if (dd->cellgraph->vertexPropertyNames().contains(dd->vertexPropertyName)) {
            vertexScalarProperty[vertexId] = dd->cellgraph->vertexProperty(dd->vertexPropertyName)[vertexId].value<double>();
        } else {
            vertexScalarProperty[vertexId] = vertexId;
        }
    }

    QList<double> vertexScalarPropertyValues = vertexScalarProperty.values();
    auto mm = std::minmax_element(vertexScalarPropertyValues.begin(),vertexScalarPropertyValues.end());
    dd->range_min = *(mm.first);
    dd->range_max = *(mm.second);
    if (dd->vertexPropertyRange.size() == 0) {
        dd->vertexPropertyRange<<dd->range_min<<dd->range_max;
    }

    QMap<long, double> filterScalarProperty;
    for (const auto& vertexId : vertices) {
        if (dd->cellgraph->vertexPropertyNames().contains(dd->filterPropertyName)) {
            filterScalarProperty[vertexId] = dd->cellgraph->vertexProperty(dd->filterPropertyName)[vertexId].value<double>();
        } else {
            filterScalarProperty[vertexId] = vertexId;
        }
    }

    QList<double> filterScalarPropertyValues = filterScalarProperty.values();
    auto filter_mm = std::minmax_element(filterScalarPropertyValues.begin(),filterScalarPropertyValues.end());
    dd->filter_range_min = *(filter_mm.first);
    dd->filter_range_max = *(filter_mm.second);
    if (dd->filterPropertyRange.size() == 0) {
        dd->filterPropertyRange<<dd->range_min<<dd->range_max;
    }

    for (const auto& vertexId : vertices) {
        double x = positions_x[vertexId].value<double>();
        double y = positions_y[vertexId].value<double>();
        double z = positions_z[vertexId].value<double>();
        double filter = filterScalarProperty[vertexId];
        bool vertexDisplay = true;
        vertexDisplay = vertexDisplay&&(x>=dd->slice["x"][0])&&(x<=dd->slice["x"][1]);
        vertexDisplay = vertexDisplay&&(y>=dd->slice["y"][0])&&(y<=dd->slice["y"][1]);
        vertexDisplay = vertexDisplay&&(z>=dd->slice["z"][0])&&(z<=dd->slice["z"][1]);
        vertexDisplay = vertexDisplay&&(filter>=dd->filterPropertyRange[0])&&(filter<=dd->filterPropertyRange[1]);
        if (vertexDisplay) {
            long vtkId = polydataPoints->InsertNextPoint(x,y,z);
            vertexPoint[vertexId] = vtkId;
            polydataPointData->InsertValue(vtkId,vertexScalarProperty[vertexId]);
        }
        // qDebug()<<vtkId<<":"<<vertexId<<" ("<<positions_x[vertexId].value<double>()<<","<<positions_y[vertexId].value<double>()<<","<<positions_z[vertexId].value<double>()<<")";
    }

    QList<long> edges = dd->cellgraph->edgeIds();

    for (const auto& edgeId : edges) {
        QList<long> edgeVertices = dd->cellgraph->edgeVertexIds(edgeId);
        bool edgeDisplay = true;
        for (const auto& v : edgeVertices) {
            edgeDisplay = edgeDisplay && (vertexPoint.keys().contains(v));
        }
        if (edgeDisplay) {
            long vtkId = polydataLines->InsertNextCell(edgeVertices.size());
            for (const auto& v : edgeVertices) {
                polydataLines->InsertCellPoint(vertexPoint[v]);
            }
        }
    }

    if (!d->mesh) {
        d->mesh = vtkSmartPointer<vtkPolyData>::New();
    }
    d->mesh->SetPoints(polydataPoints);
    d->mesh->SetLines(polydataLines);
    d->mesh->Modified();

    if (!d->mapper) {
        d->mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        d->mapper->SetInputData(d->mesh);
    }

    if(!d->actor) {
        d->actor = vtkSmartPointer<vtkActor>::New();
        d->actor->SetMapper(d->mapper);
        this->AddPart(d->actor);
    }
    d->actor->GetProperty()->SetOpacity(dd->edgeOpacity);
    d->actor->GetProperty()->SetLineWidth(dd->edgeLinewidth);
    d->actor->Modified();

    if (!dd->point_mesh) {
        dd->point_mesh = vtkSmartPointer<vtkPolyData>::New();
    }
    dd->point_mesh->SetPoints(polydataPoints);
    dd->point_mesh->GetPointData()->SetScalars(polydataPointData);

    if(!dd->sphere) {
        dd->sphere = vtkSmartPointer<vtkSphereSource>::New();
        dd->sphere->SetThetaResolution(12);
        dd->sphere->SetPhiResolution(12);
        dd->sphere->SetRadius(1);
    }
    dd->sphere->SetRadius(dd->vertexSize);
    dd->sphere->Update();

    if (!dd->point_glyph) {
        dd->point_glyph = vtkSmartPointer<vtkGlyph3D>::New();
        dd->point_glyph->SetScaleModeToDataScalingOff();
        dd->point_glyph->SetColorModeToColorByScalar();
        dd->point_glyph->SetSourceConnection(dd->sphere->GetOutputPort());
        dd->point_glyph->SetInputData(dd->point_mesh);
    }
    dd->point_glyph->Update();


    if(!d->colorFunction) {
        d->colorFunction = vtkSmartPointer<vtkColorTransferFunction>::New();
        d->colorFunction->SetColorSpaceToRGB();
    }

    d->colorFunction->RemoveAllPoints();
    for (const auto& val : dd->colormap.keys()) {
        double node = val*dd->vertexPropertyRange[1] + (1-val)*dd->vertexPropertyRange[0];
        d->colorFunction->AddRGBPoint(node, dd->colormap[val].red()/255., dd->colormap[val].green()/255., dd->colormap[val].blue()/255.);
    }
    d->colorFunction->ClampingOn();
    d->colorFunction->Modified();


    if (!dd->point_mapper) {
        dd->point_mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
        dd->point_mapper->SetInputData(dd->point_glyph->GetOutput());
        dd->point_mapper->SetScalarRange(0, dd->cellgraph->vertexCount()-1);
    }
    dd->point_mapper->SetLookupTable(d->colorFunction);
    dd->point_mapper->Modified();

    if(!dd->point_actor) {
        dd->point_actor = vtkSmartPointer<vtkActor>::New();
        dd->point_actor->SetMapper(dd->point_mapper);
        this->AddPart(dd->point_actor);
    }
    dd->point_actor->Modified();

    d->interactor->Render();
}


const QMap<double, QColor>& gnomonActorMeshCellGraph::colormap(void) const
{
    return dd->colormap;
}

void gnomonActorMeshCellGraph::setCellGraph(gnomonCellGraph *cellgraph)
{
    dd->cellgraph = cellgraph;

    this->update();
}

void gnomonActorMeshCellGraph::setColorMap(const QMap<double, QColor>& colormap)
{
    dd->colormap = colormap;
    this->update();
}

void gnomonActorMeshCellGraph::setVertexProperty(const QString& propertyName)
{
    dd->vertexPropertyName = propertyName;
    this->update();
}

void gnomonActorMeshCellGraph::setVertexPropertyRange(const QList<double>& range)
{
    dd->vertexPropertyRange = range;
    this->update();
}

void gnomonActorMeshCellGraph::setFilterProperty(const QString& propertyName)
{
    dd->filterPropertyName = propertyName;
    this->update();
}

void gnomonActorMeshCellGraph::setFilterPropertyRange(const QList<double>& range)
{
    dd->filterPropertyRange = range;
    this->update();
}

void gnomonActorMeshCellGraph::setVertexSize(double radius)
{
    dd->vertexSize = radius;
    this->update();
}

void gnomonActorMeshCellGraph::setEdgeOpacity(double opacity)
{
    dd->edgeOpacity = opacity;
    this->update();
}

void gnomonActorMeshCellGraph::setEdgeLinewidth(double linewidth)
{
    dd->edgeLinewidth = linewidth;
    this->update();
}

void gnomonActorMeshCellGraph::setSlice(const QString& dim, const QList<double>& slice)
{
    dd->slice[dim] = slice;
    this->update();
}

double gnomonActorMeshCellGraph::rangeMin() const
{
    return dd->range_min;
}

double gnomonActorMeshCellGraph::rangeMax() const
{
    return dd->range_max;
}

double gnomonActorMeshCellGraph::filterRangeMin() const
{
    return dd->filter_range_min;
}

double gnomonActorMeshCellGraph::filterRangeMax() const
{
    return dd->filter_range_max;
}

gnomonActorMeshCellGraph::gnomonActorMeshCellGraph(void) : gnomonActorMesh(), dd(new gnomonActorMeshCellGraphPrivate)
{
    dd->cellgraph = Q_NULLPTR;

    dd->vertexPropertyName = "";
    dd->vertexPropertyRange = QList<double>();

    dd->vertexSize = 1.0;
    dd->edgeOpacity = 0.5;
    dd->edgeLinewidth = 2.0;

    dd->slice["x"] = QList<double>();
    dd->slice["y"] = QList<double>();
    dd->slice["z"] = QList<double>();

    dd->filterPropertyName = "";
    dd->filterPropertyRange = QList<double>();

    dd->colormap[0] = Qt::blue;
    dd->colormap[0.5] = Qt::green;
    dd->colormap[1] = Qt::red;
}

gnomonActorMeshCellGraph::~gnomonActorMeshCellGraph(void)
{
    delete dd;

    dd = NULL;
}

//
// gnomonActorMesh.cpp ends here
