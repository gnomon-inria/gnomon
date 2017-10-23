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

#include "tissueCoreVisuWidget.h"

#include <dtkWidgets>

#include <dtkComposer/dtkComposerViewManager.h>
#include <dtkComposer/dtkComposerViewWidget.h>
#include <dtkComposer/dtkComposerViewController.h>

#include <QVTKOpenGLWidget.h>
#include <vtkContextView.h>
#include <vtkGenericOpenGLRenderWindow.h>

#include <vtkActor.h>
#include <vtkCellArray.h>
#include <vtkDoubleArray.h>
#include <vtkFloatArray.h>
#include <vtkIntArray.h>
#include <vtkCellData.h>
#include <vtkPointData.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>

#include "tissueCellComplexData.h"

class tissueCoreVisuWidgetPrivate
{
public:
    vtkGenericOpenGLRenderWindow *window;
    QVTKOpenGLWidget *widget;
    vtkRenderer *renderer;

public:
    vtkPolyData *data;
};

tissueCoreVisuWidget::tissueCoreVisuWidget(QWidget *parent) : QWidget(parent)
{
    d = new tissueCoreVisuWidgetPrivate;

    d->data = nullptr;

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

tissueCoreVisuWidget::~tissueCoreVisuWidget(void)
{
    d->renderer->Delete();
    d->window->Delete();

    delete d->widget;
    delete d;
}

void tissueCoreVisuWidget::addCellComplex(tissueCellComplexData &cell)
{
    // if ( ! d->data ) {
    //     qDebug() << Q_FUNC_INFO << "cellData already pushed";
    //     return;
    // }

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

    qDebug() << Q_FUNC_INFO << "Object added";

    return;
}

//
// tissueCoreVisuWidget.cpp ends here
