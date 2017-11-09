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

#include "gnomonView.h"
#include "gnomonViewManager.h"

#include "gnomonActor.h"
#include "gnomonActorVolume.h"
#include "gnomonActorMeshCellGraph.h"

#include "gnomonCellComplex.h"
#include "gnomonCellGraph.h"

#include <dtkWidgets>

#include <vtkActor.h>
#include <vtkCellArray.h>
#include <vtkContextView.h>
#include <vtkDoubleArray.h>
#include <vtkFloatArray.h>
#include <vtkGlyph3D.h>
#include <vtkImageData.h>
#include <vtkIntArray.h>
#include <vtkCellData.h>
#include <vtkPointData.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>

#include <vtkGenericOpenGLRenderWindow.h>
#include <QVTKOpenGLWidget.h>
// #include <vtkRenderWindow.h>
// #include <QVTKWidget.h>

class gnomonViewPrivate
{
public:
    vtkGenericOpenGLRenderWindow *window;
    QVTKOpenGLWidget *widget;
    // vtkRenderWindow *window;
    // QVTKWidget *widget;
    vtkRenderer *renderer;

public:
    gnomonViewManager *manager;
};

gnomonView::gnomonView(QWidget *parent) : dtkViewWidget(parent)
{
    d = new gnomonViewPrivate;

    d->manager = new gnomonViewManager;

    d->renderer = vtkRenderer::New();
    d->renderer->SetBackground(0.2, 0.2, 0.2);

    d->window = vtkGenericOpenGLRenderWindow::New();
    // d->window = vtkRenderWindow::New();
    d->window->AddRenderer(d->renderer);

    d->widget = new QVTKOpenGLWidget(this);
    // d->widget = new QVTKWidget(this);
    d->widget->SetRenderWindow(d->window);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(0);
    layout->addWidget(d->widget);
    layout->setContentsMargins(0, 0, 0, 0);

    this->setLayout(layout);

    connect(d->manager, SIGNAL(inserted(vtkImageData *)), this, SLOT(onInserted(vtkImageData *)));
    connect(d->manager, SIGNAL(inserted(gnomonCellGraph *)), this, SLOT(onInserted(gnomonCellGraph *)));
}

gnomonView::~gnomonView(void)
{
    d->renderer->Delete();
    d->window->Delete();

    delete d->manager;
    delete d->widget;
    delete d;
}

gnomonViewManager *gnomonView::manager(void)
{
    return d->manager;
}

QWidget *gnomonView::widget(void)
{
    return d->widget;
}

void gnomonView::addCellComplex(gnomonCellComplex &cell)
{
    vtkSmartPointer<vtkPoints> polydataPoints = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkCellArray> polydataFaces = vtkSmartPointer<vtkCellArray>::New();
    vtkSmartPointer<vtkDoubleArray> polydataFaceData = vtkSmartPointer<vtkDoubleArray>::New();

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

    vtkSmartPointer<vtkPolyData> polydata = vtkSmartPointer<vtkPolyData>::New();
    polydata->SetPoints(polydataPoints);
    polydata->SetPolys(polydataFaces);
    polydata->GetCellData()->SetScalars(polydataFaceData);

    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputData(polydata);
    mapper->SetScalarRange(0, cell.elementCount(2)-1);

    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);

    d->renderer->AddActor(actor);

    return;
}

void gnomonView::addCellGraph(gnomonCellGraph &graph)
{
    vtkSmartPointer<vtkPoints> polydataPoints = vtkSmartPointer<vtkPoints>::New();
    vtkSmartPointer<vtkCellArray> polydataLines = vtkSmartPointer<vtkCellArray>::New();
    vtkSmartPointer<vtkDoubleArray> polydataPointData = vtkSmartPointer<vtkDoubleArray>::New();

    QMap<long, QVariant> positions = graph.vertexProperty("barycenter");

    QMap<long,long> vertexPoint;

    QList<long> vertices = graph.vertexIds();

    for (const auto& vertexId : vertices) {
        std::vector<double> pos = positions[vertexId].value<std::vector<double>>();
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

    vtkSmartPointer<vtkPolyData> linePolydata = vtkSmartPointer<vtkPolyData>::New();
    linePolydata->SetPoints(polydataPoints);
    linePolydata->SetLines(polydataLines);

    vtkSmartPointer<vtkPolyDataMapper> lineMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    lineMapper->SetInputData(linePolydata);

    vtkSmartPointer<vtkActor> lineActor = vtkSmartPointer<vtkActor>::New();
    lineActor->SetMapper(lineMapper);

    vtkSmartPointer<vtkPolyData> pointPolydata = vtkSmartPointer<vtkPolyData>::New();
    pointPolydata->SetPoints(polydataPoints);
    pointPolydata->GetPointData()->SetScalars(polydataPointData);

    vtkSmartPointer<vtkSphereSource> sphere = vtkSmartPointer<vtkSphereSource>::New();
    sphere->SetRadius(1);
    sphere->SetThetaResolution(12);
    sphere->SetPhiResolution(12);
    sphere->Update();

    vtkSmartPointer<vtkGlyph3D> glyph = vtkSmartPointer<vtkGlyph3D>::New();
    glyph->SetScaleModeToDataScalingOff();
    glyph->SetColorModeToColorByScalar();
    glyph->SetSourceData(sphere->GetOutput());
    glyph->SetInputData(pointPolydata);
    glyph->Update();

    vtkSmartPointer<vtkPolyDataMapper> pointMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    pointMapper->SetInputData(glyph->GetOutput());
    pointMapper->SetScalarRange(0, graph.vertexCount()-1);

    vtkSmartPointer<vtkActor> pointActor = vtkSmartPointer<vtkActor>::New();
    pointActor->SetMapper(pointMapper);

    d->renderer->AddActor(lineActor);
    d->renderer->AddActor(pointActor);

    return;
}

void gnomonView::onInserted(vtkImageData *image)
{
    gnomonActorVolume *actor = dynamic_cast<gnomonActorVolume *>(d->manager->actor(image));
    actor->setInteractor(d->widget->GetInteractor());
    actor->setVolume(image);

    d->renderer->AddActor(actor);
}

void gnomonView::onInserted(gnomonCellGraph *cellgraph)
{
    gnomonActorMeshCellGraph *actor = dynamic_cast<gnomonActorMeshCellGraph *>(d->manager->actor(cellgraph));
    actor->setInteractor(d->widget->GetInteractor());
    actor->setCellGraph(cellgraph);

    d->renderer->AddActor(actor);
}

//
// gnomonView.cpp ends here
