#include "gnomonVtkDecoratorStreamTracer.h"
#include "gnomonVisualizations/gnomonMesh/gnomonVtkDecorator.h"

#include <dtkCoreParameterInList.h>
#include <dtkCoreParameterNumeric.h>
#include <dtkCoreParameters.h>

#include <vtkActor.h>
#include <vtkBoundingBox.h>
#include <vtkCellCenters.h>
#include <vtkCellData.h>
#include <vtkCenterOfMass.h>
#include <vtkColorTransferFunction.h>
#include <vtkLineRepresentation.h>
#include <vtkLineSource.h>
#include <vtkLineWidget2.h>
#include <vtkLookupTable.h>
#include <vtkPiecewiseFunction.h>
#include <vtkPointData.h>
#include <vtkPointSource.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkSmartPointerBase.h>
#include <vtkSphereRepresentation.h>
#include <vtkSphereWidget2.h>
#include <vtkStreamTracer.h>
#include <vtkTubeFilter.h>

#include <gnomonVisualization/gnomonCoreParameterColor.h>

// Observers
// used to get the widgets coordinates and move their seeds accordingly
class gnomonStreamTracerSphereObserver : public vtkCommand
{
private:
    gnomonStreamTracerSphereObserver(void) {
    };

public:
    static gnomonStreamTracerSphereObserver *New(void)
    {
        return new gnomonStreamTracerSphereObserver;
    };

    void Execute(vtkObject *caller, unsigned long event, void *) override
    {
        if (event == vtkCommand::InteractionEvent) {
            source_sphere->GetPolyData(source_data);
            stream_tracer->SetSourceData(source_data);
            stream_tracer->Modified();

            vtkSphereWidget2 *source_sphere_widget = reinterpret_cast<vtkSphereWidget2*>(caller);
            vtkSphereRepresentation *sphere_representation = vtkSphereRepresentation::SafeDownCast(source_sphere_widget->GetRepresentation());
            double pos[3];
            sphere_representation->GetCenter(pos);
            source_sphere->SetCenter(pos);
            double radius = sphere_representation->GetRadius();
            source_sphere->SetRadius(radius);
        }
    };

public:
    vtkSmartPointer<vtkPolyData> source_data;
    vtkSmartPointer<vtkSphereRepresentation> source_sphere;
    vtkSmartPointer<vtkStreamTracer> stream_tracer;
};

class gnomonStreamTracerLineObserver : public vtkCommand
{
private:
    gnomonStreamTracerLineObserver(void) {
    };

public:
    static gnomonStreamTracerLineObserver *New(void)
    {
        return new gnomonStreamTracerLineObserver;
    };

    void Execute(vtkObject *caller, unsigned long event, void *) override
    {
        if (event == vtkCommand::InteractionEvent) {
            source_line->GetPolyData(source_data);
            stream_tracer->SetSourceData(source_data);
            stream_tracer->Modified();

            vtkLineWidget2 *source_line_widget = reinterpret_cast<vtkLineWidget2*>(caller);
            vtkLineRepresentation *line_representation = vtkLineRepresentation::SafeDownCast(source_line_widget->GetLineRepresentation());
            double pos1[3];
            double pos2[3];
            line_representation->GetPoint1WorldPosition(pos1);
            line_representation->GetPoint2WorldPosition(pos2);
            source_line->SetPoint1WorldPosition(pos1);
            source_line->SetPoint2WorldPosition(pos2);
        }
    };

public:
    vtkSmartPointer<vtkPolyData> source_data;
    vtkSmartPointer<vtkLineRepresentation>   source_line;
    vtkSmartPointer<vtkStreamTracer> stream_tracer;
};


struct gnomonVtkDecoratorStreamTracerPrivate {
    gnomonVtkDecoratorStreamTracer *q = nullptr;

    std::string property_name = "";
    vtkSmartPointer<gnomonStreamTracerSphereObserver> source_sphere_observer = nullptr;
    vtkSmartPointer<vtkSphereWidget2> source_sphere_widget = nullptr;
    vtkSmartPointer<vtkSphereRepresentation> source_sphere = nullptr;

