#include "gnomonVtkDecoratorIsoContours.h"

#include <dtkCoreParameterInList.h>
#include <dtkCoreParameterNumeric.h>
#include <dtkCoreParameters.h>

#include <dtkLog.h>
#include <vtkActor.h>
#include <vtkCellDataToPointData.h>
#include <vtkCellData.h>
#include <vtkColorTransferFunction.h>
#include <vtkContourFilter.h>
#include <vtkDataObject.h>
#include <vtkDataSet.h>
#include <vtkDataSetMapper.h>
#include <vtkLookupTable.h>
#include <vtkPiecewiseFunction.h>
#include <vtkPointData.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkUnstructuredGrid.h>

#include <gnomonVisualization/gnomonCoreParameterColor.h>

struct gnomonVtkDecoratorIsoContoursPrivate {
    gnomonVtkDecoratorIsoContours *q = nullptr;

    std::string property_name = "";
    int default_nb_isolines = 10;
    QHash<QString, std::size_t> isolines_counts; //count per array
    vtkSmartPointer<vtkColorTransferFunction> colorFunction = nullptr;
    vtkSmartPointer<vtkContourFilter> isolines = nullptr;
    vtkSmartPointer<vtkCellDataToPointData> c2p_filter = nullptr;

    void updateColorFunction(void);
};

void gnomonVtkDecoratorIsoContoursPrivate::updateColorFunction(void)
{
    if (!this->colorFunction || !q->m_grid)
        return;

    QMap<double, QColor> colormap = ((gnomonCoreParameterColorMap *)q->m_parameters["4_colormap"])->value();

    auto&& range = ((dtk::d_range_real *)q->m_parameters["5_value_range"])->value();

    this->colorFunction->RemoveAllPoints();
    for (const auto& val : colormap.keys()) {
        double node = val*range[1] + (1-val)*range[0];
        double r = colormap[val].red()/255.;
        double g = colormap[val].green()/255.;
        double b = colormap[val].blue()/255.;
        this->colorFunction->AddRGBPoint(node, r, g, b);
    }

    this->colorFunction->ClampingOn();
    if(q->m_mapper3d) {
        q->m_mapper3d->Modified();
    }
}

