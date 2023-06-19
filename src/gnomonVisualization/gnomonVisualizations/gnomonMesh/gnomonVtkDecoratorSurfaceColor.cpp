#include "gnomonVtkDecoratorSurfaceColor.h"

#include <dtkCoreParameterNumeric.h>
#include <dtkCoreParameters.h>

#include <qvariant.h>
#include <vtkAbstractArray.h>
#include <vtkAbstractMapper.h>
#include <vtkActor.h>
#include <vtkCellData.h>
#include <vtkColorTransferFunction.h>
#include <vtkDataArray.h>
#include <vtkDataSetMapper.h>
#include <vtkDoubleArray.h>
#include <vtkProperty.h>
#include <vtkPointData.h>
#include <vtkPoints.h>
#include <vtkPointSet.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkSmartPointerBase.h>
#include <vtkUnstructuredGrid.h>

#include <gnomonVisualization/gnomonCoreParameterColor.h>
#include "gnomonView/gnomonVtkView.h"
#include "gnomonVisualizations/gnomonMesh/gnomonVtkDecorator.h"

struct gnomonVtkDecoratorSurfaceColorPrivate {
    bool is_decorating = false;
    gnomonVtkView *view = nullptr;
    vtkSmartPointer<vtkUnstructuredGrid> grid = nullptr;
    dtkCoreParameters parameters;

    vtkSmartPointer<vtkActor> actor3d = nullptr;
    vtkSmartPointer<vtkDataSetMapper> mapper3d = nullptr;
    vtkSmartPointer<vtkActor> actor2d = nullptr;
    vtkSmartPointer<vtkDataSetMapper> mapper2d = nullptr;
    vtkSmartPointer<vtkColorTransferFunction> colorFunction;

    void updateValueRange(void);
    void updateColorFunction(void);
};

void gnomonVtkDecoratorSurfaceColorPrivate::updateValueRange()
{
    QString property_name = ((dtk::d_inliststring *)this->parameters["1_property_name"])->value();

    if(!this->grid)
        return;

    vtkDataArray *array = this->grid->GetPointData()->GetArray(property_name.toStdString().c_str());
    if(array) {
        if(this->mapper3d) this->mapper3d->SetScalarModeToUsePointData();
        if(this->mapper2d) this->mapper2d->SetScalarModeToUsePointData();
        this->grid->GetPointData()->SetActiveScalars(property_name.toStdString().c_str());
    } else {
        array = this->grid->GetCellData()->GetArray(property_name.toStdString().c_str());
        if(array) this->grid->GetCellData()->SetActiveScalars(property_name.toStdString().c_str());
        if(this->mapper3d) this->mapper3d->SetScalarModeToUseCellData();
        if(this->mapper2d) this->mapper2d->SetScalarModeToUseCellData();
    }

    if(array) {
        is_decorating = true;
        double range[2];
        array->GetRange(range);
        ((dtk::d_range_real *)this->parameters["value_range"])->setBounds({range[0], range[1]});
        ((dtk::d_range_real *)this->parameters["value_range"])->setValue({range[0], range[1]});
        if(this->mapper3d) this->mapper3d->SetScalarRange(array->GetRange());
        if(this->mapper2d) this->mapper2d->SetScalarRange(array->GetRange());
        this->updateColorFunction();
    } else {
        is_decorating = false;
        dtkWarn() << Q_FUNC_INFO << "cannot get array " << property_name << " no updateValueRange";
        ((dtk::d_range_real *)this->parameters["value_range"])->setBounds({0., 0.});
    }
}

void gnomonVtkDecoratorSurfaceColorPrivate::updateColorFunction(void)
{
    if (!this->colorFunction)
        return;

    QMap<double, QColor> colormap = ((gnomonCoreParameterColorMap *)this->parameters["2_colormap"])->value();
    std::array<double, 2> value_range = ((dtk::d_range_real *)this->parameters["value_range"])->value();
    this->colorFunction->RemoveAllPoints();
    for (const auto& val : colormap.keys()) {
        double node = val*value_range[1] + (1-val)*value_range[0];
        double r = colormap[val].red()/255.;
        double g = colormap[val].green()/255.;
        double b = colormap[val].blue()/255.;
        this->colorFunction->AddRGBPoint(node, r, g, b);
    }

    this->colorFunction->ClampingOn();
    //this->colorFunction->Build();

    if(this->mapper3d)
        this->mapper3d->Update();

    if(this->mapper2d)
        this->mapper2d->Update();

    if(this->view)
        this->view->interactor()->Render();
}

gnomonVtkDecoratorSurfaceColor::gnomonVtkDecoratorSurfaceColor(void)
{
    d = new gnomonVtkDecoratorSurfaceColorPrivate();

    d->parameters["1_property_name"] = new dtk::d_inliststring("", {""}, "Mesh attribute to be displayed");
    d->parameters["value_range"] = new dtk::d_range_real("value_range", {0., 1.}, 0., 1., "Value range for color adjustment");
    d->parameters["2_colormap"] = new gnomonCoreParameterColorMap("colormap", "gray", "Colormap to apply to the mesh");
    d->parameters["alpha"] = new dtk::d_real("alpha", 1, 0, 1, 2, "Transparency value for the mesh rendering");

    d->parameters["alpha"]->connect([=] (QVariant v) {
        double alpha = v.value<dtk::d_real>().value();
        if(d->actor3d) {
            d->actor3d->GetProperty()->SetOpacity(alpha);
        }
        if(d->actor2d) {
            d->actor2d->GetProperty()->SetOpacity(alpha);
        }
    });

    d->parameters["2_colormap"]->connect([=] (QVariant v) {
        d->updateColorFunction();
    });

    d->parameters["1_property_name"]->connect([=] (QVariant v) {
        if(!d->grid)
            return;
        d->updateValueRange();
        //emit parametersChanged();
    });

}

