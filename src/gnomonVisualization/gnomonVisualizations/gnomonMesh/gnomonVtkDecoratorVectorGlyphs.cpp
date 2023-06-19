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

#include <gnomonVisualization/gnomonCoreParameterColor.h>
#include "gnomonView/gnomonVtkView.h"
#include "gnomonVisualizations/gnomonMesh/gnomonVtkDecorator.h"

struct gnomonVtkDecoratorVectorGlyphsPrivate {
    bool is_decorating = false;
    gnomonVtkView *view = nullptr;
    vtkSmartPointer<vtkUnstructuredGrid> grid = nullptr;
    dtkCoreParameters parameters;
    std::string current_property;

    vtkSmartPointer<vtkArrowSource> source_arrow;
    vtkSmartPointer<vtkCellCenters> cell_centers;
    vtkSmartPointer<vtkMaskPoints> mask_points;

    vtkSmartPointer<vtkActor> actor3d = nullptr;
    //vtkSmartPointer<vtkGlyph3DMapper> mapper3d = nullptr;
    vtkSmartPointer<vtkGlyph3D> glyph3d = nullptr;
    vtkSmartPointer<vtkDataSetMapper> mapper3d = nullptr;
    vtkSmartPointer<vtkActor> actor2d = nullptr;
    vtkSmartPointer<vtkGlyph3DMapper> mapper2d = nullptr;
    vtkSmartPointer<vtkColorTransferFunction> colorFunction;

    void adjustGlyphSizeToDataSet(void);
    //void updateValueRange(void);
    void updateColorFunction(void);
};

void gnomonVtkDecoratorVectorGlyphsPrivate::adjustGlyphSizeToDataSet()
{
    if(!this->grid || !this->mapper3d)
        return;

    double bounds[6];
    grid->GetBounds(bounds);
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

    double measure = std::pow(max_dim / this->grid->GetNumberOfCells(), 1. / exponent);
    qDebug() << "NEW SCALE FACTOR" << measure;
    //dynamic_cast<dtk::d_real*>(this->parameters["5_scale_factor"])->setValue(measure);
}

/*
void gnomonVtkDecoratorVectorGlyphsPrivate::updateValueRange()
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
*/
void gnomonVtkDecoratorVectorGlyphsPrivate::updateColorFunction(void)
{
    if (!this->colorFunction || !this->grid)
        return;

    QMap<double, QColor> colormap = ((gnomonCoreParameterColorMap *)this->parameters["6_colormap"])->value();

    auto *array = this->grid->GetPointData()->GetArray(this->current_property.c_str());
    if(!array) {
        array = this->grid->GetCellData()->GetArray(this->current_property.c_str());
    }
    if(!array) {
        dtkWarn() << Q_FUNC_INFO << "Cannot get attribute " << this->current_property.c_str();
    }
    auto&& range = array->GetRange();

    this->colorFunction->RemoveAllPoints();
    for (const auto& val : colormap.keys()) {
        double node = val*range[1] + (1-val)*range[0];
        double r = colormap[val].red()/255.;
        double g = colormap[val].green()/255.;
        double b = colormap[val].blue()/255.;
        this->colorFunction->AddRGBPoint(node, r, g, b);
    }

    this->colorFunction->ClampingOn();
    //this->colorFunction->Build();

    this->colorFunction->SetVectorModeToMagnitude();
    /*
switch(component_id) {
        case -1:
            this->color_function->SetVectorModeToMagnitude();
            break;
        case 0:
            this->color_function->SetVectorModeToComponent();
            this->color_function->SetVectorComponent(0);
            break;
        case 1:
            this->color_function->SetVectorModeToComponent();
            this->color_function->SetVectorComponent(1);
            break;
        case 2:
            this->color_function->SetVectorModeToComponent();
            this->color_function->SetVectorComponent(2);
            break;

    */


    if(this->mapper3d) {
        this->mapper3d->SetLookupTable(this->colorFunction);
        this->mapper3d->SelectColorArray(this->current_property.c_str());
        this->mapper3d->SetScalarRange(range[0], range[1]);
        this->mapper3d->Modified();
        this->mapper3d->Update();
    }


    if(this->mapper2d)
        this->mapper2d->Update();

    if(this->view)
        this->view->interactor()->Render();
}