gnomonVtkDecoratorIsoContours::gnomonVtkDecoratorIsoContours(void)
{
    d = new gnomonVtkDecoratorIsoContoursPrivate();
    d->q = this;

    m_parameters["1_property_name"] = new dtk::d_inliststring("", {""}, "Mesh attribute to be displayed");
    m_parameters["2_alpha"] = new dtk::d_real("alpha", 1, 0, 1, 2, "Transparency value for the mesh rendering");
    m_parameters["3_count"] = new dtk::d_int("Nb lines", d->default_nb_isolines, 2, 1000, "number of isolines to draw");
    m_parameters["4_colormap"] = new gnomonCoreParameterColorMap("colormap", "gray", "Colormap to apply to the mesh");
    m_parameters["5_value_range"] = new dtk::d_range_real("value_range", {0., 1.}, -100000000., 1000000., "Value range for color adjustment");

    m_parameters["1_property_name"]->connect([=] (QVariant v) {
        this->m_is_decorating = false;
        if(!m_grid)
            return;

        //1 get array
        d->property_name = v.value<dtk::d_inliststring>().value().toStdString();
        vtkDataArray *array = m_grid->GetPointData()->GetArray(d->property_name.c_str());
        if(array) {
            d->isolines->SetInputData(m_grid);
            d->isolines->SetInputArrayToProcess(0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_POINTS, d->property_name.c_str());

        } else {
            array = m_grid->GetCellData()->GetArray(d->property_name.c_str());
            if(array) {
                d->c2p_filter->SetInputArrayToProcess(0, 0, 0,
                                                      vtkDataObject::FIELD_ASSOCIATION_CELLS,
                                                      d->property_name.c_str());
                d->isolines->SetInputConnection(d->c2p_filter->GetOutputPort());
                d->isolines->SetInputArrayToProcess(0, 0, 0,
                                                    vtkDataObject::FIELD_ASSOCIATION_POINTS,
                                                    d->property_name.c_str());
                d->c2p_filter->Modified();
            } else {
                dtkWarn() << Q_FUNC_INFO << "cannot get array " << v.value<dtk::d_inliststring>().value();
                return;
            }
        }

        // 2 set range
        double range[2];
        array->GetRange(range);
        //will trigger parameter->connect method
        ((dtk::d_range_real *)m_parameters["5_value_range"])->setBounds({range[0], range[1]});
        ((dtk::d_range_real *)m_parameters["5_value_range"])->setValue({range[0], range[1]});

        auto count = d->isolines_counts[v.value<dtk::d_inliststring>().value()];
        ((dtk::d_int *)m_parameters["3_count"])->setValue(count);
        d->isolines->GenerateValues(count, range[0], range[1]);
        d->isolines->Modified();

        // 3 set colorarray
        if(m_mapper3d) {
            m_mapper3d->SelectColorArray(d->property_name.c_str());
            m_mapper2d->SelectColorArray(d->property_name.c_str());
            d->updateColorFunction();
        }

        m_view->interactor()->Render();
        m_is_decorating = true;
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

    m_parameters["3_count"]->connect([=] (QVariant v) {
        if(!m_grid)
            return;

        int count = v.value<dtk::d_int>().value();
        d->isolines_counts[((dtk::d_inliststring *)m_parameters["1_property_name"])->value()] = count;
        auto&& range = ((dtk::d_range_real *)m_parameters["5_value_range"])->value();
        d->isolines->GenerateValues(count, range[0], range[1]);
        d->isolines->Modified();

    });

    m_parameters["4_colormap"]->connect([=] (QVariant v) {
        d->updateColorFunction();
    });

    m_parameters["5_value_range"]->connect([=] (QVariant v) {
        if(!m_grid)
            return;

        auto&& range = v.value<dtk::d_range_real>().value();
        int count = ((dtk::d_int *)m_parameters["3_count"])->value();
        if(m_mapper3d) m_mapper3d->SetScalarRange(range[0], range[1]);
        if(m_mapper2d) m_mapper2d->SetScalarRange(range[0], range[1]);
        d->isolines->GenerateValues(count, range[0], range[1]);
        d->updateColorFunction();
    });
}


gnomonVtkDecoratorIsoContours::~gnomonVtkDecoratorIsoContours(void)
{
    this->unsetView();
    delete d;
}

void gnomonVtkDecoratorIsoContours::setGrid(vtkSmartPointer<vtkUnstructuredGrid> grid)
{
    //remove actor if existing
    //to check, is it necessary?
    if(m_view) {
        if(m_actor3d) m_view->renderer3D()->RemoveActor(m_actor3d);
        if(m_actor2d)  m_view->renderer2D()->RemoveActor(m_actor2d);
    }

    m_grid = grid;

    d->isolines = vtkSmartPointer<vtkContourFilter>::New();
    d->c2p_filter = vtkSmartPointer<vtkCellDataToPointData>::New();
    d->c2p_filter->SetInputData(m_grid);
    d->isolines_counts.clear();

    //create new actor
    if(!m_mapper3d)
        m_mapper3d = vtkSmartPointer<vtkDataSetMapper>::New();
    if(!m_mapper2d)
        m_mapper2d = vtkSmartPointer<vtkDataSetMapper>::New();

    m_mapper3d->SetInputConnection(d->isolines->GetOutputPort());
    m_mapper3d->SetColorModeToMapScalars();
    m_mapper3d->SetScalarVisibility(true);
    m_mapper2d->SetInputConnection(d->isolines->GetOutputPort());
    m_mapper2d->SetColorModeToMapScalars();
    m_mapper2d->SetScalarVisibility(true);

    if(!d->colorFunction)
        d->colorFunction = vtkSmartPointer<vtkColorTransferFunction>::New();
    m_mapper3d->SetLookupTable(d->colorFunction);
    m_mapper2d->SetLookupTable(d->colorFunction);

    //check param
    dtk::d_inliststring *propertyParam = (dtk::d_inliststring *)m_parameters["1_property_name"];
    QStringList properties = {};
    for(int i=0; i < m_grid->GetPointData()->GetNumberOfArrays(); ++i) {
        properties.append(m_grid->GetPointData()->GetArrayName(i));
        d->isolines_counts[m_grid->GetPointData()->GetArrayName(i)] = d->default_nb_isolines;
    }

    for(int i=0; i < m_grid->GetCellData()->GetNumberOfArrays(); ++i) {
        properties.append(m_grid->GetCellData()->GetArrayName(i));
        d->isolines_counts[m_grid->GetCellData()->GetArrayName(i)] = d->default_nb_isolines;
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
    m_mapper3d->Update();
    m_mapper2d->Update();

    m_actor3d = vtkSmartPointer<vtkActor>::New();
    m_actor3d->SetMapper(m_mapper3d);

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