gnomonVtkDecoratorSurfaceColor::~gnomonVtkDecoratorSurfaceColor(void)
{
    this->unsetView();
    delete d;
}

void gnomonVtkDecoratorSurfaceColor::AddActorIfPossible(void)
{
   if(d->view && d->is_decorating) {
       if(d->actor3d) d->view->renderer3D()->AddActor(d->actor3d);
       if(d->actor2d) d->view->renderer2D()->AddActor(d->actor2d);
   }
}

vtkSmartPointer<vtkActor> gnomonVtkDecoratorSurfaceColor::actor(void)
{
    return d->actor3d;
}

void gnomonVtkDecoratorSurfaceColor::removeActor(void)
{
    if(d->view) {
        if(d->actor3d) d->view->renderer3D()->RemoveActor(d->actor3d);
        if(d->actor2d) d->view->renderer2D()->RemoveActor(d->actor2d);
    }
}

void gnomonVtkDecoratorSurfaceColor::setVisibility(bool visible)
{
    if(d->view) {
        if(d->actor3d) d->actor3d->SetVisibility(visible);
        if(d->actor2d) d->actor2d->SetVisibility(visible);
    }
}


bool gnomonVtkDecoratorSurfaceColor::isDecorating()
{
    return d->is_decorating;
}

void gnomonVtkDecoratorSurfaceColor::setGrid(vtkSmartPointer<vtkUnstructuredGrid> grid)
{
    //remove actor if existing
    //to check, is it necessary?
    if(d->view) {
        if(d->actor3d) d->view->renderer3D()->RemoveActor(d->actor3d);
        if(d->actor2d)  d->view->renderer2D()->RemoveActor(d->actor2d);
    }

    d->grid = grid;

    //create new actor
    if(!d->mapper3d)
        d->mapper3d = vtkSmartPointer<vtkDataSetMapper>::New();
    if(!d->mapper2d)
        d->mapper2d = vtkSmartPointer<vtkDataSetMapper>::New();

    d->mapper3d->SetInputData(d->grid);
    d->mapper2d->SetInputData(d->grid);

    if(!d->colorFunction)
        d->colorFunction = vtkSmartPointer<vtkColorTransferFunction>::New();

    //check param
    dtk::d_inliststring *propertyParam = (dtk::d_inliststring *)d->parameters["1_property_name"];
    QStringList properties = {};
    for(int i=0; i < d->grid->GetPointData()->GetNumberOfArrays(); ++i) {
        properties.append(d->grid->GetPointData()->GetArrayName(i));
    }

    for(int i=0; i < d->grid->GetCellData()->GetNumberOfArrays(); ++i) {
        properties.append(d->grid->GetCellData()->GetArrayName(i));
    }
    QString old_property_name = ((dtk::d_inliststring *)d->parameters["1_property_name"])->value();

    propertyParam->setValues(properties);

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

    //set mapper
    d->updateValueRange();
    d->mapper3d->SetLookupTable(d->colorFunction);
    d->mapper3d->Update();
    d->mapper2d->SetLookupTable(d->colorFunction);
    d->mapper2d->Update();

    d->actor3d = vtkSmartPointer<vtkActor>::New();
    d->actor3d->SetMapper(d->mapper3d);
    d->actor3d->Modified();

    d->actor2d = vtkSmartPointer<vtkActor>::New();
    d->actor2d->SetMapper(d->mapper2d);
    d->actor2d->Modified();

    if(d->view && d->is_decorating) {
        d->view->renderer3D()->AddActor(d->actor3d);
        d->view->renderer3D()->ResetCamera();
        d->view->renderer2D()->AddActor(d->actor2d);
        d->view->renderer2D()->ResetCamera();
        d->view->interactor()->Render();
    }
}

void gnomonVtkDecoratorSurfaceColor::set2DClippingPlane(vtkSmartPointer<vtkPlane> plane)
{
    if(d->mapper2d) {
        d->mapper2d->RemoveAllClippingPlanes();
        d->mapper2d->AddClippingPlane(plane);
        d->mapper2d->Update();
    }
}

void gnomonVtkDecoratorSurfaceColor::setView(gnomonVtkView *view)
{
    d->view = view;
    if(d->is_decorating) {
        if(d->actor3d) d->view->renderer3D()->AddActor(d->actor3d);
        if(d->actor2d) d->view->renderer2D()->AddActor(d->actor2d);
    }
}

void gnomonVtkDecoratorSurfaceColor::unsetView()
{
    if(d->is_decorating) {
        if(d->actor3d) d->view->renderer3D()->RemoveActor(d->actor3d);
        if(d->actor2d) d->view->renderer2D()->RemoveActor(d->actor2d);
    }
    d->is_decorating = false;
    d->view = nullptr;
}

dtkCoreParameters gnomonVtkDecoratorSurfaceColor::parameters()
{
    return d->parameters;
}
