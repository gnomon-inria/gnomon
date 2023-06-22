#include "gnomonVtkDecoratorVectorGlyphs.h"

#include <dtkCoreParameterInList.h>
#include <dtkCoreParameterNumeric.h>
#include <dtkCoreParameters.h>

#include <qvariant.h>
#include <vtkAbstractArray.h>
#include <vtkArrowSource.h>
#include <vtkAbstractMapper.h>
#include <vtkActor.h>
#include <vtkCellCenters.h>
#include <vtkCellData.h>
#include <vtkColorTransferFunction.h>
#include <vtkDataArray.h>
#include <vtkDataSetMapper.h>
#include <vtkDoubleArray.h>
#include <vtkGlyph3D.h>
#include <vtkGlyph3DMapper.h>
#include <vtkMaskPoints.h>
#include <vtkProperty.h>
#include <vtkPointData.h>
#include <vtkPoints.h>
#include <vtkPointSet.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkSmartPointerBase.h>
#include <vtkUnstructuredGrid.h>


#include <vtkLabeledTreeMapDataMapper.h>
#include <vtkActor2D.h>

#include <gnomonVisualization/gnomonCoreParameterColor.h>
#include "gnomonView/gnomonVtkView.h"
#include "gnomonVisualizations/gnomonMesh/gnomonVtkDecorator.h"

struct gnomonVtkDecoratorVectorGlyphsPrivate {
    gnomonVtkDecoratorVectorGlyphs *q = nullptr;
    std::string current_property;

    vtkSmartPointer<vtkArrowSource> source_arrow;
    vtkSmartPointer<vtkCellCenters> cell_centers;
    vtkSmartPointer<vtkMaskPoints> mask_points;
    vtkSmartPointer<vtkGlyph3D> glyph3d = nullptr;
    vtkSmartPointer<vtkColorTransferFunction> colorFunction;

    void adjustGlyphSizeToDataSet(void);
    //void updateValueRange(void);
    void updateColorFunction(void);
};

void gnomonVtkDecoratorVectorGlyphsPrivate::adjustGlyphSizeToDataSet()
{
    if(!q->m_grid)
        return;

    double bounds[6];
    q->m_grid->GetBounds(bounds);
    double max_dim = 1.;
    double exponent = 1;
    if (std::abs(bounds[1] - bounds[0]) > 1.e-10) {
        max_dim = std::abs(bounds[1] - bounds[0]);
    }
    if (std::abs(bounds[3] - bounds[2]) > 1.e-10) {
        max_dim *= std::abs(bounds[3] - bounds[2]);
        exponent += 1;
    }
    if (std::abs(bounds[5] - bounds[4]) > 1.e-10) {
        max_dim *= std::abs(bounds[5] - bounds[4]);
        exponent += 1;
    }

    double measure = std::pow(max_dim / q->m_grid->GetNumberOfCells(), 1. / exponent);
    dynamic_cast<dtk::d_real*>(q->m_parameters["5_scale_factor"])->setValue(measure);
}

void gnomonVtkDecoratorVectorGlyphsPrivate::updateColorFunction(void)
{
    if (!this->colorFunction || !q->m_grid || !q->m_mapper3d)
        return;

    QMap<double, QColor> colormap = ((gnomonCoreParameterColorMap *)q->m_parameters["6_colormap"])->value();

    auto *array = q->m_grid->GetPointData()->GetArray(this->current_property.c_str());
    if(!array) {
        array = q->m_grid->GetCellData()->GetArray(this->current_property.c_str());
    }
    if(!array) {
        dtkWarn() << Q_FUNC_INFO << "Cannot get attribute " << this->current_property.c_str();
        return;
    }

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

    this->colorFunction->RemoveAllPoints();
    for (const auto& val : colormap.keys()) {
        double node = val*range[1] + (1-val)*range[0];
        double r = colormap[val].red()/255.;
        double g = colormap[val].green()/255.;
        double b = colormap[val].blue()/255.;
        this->colorFunction->AddRGBPoint(node, r, g, b);
    }

    this->colorFunction->ClampingOn();

    //if color by scalar
    //use  d->glyph3d->SetInputArrayToProcess(3, 0, 0, vtkDataObject::FIELD_ASSOCIATION_POINTS,
    //                                       scalar_array_name);
    // as color array
    // range = scalar_array->GetRange();
    // d->glyph3d->SetColorModeByScalar();

    q->m_mapper3d->SetLookupTable(this->colorFunction);
    q->m_mapper3d->SetScalarRange(range[0], range[1]);
    q->m_mapper3d->Modified();
    q->m_mapper3d->Update();

    if(q->m_view)
        q->m_view->interactor()->Render();
}