gnomonVtkDecoratorVectorGlyphs::gnomonVtkDecoratorVectorGlyphs(void)
{
    d = new gnomonVtkDecoratorVectorGlyphsPrivate();

    d->source_arrow = vtkSmartPointer<vtkArrowSource>::New();
    d->cell_centers = vtkSmartPointer<vtkCellCenters>::New();
    d->mask_points = vtkSmartPointer<vtkMaskPoints>::New();
    d->mask_points->RandomModeOff();
    d->mask_points->SetOnRatio(2); // 1 => all points are taken then no masking

    d->parameters["1_property_name"] = new dtk::d_inliststring("", {""}, "Mesh attribute to be displayed");
    d->parameters["2_scale_mode"] = new dtk::d_inliststring("Scale Mode", "No Scaling", {"No Scaling", "Magnitude", "X", "Y", "Z"}, "Scale Mode to be used for the glyphs");

    d->parameters["3_mask_ratio"] = new dtk::d_int("Mask Ratio", 1, 1., 100., "Turn on every nth point (strided sampling)");

    //d->parameters["3_sampling_mode"] = new dtk::d_inliststring("Sampling Mode", "No Scaling", {"No Scaling", "Magnitude", "X", "Y", "Z"}, "Mesh attribute to be displayed");
    //d->parameters["4_glyph_stride"] = new dtk::d_int("Stride", 2, "Stride of the glyph");
    d->parameters["5_scale_factor"] = new dtk::d_real("Scale Factor", 0.1, 0, 1000, "Scale Factor");

    d->parameters["6_colormap"] = new gnomonCoreParameterColorMap("colormap", "gray", "Colormap to apply to the mesh");

    //TODO opacity

    d->parameters["1_property_name"]->connect([=] (QVariant v) {
        if(!d->grid)
            return;

        d->current_property = v.value<dtk::d_inliststring>().value().toStdString();

        auto *array = d->grid->GetPointData()->GetArray(d->current_property.c_str());
        if(array) {
            d->mask_points->SetInputData(d->grid);
            d->grid->GetPointData()->SetActiveVectors(d->current_property.c_str());
            qDebug() << "POINT DATA" << array->GetRange()[0] <<  array->GetRange()[1] ;
            array->Print(std::cout);


            d->glyph3d->SetInputArrayToProcess(1, 0, 0, vtkDataObject::FIELD_ASSOCIATION_POINTS,
                                               d->current_property.c_str());
        } else {
            array = d->grid->GetCellData()->GetArray(d->current_property.c_str());
            d->grid->GetCellData()->SetActiveVectors(d->current_property.c_str());
            qDebug() << "CELL DATA"  << array->GetRange()[0] <<  array->GetRange()[1] ;
            array->Print(std::cout);
            d->cell_centers->SetInputArrayToProcess(0, 0, 0,
                                                    vtkDataObject::FIELD_ASSOCIATION_CELLS,
                                                    d->current_property.c_str());
            d->mask_points->SetInputConnection(d->cell_centers->GetOutputPort());

            d->glyph3d->SetInputArrayToProcess(1, 0, 0, vtkDataObject::FIELD_ASSOCIATION_CELLS,
                                               d->current_property.c_str());
        }

        d->mask_points->SetInputArrayToProcess(0, 0, 0,
                                               vtkDataObject::FIELD_ASSOCIATION_POINTS,
                                               d->current_property.c_str());
        d->mask_points->Modified();
        d->adjustGlyphSizeToDataSet();
        d->updateColorFunction();


        //d->mapper3d->SetOrientationArray(d->current_property.c_str());
        //d->mapper3d->SetScaleArray(d->current_property.c_str());
        d->mapper3d->Modified();
    });

    d->parameters["2_scale_mode"]->connect([=] (QVariant v) {
        auto scaling_mode = v.value<dtk::d_inliststring>().valueIndex();
        if(d->mapper3d) {

            //d->mapper3d->SetScaleMode(scaling_mode < 2 ? scaling_mode : 2);
            //if (scaling_mode > 1) {
            //    d->mapper3d->SetArrayComponent(scaling_mode - 2);
            //}
        }
    });

    d->parameters["3_mask_ratio"]->connect([=] (QVariant v) {
        int value = v.value<dtk::d_int>().value();
        d->mask_points->SetOnRatio(value);
    });

    //d->parameters["4_glyph_stride"]->connect([=] (QVariant v) {
    //    d->mask_points->SetOnRatio(mask_ratio);
    //});
    d->parameters["5_scale_factor"]->connect([=] (QVariant v) {
        double value = v.value<dtk::d_real>().value();
        if(d->mapper3d)
            d->glyph3d->SetScaleFactor(value);
        //    d->mapper3d->SetScaleFactor(value);
    });

    d->parameters["6_colormap"]->connect([=] (QVariant v) {
        d->updateColorFunction();
    });
}