    vtkSmartPointer<gnomonStreamTracerLineObserver> source_line_observer = nullptr;
    vtkSmartPointer<vtkLineWidget2> source_line_widget = nullptr;
    vtkSmartPointer<vtkLineRepresentation> source_line = nullptr;

    vtkSmartPointer<vtkPolyData> source_data = nullptr;
    vtkSmartPointer<vtkColorTransferFunction> colorFunction = nullptr;

    vtkSmartPointer<vtkStreamTracer> streamtracer = nullptr;
    vtkSmartPointer<vtkTubeFilter> tube_filter = nullptr;

    void updateColorFunction(void);
};


void gnomonVtkDecoratorStreamTracerPrivate::updateColorFunction(void)
{
    if (!this->colorFunction || !q->m_grid || !q->m_mapper3d)
        return;

    QMap<double, QColor> colormap = ((gnomonCoreParameterColorMap *)q->m_parameters["6_colormap"])->value();

    vtkDataArray *array = q->m_grid->GetPointData()->GetArray(property_name.c_str());
    q->m_mapper3d->SetScalarModeToUsePointFieldData();
    if(q->m_mapper2d)
            q->m_mapper2d->SetScalarModeToUsePointFieldData();

    if(!array) {
        array = q->m_grid->GetCellData()->GetArray(property_name.c_str());
        q->m_mapper3d->SetScalarModeToUseCellFieldData();
        if(q->m_mapper2d)
            q->m_mapper2d->SetScalarModeToUseCellFieldData();

       if(!array) {
            dtkWarn() << Q_FUNC_INFO << "Cannot get attribute " << this->property_name.c_str();
            return;
        }
    }

    //we use current_property to color it.
    //since it's a vector ...
    //compute range if magnitude
    double norm, range[2] = {10000000000., 0.};
    int nComponents = array->GetNumberOfComponents();
    for (vtkIdType i = 0; i < array->GetNumberOfTuples(); ++i) {
        norm = vtkMath::Norm(array->GetTuple(i), nComponents);
        if (norm > range[1]) {
            range[1] = norm;
        }
        if(norm < range[0]) {
            range[0] = norm;
        }
    }
    this->colorFunction->SetVectorModeToMagnitude();

    this->colorFunction->RemoveAllPoints();
    for (const auto& val : colormap.keys()) {
        double node = val*range[1] + (1-val)*range[0];
        double r = colormap[val].red()/255.;
        double g = colormap[val].green()/255.;
        double b = colormap[val].blue()/255.;
        this->colorFunction->AddRGBPoint(node, r, g, b);
    }

    this->colorFunction->ClampingOn();
    q->m_mapper3d->SetLookupTable(this->colorFunction);
    q->m_mapper3d->SelectColorArray(property_name.c_str());
    q->m_mapper3d->SetScalarRange(range[0], range[1]);
    q->m_mapper3d->Modified();

    if(q->m_mapper2d) {
        q->m_mapper2d->SetLookupTable(this->colorFunction);
        q->m_mapper2d->SelectColorArray(property_name.c_str());
        q->m_mapper2d->SetScalarRange(range[0], range[1]);
        q->m_mapper2d->Modified();
    }

    if(q->m_view)
        q->m_view->interactor()->Render();
}