gnomonVtkDecoratorVectorGlyphs::gnomonVtkDecoratorVectorGlyphs(void)
{
    d = new gnomonVtkDecoratorVectorGlyphsPrivate();
    d->q = this;
    m_is_decorating = false;

    m_parameters["1_property_name"] = new dtk::d_inliststring("", {""}, "Mesh attribute to be displayed");
    m_parameters["2_alpha"] = new dtk::d_real("alpha", 1, 0, 1, 2, "Transparency value for the mesh rendering");
    //m_parameters["2_scale_mode"] = new dtk::d_inliststring("Scale Mode", "Scalar", {"Scalar", "Magnitude"}, "Scale Mode to be used for the glyphs"); // , "X", "Y", "Z"

    m_parameters["3_mask_ratio"] = new dtk::d_int("Mask Ratio", 1, 1, 100, "Turn on every nth point (strided sampling)");

    //d->parameters["3_sampling_mode"] = new dtk::d_inliststring("Sampling Mode", "No Scaling", {"No Scaling", "Magnitude", "X", "Y", "Z"}, "Mesh attribute to be displayed");
    //d->parameters["4_glyph_stride"] = new dtk::d_int("Stride", 2, "Stride of the glyph");
    m_parameters["5_scale_factor"] = new dtk::d_real("Scale Factor", 0.1, 0, 10, "Scale Factor");

    m_parameters["6_colormap"] = new gnomonCoreParameterColorMap("colormap", "gray", "Colormap to apply to the mesh");
    //m_parameters["7_color_array"] = new dtk::d_inliststring("Color by", "Magnitude", {"Magnitude", "X", "Y", "Z"}, "what to use to color the arrow?");
    //if glyph3d->SetColorModeToCOlorByScalar, will use the scalar array provided as
    //         d->glyph3d->SetInputArrayToProcess(3, 0, 0, vtkDataObject::FIELD_ASSOCIATION_POINTS,
    //                                        d->current_property.c_str());
    // if colorByVector
    //((this->VectorMode == VTK_USE_VECTOR && inVectors != nullptr) ||
    //

    m_parameters["1_property_name"]->connect([=] (QVariant v) {
        if(!m_grid || !m_mapper3d)
            return;

        d->current_property = v.value<dtk::d_inliststring>().value().toStdString();
        auto *array = m_grid->GetPointData()->GetArray(d->current_property.c_str());
        if(array) {
            m_grid->GetPointData()->SetActiveVectors(d->current_property.c_str());
            d->mask_points->SetInputData(m_grid);
        } else {
            array = m_grid->GetCellData()->GetArray(d->current_property.c_str());
            if(array) {
                d->cell_centers->SetInputArrayToProcess(0, 0, 0,
                                                    vtkDataObject::FIELD_ASSOCIATION_CELLS,
                                                    d->current_property.c_str());
                d->mask_points->SetInputConnection(d->cell_centers->GetOutputPort());
            } else {
                dtkWarn() << Q_FUNC_INFO
                          << "cannot get array : "
                          << v.value<dtk::d_inliststring>().value();
                return;
            }
        }

        d->mask_points->SetInputArrayToProcess(0, 0, 0,
                                               vtkDataObject::FIELD_ASSOCIATION_POINTS,
                                               d->current_property.c_str());

        d->mask_points->Modified();
        d->glyph3d->SetInputArrayToProcess(1, 0, 0, vtkDataObject::FIELD_ASSOCIATION_POINTS,
                                           d->current_property.c_str());
        d->updateColorFunction();
        m_mapper3d->Modified();
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

    //not usefull for now
    /*
    m_parameters["2_scale_mode"]->connect([=] (QVariant v) {
        auto scaling_mode = v.value<dtk::d_inliststring>().valueIndex();
        if(d->glyph3d) {
            //scale_by_scalar = 0 , by_vector = 1, by_vector_component = 2
            d->glyph3d->SetScaleMode(scaling_mode < 2 ? scaling_mode : 2);
            //if (scaling_mode > 1) {
            //    d->glyph3d->SetArrayComponent(scaling_mode - 2);
            //}
        }
    });
    */

    m_parameters["3_mask_ratio"]->connect([=] (QVariant v) {
        int value = v.value<dtk::d_int>().value();
        d->mask_points->SetOnRatio(value);
    });

    //d->parameters["4_glyph_stride"]->connect([=] (QVariant v) {
    //    d->mask_points->SetOnRatio(mask_ratio);
    //});
    m_parameters["5_scale_factor"]->connect([=] (QVariant v) {
        double value = v.value<dtk::d_real>().value();
        if(m_mapper3d)
            d->glyph3d->SetScaleFactor(value);
    });

    m_parameters["6_colormap"]->connect([=] (QVariant v) {
        d->updateColorFunction();
    });
}

gnomonVtkDecoratorVectorGlyphs::~gnomonVtkDecoratorVectorGlyphs(void)
{
    this->unsetView();
    delete d;
}

void gnomonVtkDecoratorVectorGlyphs::setGrid(vtkSmartPointer<vtkUnstructuredGrid> grid)
{
    //remove actor if existing
    //to check, is it necessary?
    if(m_view) {
        if(m_actor3d) m_view->renderer3D()->RemoveActor(m_actor3d);
        if(m_actor2d) m_view->renderer2D()->RemoveActor(m_actor2d);
    }

    m_grid = grid;

    d->adjustGlyphSizeToDataSet();

    //create vtk source, arrows, glyph..
    d->cell_centers = vtkSmartPointer<vtkCellCenters>::New();
    d->cell_centers->SetInputData(m_grid);

    d->mask_points = vtkSmartPointer<vtkMaskPoints>::New();
    d->mask_points->RandomModeOff();
    d->mask_points->SetOnRatio(1); // 1 => all points are taken then no masking
    d->mask_points->SetInputData(m_grid);
    d->mask_points->GenerateVerticesOn(); // For Debug
    d->mask_points->Update();

    d->source_arrow = vtkSmartPointer<vtkArrowSource>::New();
    d->glyph3d = vtkSmartPointer<vtkGlyph3D>::New();
    d->glyph3d->SetInputConnection(d->mask_points->GetOutputPort());
    d->glyph3d->SetSourceConnection(d->source_arrow->GetOutputPort());
    d->glyph3d->SetScaleModeToScaleByVector();
    d->glyph3d->SetVectorModeToUseVector();
    d->glyph3d->SetColorModeToColorByVector();
    d->glyph3d->ScalingOn();
    d->glyph3d->OrientOn();
    d->glyph3d->SetScaleFactor(((dtk::d_real *)m_parameters["5_scale_factor"])->value());
    d->glyph3d->Update();

    //create mapper and actor
    if(!d->colorFunction)
        d->colorFunction = vtkSmartPointer<vtkColorTransferFunction>::New();

    m_mapper3d = vtkSmartPointer<vtkDataSetMapper>::New();
    m_mapper3d->SetInputConnection(d->glyph3d->GetOutputPort());

    //set first field
    dtk::d_inliststring *propertyParam = (dtk::d_inliststring *)m_parameters["1_property_name"];
    QStringList properties = {};
    for(int i=0; i < m_grid->GetPointData()->GetNumberOfArrays(); ++i) {
        if(m_grid->GetPointData()->GetArray(i)->GetNumberOfComponents() == 3) {
            properties.append(m_grid->GetPointData()->GetArrayName(i));
        }
    }

    for(int i=0; i < m_grid->GetCellData()->GetNumberOfArrays(); ++i) {
        if(m_grid->GetCellData()->GetArray(i)->GetNumberOfComponents() == 3) {
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
    d->current_property = propertyParam->value().toStdString();

    //set mapper
    m_mapper3d->SetLookupTable(d->colorFunction);
    //d->mapper3d->Update();
    //d->mapper2d->SetLookupTable(d->colorFunction);
    //d->mapper2d->Update();

    m_actor3d = vtkSmartPointer<vtkActor>::New();
    m_actor3d->SetMapper(m_mapper3d);
    m_actor3d->Modified();

    //d->actor2d = vtkSmartPointer<vtkActor>::New();
    //d->actor2d->SetMapper(d->mapper2d);
    //d->actor2d->Modified();

    if(m_view && m_is_decorating) {
        m_view->renderer3D()->AddActor(m_actor3d);
        m_view->renderer3D()->ResetCamera();
        //d->view->renderer2D()->AddActor(d->actor2d);
        m_view->renderer2D()->ResetCamera();
        m_view->interactor()->Render();
    }
}
