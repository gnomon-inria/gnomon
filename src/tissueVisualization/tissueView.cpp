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

#include "tissueView.h"

#include <dtkWidgets>

#include <vtkActor.h>
#include <vtkCellArray.h>
#include <vtkContextView.h>
#include <vtkDoubleArray.h>
#include <vtkFloatArray.h>
#include <vtkGlyph3D.h>
#include <vtkIntArray.h>
#include <vtkCellData.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkPointData.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSphereSource.h>

#include <QVTKOpenGLWidget.h>

#include "tissueCellComplex.h"
#include "tissueCellGraph.h"

class tissueViewPrivate
{
public:
    vtkGenericOpenGLRenderWindow *window;
    QVTKOpenGLWidget *widget;
    vtkRenderer *renderer;
};

tissueView::tissueView(QWidget *parent) : dtkViewWidget(parent)
{
    d = new tissueViewPrivate;

    d->renderer = vtkRenderer::New();
    d->renderer->SetBackground(1, 1, 1);

    d->window = vtkGenericOpenGLRenderWindow::New();
    d->window->AddRenderer(d->renderer);

    d->widget = new QVTKOpenGLWidget(this);
    d->widget->SetRenderWindow(d->window);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(0);
    layout->addWidget(d->widget);
    layout->setContentsMargins(0, 0, 0, 0);

    this->setLayout(layout);
}

tissueView::~tissueView(void)
{
    d->renderer->Delete();
    d->window->Delete();

    delete d->widget;
    delete d;
}

QWidget *tissueView::widget(void)
{
    return d->widget;
}

void tissueView::addCellComplex(tissueCellComplex &cell)
{
    vtkPoints* polydataPoints = vtkPoints::New();
    vtkCellArray* polydataFaces = vtkCellArray::New();
    vtkDoubleArray* polydataFaceData = vtkDoubleArray::New();

    QMap<long, QVariant> positions = cell.elementProperty(0,"position");

    QMap<long,long> vertexPoint;

    QList<long> vertices = cell.elementIds(0);

    for (const auto& vertexId : vertices) {
        std::vector<double> pos = positions[vertexId].value<std::vector<double> >();
        long vtkId = polydataPoints->InsertNextPoint(pos[0],pos[1],pos[2]);
        vertexPoint[vertexId] = vtkId;
    }

    QList<long> faces = cell.elementIds(2);

    for (const auto& faceId : faces) {
        QList<long> faceVertices = cell.orientedFaceVertexIds(faceId);
        long vtkId = polydataFaces->InsertNextCell(faceVertices.size());
        for (const auto& v : faceVertices) {
            polydataFaces->InsertCellPoint(vertexPoint[v]);
        }
        polydataFaceData->InsertValue(vtkId,faceId);
    }

    vtkPolyData* polydata = vtkPolyData::New();
    polydata->SetPoints(polydataPoints);
    polydata->SetPolys(polydataFaces);
    polydata->GetCellData()->SetScalars(polydataFaceData);

    vtkPolyDataMapper* mapper = vtkPolyDataMapper::New();
    mapper->SetInputData(polydata);
    mapper->SetScalarRange(0, cell.elementCount(2)-1);

    vtkActor* actor = vtkActor::New();
    actor->SetMapper(mapper);

    d->renderer->AddActor(actor);

    return;
}

void tissueView::addCellGraph(tissueCellGraph &graph)
{
    vtkPoints* polydataPoints = vtkPoints::New();
    vtkCellArray* polydataLines = vtkCellArray::New();
    vtkDoubleArray* polydataPointData = vtkDoubleArray::New();

    QMap<long, QVariant> positions = graph.vertexProperty("barycenter");

    QMap<long,long> vertexPoint;

    QList<long> vertices = graph.vertexIds();

    for (const auto& vertexId : vertices) {
        std::vector<double> pos = positions[vertexId].value<std::vector<double> >();
        long vtkId = polydataPoints->InsertNextPoint(pos[0],pos[1],pos[2]);
        vertexPoint[vertexId] = vtkId;
        polydataPointData->InsertValue(vtkId,vertexId);
    }

    QList<long> edges = graph.edgeIds();

    for (const auto& edgeId : edges) {
        QList<long> edgeVertices = graph.edgeVertexIds(edgeId);
        long vtkId = polydataLines->InsertNextCell(edgeVertices.size());
        for (const auto& v : edgeVertices) {
            polydataLines->InsertCellPoint(vertexPoint[v]);
        }
    }

    vtkPolyData* linePolydata = vtkPolyData::New();
    linePolydata->SetPoints(polydataPoints);
    linePolydata->SetLines(polydataLines);

    vtkPolyDataMapper* lineMapper = vtkPolyDataMapper::New();
    lineMapper->SetInputData(linePolydata);
    // lineMapper->SetScalarRange(0, 1);

    vtkActor* lineActor = vtkActor::New();
    lineActor->SetMapper(lineMapper);
    d->renderer->AddActor(lineActor);

    vtkPolyData* pointPolydata = vtkPolyData::New();
    pointPolydata->SetPoints(polydataPoints);
    pointPolydata->GetPointData()->SetScalars(polydataPointData);

    vtkSphereSource* sphere = vtkSphereSource::New();
    sphere->SetRadius(1);
    sphere->SetThetaResolution(12);
    sphere->SetPhiResolution(12);
    sphere->Update();

    vtkGlyph3D* glyph = vtkGlyph3D::New();
    glyph->SetScaleModeToDataScalingOff();
    glyph->SetColorModeToColorByScalar();
    glyph->SetSourceData(sphere->GetOutput());
    glyph->SetInputData(pointPolydata);
    glyph->Update();

    vtkPolyDataMapper* pointMapper = vtkPolyDataMapper::New();
    pointMapper->SetInputData(glyph->GetOutput());
    pointMapper->SetScalarRange(0, graph.vertexCount()-1);

    vtkActor* pointActor = vtkActor::New();
    pointActor->SetMapper(pointMapper);
    d->renderer->AddActor(pointActor);

    return;
}

//
// tissueView.cpp ends here