gnomonVtkDecoratorStreamTracer::gnomonVtkDecoratorStreamTracer(void)
{
    d = new gnomonVtkDecoratorStreamTracerPrivate();
    d->q = this;

    m_parameters["1_property_name"] = new dtk::d_inliststring("", {""}, "Mesh attribute to be displayed");
    m_parameters["2_alpha"] = new dtk::d_real("alpha", 1, 0, 1, 2, "Transparency value for the mesh rendering");
    m_parameters["3_integration_dir"] = new dtk::d_inliststring("Direction", {"Forward", "Backward", "Both"}, "Integration direction");
    m_parameters["4_integrator_type"] = new dtk::d_inliststring("Integration Type", {"RK 2", "RK 4", "RK 4-5"}, "Choose the Integrator type: Runge-Kutta 2, Runge-Kutta 4 or Runge-Kutta 4-5");
    m_parameters["5_seed_type"] = new dtk::d_inliststring("Seed Type", {"Sphere", "Line"}, "Seed type"); //seed type List(line,point cloud)
    //m_parameters["5_max_streamline"] = new dtk::d_real("Max nb of Streamline", d->default_nb_isolines, 2, 1000, "number of isolines to draw");
    m_parameters["6_colormap"] = new gnomonCoreParameterColorMap("colormap", "temperature", "Colormap to apply to the streamlines");

    m_parameters["1_property_name"]->connect([=] (QVariant v) {
        this->m_is_decorating = false;
        if(!m_grid)
            return;

        //1 get array
        d->property_name = v.value<dtk::d_inliststring>().value().toStdString();
        vtkDataArray *array = m_grid->GetPointData()->GetArray(d->property_name.c_str());
        if(array) {
            d->streamtracer->SetInputArrayToProcess(0, 0, 0,
                                                    vtkDataObject::FIELD_ASSOCIATION_POINTS,
                                                    d->property_name.c_str());
        } else {
            array = m_grid->GetCellData()->GetArray(d->property_name.c_str());
            if(array) {
                d->streamtracer->SetInputArrayToProcess(0, 0, 0,
                                                        vtkDataObject::FIELD_ASSOCIATION_CELLS,
                                                        d->property_name.c_str());
            } else {
                dtkWarn() << Q_FUNC_INFO << "cannot get array " << v.value<dtk::d_inliststring>().value();
                return;
            }
        }
        this->m_is_decorating = true;
        d->streamtracer->Modified();
        d->updateColorFunction();
        if(m_view)
            m_view->interactor()->Render();
    });

    m_parameters["2_alpha"]->connect([=] (QVariant v) {
        double alpha = v.value<dtk::d_real>().value();
        if(m_actor3d) {
            m_actor3d->GetProperty()->SetOpacity(alpha);
        }
        if(m_actor2d) {
            m_actor2d->GetProperty()->SetOpacity(alpha);
        }
    });

    m_parameters["3_integration_dir"]->connect([=] (QVariant v) {
        this->m_is_decorating = false;
        if(!m_grid)
            return;

        int idx = v.value<dtk::d_inliststring>().valueIndex();
        d->streamtracer->SetIntegrationDirection(idx);
        d->streamtracer->Modified();
        if(m_view)
            m_view->interactor()->Render();
    });

    m_parameters["4_integrator_type"]->connect([=] (QVariant v) {
        this->m_is_decorating = false;
        if(!m_grid)
            return;

        int idx = v.value<dtk::d_inliststring>().valueIndex();
        d->streamtracer->SetIntegratorType(idx);
        d->streamtracer->Modified();
        if(m_view)
            m_view->interactor()->Render();
    });

    m_parameters["5_seed_type"]->connect([=] (QVariant v) {
        this->m_is_decorating = false;
        if(!m_grid)
            return;

        QString seed = v.value<dtk::d_inliststring>().value();
        if(seed == "Sphere") {
            d->source_line_widget->Off();
            d->source_sphere_widget->On();
            d->source_sphere->GetPolyData(d->source_data);
        } else {
            d->source_line_widget->On();
            d->source_sphere_widget->Off();
            d->source_line->GetPolyData(d->source_data);
        }

        /*
        if (is_sphere) {
        source_sphere_observer->Execute(source_sphere_widget, vtkCommand::InteractionEvent, nullptr);
    } else {
        source_line_observer->Execute(source_line_widget, vtkCommand::InteractionEvent, nullptr);
    }
        */

        d->streamtracer->SetSourceData(d->source_data);
        d->streamtracer->Modified();
        if(this->m_view)
            this->m_view->interactor()->Render();

    });

    m_parameters["6_colormap"]->connect([=] (QVariant v) {
        d->updateColorFunction();
    });


}