gnomonVtkDecoratorVectorGlyphs::~gnomonVtkDecoratorVectorGlyphs(void)
{
    this->unsetView();
    delete d;
}

void gnomonVtkDecoratorVectorGlyphs::AddActorIfPossible(void)
{
   if(d->view && d->is_decorating) {
       if(d->actor3d) d->view->renderer3D()->AddActor(d->actor3d);
       if(d->actor2d) d->view->renderer2D()->AddActor(d->actor2d);
   }
}

vtkSmartPointer<vtkActor> gnomonVtkDecoratorVectorGlyphs::actor(void)
{
    return d->actor3d;
}

void gnomonVtkDecoratorVectorGlyphs::removeActor(void)
{
    if(d->view) {
        if(d->actor3d) d->view->renderer3D()->RemoveActor(d->actor3d);
        if(d->actor2d) d->view->renderer2D()->RemoveActor(d->actor2d);
    }
}

void gnomonVtkDecoratorVectorGlyphs::setVisibility(bool visible)
{
    if(d->view) {
        if(d->actor3d) d->actor3d->SetVisibility(visible);
        if(d->actor2d) d->actor2d->SetVisibility(visible);
    }
}


bool gnomonVtkDecoratorVectorGlyphs::isDecorating()
{
    return d->is_decorating;
}

