#include "gnomonVtkDecoratorSurfaceColor.h"

#include <dtkCoreParameterNumeric.h>
#include <dtkCoreParameters.h>

#include <qvariant.h>
#include <vtkAbstractArray.h>
#include <vtkAbstractMapper.h>
#include <vtkActor.h>
#include <vtkCellData.h>
#include <vtkColorTransferFunction.h>
#include <vtkCutter.h>
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
    gnomonVtkDecoratorSurfaceColor *q = nullptr;

    vtkSmartPointer<vtkColorTransferFunction> colorFunction;
    vtkSmartPointer<vtkCutter> cutter = nullptr;

    void updateValueRange(void);
    void updateColorFunction(void);
};

void gnomonVtkDecoratorSurfaceColorPrivate::updateValueRange()
{
    QString property_name = ((dtk::d_inliststring *)q->m_parameters["1_property_name"])->value();

    if(!q->m_grid)
        return;

    vtkDataArray *array = q->m_grid->GetPointData()->GetArray(property_name.toStdString().c_str());
    if(array) {
        if(q->m_mapper3d) q->m_mapper3d->SetScalarModeToUsePointData();
        if(q->m_mapper2d) q->m_mapper2d->SetScalarModeToUsePointData();
        q->m_grid->GetPointData()->SetActiveScalars(property_name.toStdString().c_str());
    } else {
        array = q->m_grid->GetCellData()->GetArray(property_name.toStdString().c_str());
        if(array) q->m_grid->GetCellData()->SetActiveScalars(property_name.toStdString().c_str());
        if(q->m_mapper3d) q->m_mapper3d->SetScalarModeToUseCellData();
        if(q->m_mapper2d) q->m_mapper2d->SetScalarModeToUseCellData();
    }

    if(array) {
        q->m_is_decorating = true;
        double range[2];
        array->GetRange(range);
        ((dtk::d_range_real *)q->m_parameters["value_range"])->setBounds({range[0], range[1]});
        ((dtk::d_range_real *)q->m_parameters["value_range"])->setValue({range[0], range[1]});
        if(q->m_mapper3d) q->m_mapper3d->SetScalarRange(array->GetRange());
        if(q->m_mapper2d) q->m_mapper2d->SetScalarRange(array->GetRange());
        this->updateColorFunction();
    } else {
        q->m_is_decorating = false;
        dtkWarn() << Q_FUNC_INFO << "cannot get array " << property_name << " no updateValueRange";
        ((dtk::d_range_real *)q->m_parameters["value_range"])->setBounds({0., 0.});
    }
}

void gnomonVtkDecoratorSurfaceColorPrivate::updateColorFunction(void)
{
    if (!this->colorFunction)
        return;

    QMap<double, QColor> colormap = ((gnomonCoreParameterColorMap *)q->m_parameters["2_colormap"])->value();
    std::array<double, 2> value_range = ((dtk::d_range_real *)q->m_parameters["value_range"])->value();
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

    if(q->m_mapper3d)
        q->m_mapper3d->Update();

    if(q->m_mapper2d)
        q->m_mapper2d->Update();

    if(q->m_view)
        q->m_view->interactor()->Render();
}

gnomonVtkDecoratorSurfaceColor::gnomonVtkDecoratorSurfaceColor(void)
{
    d = new gnomonVtkDecoratorSurfaceColorPrivate();
    d->q = this;

    m_parameters["1_property_name"] = new dtk::d_inliststring("", {""}, "Mesh attribute to be displayed");
    m_parameters["value_range"] = new dtk::d_range_real("value_range", {0., 1.}, 0., 1., "Value range for color adjustment");
    m_parameters["2_colormap"] = new gnomonCoreParameterColorMap("colormap", "temperature", "Colormap to apply to the mesh");
    m_parameters["alpha"] = new dtk::d_real("alpha", 1, 0, 1, 2, "Transparency value for the mesh rendering");

    m_parameters["alpha"]->connect([=] (QVariant v) {
        double alpha = v.value<dtk::d_real>().value();
        if(m_actor3d) {
            m_actor3d->GetProperty()->SetOpacity(alpha);
        }
        if(m_actor2d) {
            m_actor2d->GetProperty()->SetOpacity(alpha);
        }
    });

    m_parameters["2_colormap"]->connect([=] (QVariant v) {
        d->updateColorFunction();
    });

    m_parameters["1_property_name"]->connect([=] (QVariant v) {
        if(!m_grid)
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

void gnomonVtkDecoratorSurfaceColor::setGrid(vtkSmartPointer<vtkUnstructuredGrid> grid)
{
    //remove actor if existing
    //to check, is it necessary?
    if(m_view) {
        if(m_actor3d) m_view->renderer3D()->RemoveActor(m_actor3d);
        if(m_actor2d)  m_view->renderer2D()->RemoveActor(m_actor2d);
    }

    m_grid = grid;

    //check param
    dtk::d_inliststring *propertyParam = (dtk::d_inliststring *)m_parameters["1_property_name"];
    QStringList properties = {};
    for(int i=0; i < m_grid->GetPointData()->GetNumberOfArrays(); ++i) {
        properties.append(m_grid->GetPointData()->GetArrayName(i));
    }

    for(int i=0; i < m_grid->GetCellData()->GetNumberOfArrays(); ++i) {
        properties.append(m_grid->GetCellData()->GetArrayName(i));
    }
    QString old_property_name = ((dtk::d_inliststring *)m_parameters["1_property_name"])->value();

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
    if(!m_mapper3d)
        m_mapper3d = vtkSmartPointer<vtkDataSetMapper>::New();
    if(!m_mapper2d)
        m_mapper2d = vtkSmartPointer<vtkDataSetMapper>::New();
    if(!d->colorFunction)
        d->colorFunction = vtkSmartPointer<vtkColorTransferFunction>::New();

    m_mapper3d->SetInputData(m_grid);
    m_mapper3d->SetLookupTable(d->colorFunction);
    m_mapper3d->Update();

    if(m_clippingPlane) {
        d->cutter = vtkSmartPointer<vtkCutter>::New();
        d->cutter->SetCutFunction(m_clippingPlane);
        d->cutter->SetInputData(m_grid);

        m_mapper2d->SetInputConnection(d->cutter->GetOutputPort());

        m_mapper2d->SetLookupTable(d->colorFunction);
        m_mapper2d->Update();
    }

    d->updateValueRange();

    //create new actor
    m_actor3d = vtkSmartPointer<vtkActor>::New();
    m_actor3d->SetMapper(m_mapper3d);
    m_actor3d->Modified();

    m_actor2d = vtkSmartPointer<vtkActor>::New();
    m_actor2d->SetMapper(m_mapper2d);
    m_actor2d->Modified();

    if(m_view && m_is_decorating) {
        m_view->renderer3D()->AddActor(m_actor3d);
        m_view->renderer3D()->ResetCamera();
        m_view->renderer2D()->AddActor(m_actor2d);
        m_view->renderer2D()->ResetCamera();
        m_view->interactor()->Render();
    }
}
