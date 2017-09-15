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

#include "vtkActor.h"
#include "vtkCellArray.h"
#include "vtkDoubleArray.h"
#include "vtkFloatArray.h"
#include "vtkIntArray.h"
#include "vtkCellData.h"
#include "vtkPointData.h"
#include "vtkPoints.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"



#include "tissueCellComplexData.h"   // use tissueCellComplex in final release

class tissueCoreVisuWidgetPrivate
{
public:
    vtkGenericOpenGLRenderWindow *window;
    QVTKOpenGLWidget *widget;
    vtkRenderer *renderer;

public:
    vtkPolyData *data;
};

tissueCoreVisuWidget::tissueCoreVisuWidget(QWidget *parent) : QFrame(parent)
{
    d = new tissueCoreVisuWidgetPrivate;

    d->data = nullptr;

    d->renderer = vtkRenderer::New();
    d->renderer->SetBackground(0, 0, 0);

    d->window = vtkGenericOpenGLRenderWindow::New();
    d->window->AddRenderer(d->renderer);

    d->widget = new QVTKOpenGLWidget(this);
    d->widget->SetRenderWindow(d->window);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(0);
    layout->addWidget(d->widget);

    this->setFixedHeight(300);
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

    QMap<long,long> vertexPoint = QMap<long,long>();

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

    ///////  FAKE OBJECT

    /*vtkFloatArray* pcoords = vtkFloatArray::New();
    pcoords->SetNumberOfComponents(3);
    pcoords->SetNumberOfTuples(4);

    float pts[4][3] = { {0.0, 0.0, 0.0}, {0.0, 1.0, 0.0},
                        {1.0, 0.0, 0.0}, {1.0, 1.0, 0.0} };
    for (int i=0; i<4; i++) {
        pcoords->SetTuple(i, pts[i]);
    }

    vtkPoints* points = vtkPoints::New();
    points->SetData(pcoords);

    vtkCellArray* strips = vtkCellArray::New();
    strips->InsertNextCell(4);
    strips->InsertCellPoint(0);
    strips->InsertCellPoint(1);
    strips->InsertCellPoint(2);
    strips->InsertCellPoint(3);

    vtkIntArray* temperature = vtkIntArray::New();
    temperature->SetName("Temperature");
    temperature->InsertNextValue(10);
    temperature->InsertNextValue(20);
    temperature->InsertNextValue(30);
    temperature->InsertNextValue(40);

    vtkDoubleArray* vorticity = vtkDoubleArray::New();
    vorticity->SetName("Vorticity");
    vorticity->InsertNextValue(2.7);
    vorticity->InsertNextValue(4.1);
    vorticity->InsertNextValue(5.3);
    vorticity->InsertNextValue(3.4);

    vtkPolyData* polydata = vtkPolyData::New();
    polydata->SetPoints(points);
    polydata->SetStrips(strips);
    polydata->GetPointData()->SetScalars(temperature);
    /polydata->GetPointData()->AddArray(vorticity);
    */

    ///// END OF FAKE

    vtkPolyData* polydata = vtkPolyData::New();
    polydata->SetPoints(polydataPoints);
    polydata->SetPolys(polydataFaces);
    polydata->GetCellData()->SetScalars(polydataFaceData);

    vtkPolyDataMapper* mapper = vtkPolyDataMapper::New();
    mapper->SetInputData(polydata);
    mapper->SetScalarRange(0, 40);

    vtkActor* actor = vtkActor::New();
    actor->SetMapper(mapper);

    // vtkRenderWindowInteractor *interactor = vtkRenderWindowInteractor::New();
    // interactor->SetRenderWindow(d->window);

    d->renderer->AddActor(actor);
    // d->window->Render();

    // interactor->Start();

    qDebug() << Q_FUNC_INFO << "Object added";
    return;
}

//
// tissueCoreVisuWidget.cpp ends here