void gnomonVtkDecoratorVectorGlyphs::setGrid(vtkSmartPointer<vtkUnstructuredGrid> grid)
{
    //remove actor if existing
    //to check, is it necessary?
    if(d->view) {
        if(d->actor3d) d->view->renderer3D()->RemoveActor(d->actor3d);
        if(d->actor2d)  d->view->renderer2D()->RemoveActor(d->actor2d);
    }

    d->grid = grid;

    //create new actor
    if(!d->mapper3d) {
        d->glyph3d = vtkSmartPointer<vtkGlyph3D>::New();
        d->glyph3d->SetInputData(d->grid);
        d->glyph3d->SetSourceConnection(d->source_arrow->GetOutputPort());
        d->glyph3d->SetScaleModeToScaleByVector();
        d->glyph3d->SetVectorModeToUseVector();
        d->glyph3d->SetColorModeToColorByVector();
        d->glyph3d->ScalingOn();
        d->glyph3d->OrientOn();
        //d->glyph3d->SetInputArrayToProcess(
        //                                   elementInFile,
        //                                   image.GetInformation());

        d->glyph3d->SetScaleFactor(0.1);
        d->glyph3d->Update();

        d->mapper3d = vtkSmartPointer<vtkDataSetMapper>::New();
        d->mapper3d->SetInputConnection(d->glyph3d->GetOutputPort());

        /*
        d->mapper3d = vtkSmartPointer<vtkGlyph3DMapper>::New();
        d->mapper3d->SetInputData(d->grid);
        //d->mapper3d->SetInputConnection(d->mask_points->GetOutputPort());
        d->mapper3d->SetSourceConnection(d->source_arrow->GetOutputPort());
        //d->mapper3d->SetColorModeToMapScalars();
        d->mapper3d->SetOrientationModeToDirection();
        //d->mapper3d->SetScalarModeToUsePointFieldData();
        d->mapper3d->SetScaleMode(vtkGlyph3DMapper::ScaleModes::NO_DATA_SCALING);
        d->mapper3d->OrientOn();
        d->mapper3d->ScalingOn();
        d->mapper3d->SetScaleFactor(0.1);
        */
    }
    //if(!d->mapper2d) {
    //    d->mapper2d = d->mapper3d;
    //}

    d->cell_centers->SetInputData(d->grid);

    if(!d->colorFunction)
        d->colorFunction = vtkSmartPointer<vtkColorTransferFunction>::New();



    //check param
    dtk::d_inliststring *propertyParam = (dtk::d_inliststring *)d->parameters["1_property_name"];
    QStringList properties = {};
    for(int i=0; i < d->grid->GetPointData()->GetNumberOfArrays(); ++i) {
        if(d->grid->GetPointData()->GetArray(i)->GetNumberOfComponents() == 3) {
            properties.append(d->grid->GetPointData()->GetArrayName(i));
        }
    }

    for(int i=0; i < d->grid->GetCellData()->GetNumberOfArrays(); ++i) {
        if(d->grid->GetCellData()->GetArray(i)->GetNumberOfComponents() == 3) {
            properties.append(d->grid->GetCellData()->GetArrayName(i));
        }
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
    //d->updateValueRange();
    //d->mapper3d->SetLookupTable(d->colorFunction);
    //d->mapper3d->Update();
    //d->mapper2d->SetLookupTable(d->colorFunction);
    //d->mapper2d->Update();

    d->actor3d = vtkSmartPointer<vtkActor>::New();
    d->actor3d->SetMapper(d->mapper3d);
    d->actor3d->Modified();

    //d->actor2d = vtkSmartPointer<vtkActor>::New();
    //d->actor2d->SetMapper(d->mapper2d);
    //d->actor2d->Modified();

    if(d->view && d->is_decorating) {
        d->view->renderer3D()->AddActor(d->actor3d);
        d->view->renderer3D()->ResetCamera();
        //d->view->renderer2D()->AddActor(d->actor2d);
        d->view->renderer2D()->ResetCamera();
        d->view->interactor()->Render();
    }
}

void gnomonVtkDecoratorVectorGlyphs::set2DClippingPlane(vtkSmartPointer<vtkPlane> plane)
{
    if(d->mapper2d) {
        d->mapper2d->RemoveAllClippingPlanes();
        d->mapper2d->AddClippingPlane(plane);
        d->mapper2d->Update();
    }
}

void gnomonVtkDecoratorVectorGlyphs::setView(gnomonVtkView *view)
{
    d->view = view;
    if(d->is_decorating) {
        if(d->actor3d) d->view->renderer3D()->AddActor(d->actor3d);
        if(d->actor2d) d->view->renderer2D()->AddActor(d->actor2d);
    }
}

void gnomonVtkDecoratorVectorGlyphs::unsetView()
{
    if(d->is_decorating) {
        if(d->actor3d) d->view->renderer3D()->RemoveActor(d->actor3d);
        if(d->actor2d) d->view->renderer2D()->RemoveActor(d->actor2d);
    }
    d->is_decorating = false;
    d->view = nullptr;
}

dtkCoreParameters gnomonVtkDecoratorVectorGlyphs::parameters()
{
    return d->parameters;
}