gnomonVtkDecoratorStreamTracer::~gnomonVtkDecoratorStreamTracer(void)
{
    this->unsetView();
    delete d;
}

void gnomonVtkDecoratorStreamTracer::setGrid(vtkSmartPointer<vtkUnstructuredGrid> grid)
{
    //remove actor if existing
    //to check, is it necessary?
    if(m_view) {
        if(m_actor3d) m_view->renderer3D()->RemoveActor(m_actor3d);
        if(m_actor2d)  m_view->renderer2D()->RemoveActor(m_actor2d);
    }

    m_grid = grid;
    double bounds[6];
    m_grid->GetBounds(bounds);
    vtkBoundingBox box;
    box.SetBounds(bounds);
    double center[3]; box.GetCenter(center);
    double pos1[3]; pos1[0] = bounds[0]; pos1[1] = bounds[2]; pos1[2] = bounds[4];
    double pos2[3]; pos2[0] = bounds[1]; pos2[1] = bounds[3]; pos2[2] = bounds[5];

    d->source_data = vtkSmartPointer<vtkPolyData>::New();

    d->source_line = vtkSmartPointer<vtkLineRepresentation>::New();
    d->source_line->SetResolution(50);
    d->source_line->SetPoint1WorldPosition(pos1);
    d->source_line->SetPoint2WorldPosition(pos2);

    d->source_line->GetPolyData(d->source_data);
    d->source_line_widget = vtkSmartPointer<vtkLineWidget2>::New();
    d->source_line_widget->SetEnabled(false);
    d->source_line_widget->SetRepresentation(d->source_line);

    d->source_sphere = vtkSmartPointer<vtkSphereRepresentation>::New();
    d->source_sphere->SetPhiResolution(7);
    d->source_sphere->SetThetaResolution(7);
    d->source_sphere->SetRadius(1.0);
    d->source_sphere->SetRepresentationToWireframe();
    d->source_sphere->RadialLineOff();
    d->source_sphere->SetCenter(center);
    d->source_sphere->SetRadius(box.GetDiagonalLength()/20.0);
    d->source_sphere->Modified();
    d->source_sphere->GetPolyData(d->source_data);

    d->source_sphere_widget = vtkSmartPointer<vtkSphereWidget2>::New();
    d->source_sphere_widget->SetEnabled(false);
    d->source_sphere_widget->SetRepresentation(d->source_sphere);

    if(m_view) {
        d->source_line_widget->SetInteractor(m_view->interactor());
        d->source_sphere_widget->SetInteractor(m_view->interactor());
    }

    d->source_line_widget->Off();
    d->source_sphere_widget->On();



    d->streamtracer = vtkSmartPointer<vtkStreamTracer>::New();
    d->streamtracer->SetInputData(m_grid);
    d->streamtracer->SetSourceData(d->source_data);
    d->streamtracer->SetInitialIntegrationStep(0.1);
    d->streamtracer->SetIntegrationDirectionToBoth();

    d->streamtracer->SetMaximumPropagation(box.GetDiagonalLength());
    d->streamtracer->SetMaximumNumberOfSteps(2000);
    d->streamtracer->SetMaximumPropagation(20); // To be set as the maximum size of the dataset
    d->streamtracer->SetTerminalSpeed(1.e-12);
    d->streamtracer->SetMaximumError(1.e-6);
    d->streamtracer->SetComputeVorticity(true);

    //observers
    d->source_sphere_observer = vtkSmartPointer<gnomonStreamTracerSphereObserver>::New();
    d->source_sphere_observer->source_data = d->source_data;
    d->source_sphere_observer->source_sphere = d->source_sphere;
    d->source_sphere_observer->stream_tracer = d->streamtracer;

    d->source_line_observer = vtkSmartPointer<gnomonStreamTracerLineObserver>::New();
    d->source_line_observer->source_data = d->source_data;
    d->source_line_observer->source_line = d->source_line;
    d->source_line_observer->stream_tracer = d->streamtracer;
    d->source_line_widget->AddObserver(vtkCommand::InteractionEvent, d->source_line_observer);
    d->source_sphere_widget->AddObserver(vtkCommand::InteractionEvent, d->source_sphere_observer);


    //check param
    dtk::d_inliststring *propertyParam = (dtk::d_inliststring *)m_parameters["1_property_name"];
    QStringList properties = {};
    for(int i=0; i < m_grid->GetPointData()->GetNumberOfArrays(); ++i) {
        if(m_grid->GetPointData()->GetArray(i)->GetNumberOfComponents() == 3) {
            properties.append(m_grid->GetPointData()->GetArrayName(i));
        }
    }

    for(int i=0; i < m_grid->GetCellData()->GetNumberOfArrays(); ++i) {
        if(m_grid->GetPointData()->GetArray(i)->GetNumberOfComponents() == 3) {
            properties.append(m_grid->GetCellData()->GetArrayName(i));
        }
    }

    QString old_property_name = propertyParam->value();

    propertyParam->setValues(properties);
    this->m_is_decorating = !properties.isEmpty();

    if (!old_property_name.isEmpty() && properties.contains(old_property_name)) {
        propertyParam->setValue(old_property_name);
    } else {
        if(properties.isEmpty()) {
            propertyParam->setValues({""});
            propertyParam->setValue(QString(""));
        } else {
            propertyParam->setValueIndex(0);
        }
    }
    d->property_name = propertyParam->value().toStdString();
    d->streamtracer->Modified();


    //d->tube_filter = vtkSmartPointer<vtkTubeFilter>::New();
    //d->tube_filter->SetInputConnection(d->streamtracer->GetOutputPort());
    //d->tube_filter->SetNumberOfSides(8);
    //d->tube_filter->SetRadius(.02);
    //d->tube_filter->SetVaryRadius(0);


    //create new mapper
    if(!m_mapper3d)
        m_mapper3d = vtkSmartPointer<vtkDataSetMapper>::New();
    if(!m_mapper2d)
        m_mapper2d = vtkSmartPointer<vtkDataSetMapper>::New();

    if(!d->colorFunction)
        d->colorFunction = vtkSmartPointer<vtkColorTransferFunction>::New();
    m_mapper3d->SetLookupTable(d->colorFunction);
    m_mapper2d->SetLookupTable(d->colorFunction);
    d->updateColorFunction();
    m_mapper3d->SetInputConnection(d->streamtracer->GetOutputPort());
    m_mapper2d->SetInputConnection(d->streamtracer->GetOutputPort());
    //m_mapper3d->SetInputConnection(d->tube_filter->GetOutputPort());
    //m_mapper2d->SetInputConnection(d->tube_filter->GetOutputPort());

    //set actor
    m_mapper3d->Update();
    m_mapper2d->Update();

    m_actor3d = vtkSmartPointer<vtkActor>::New();
    m_actor3d->SetMapper(m_mapper3d);
    m_actor3d->VisibilityOn();

    m_actor2d = vtkSmartPointer<vtkActor>::New();
    m_actor2d->SetMapper(m_mapper2d);

    if(m_view && m_is_decorating) {
        m_view->renderer3D()->AddActor(m_actor3d);
        m_view->renderer3D()->ResetCamera();
        m_view->renderer2D()->AddActor(m_actor2d);
        m_view->renderer2D()->ResetCamera();
        m_view->interactor()->Render();
    }
}

void gnomonVtkDecoratorStreamTracer::setView(gnomonVtkView *view)
{
    gnomonVtkDecorator::setView(view);

    if(d->source_line_widget) {
        d->source_line_widget->SetInteractor(m_view->interactor());
        d->source_sphere_widget->SetInteractor(m_view->interactor());
    }
}
