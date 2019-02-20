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

#include "gnomonVisualizationCellImageMarchingCubes.h"
#include "gnomonAbstractVisualization_p.h"

#include <QtWidgets>

#include <gnomonCore/gnomonCellImage>
#include <gnomonCore/gnomonCoreParameter>
#include "gnomonCoreParameterColor.h"
#include <dtkImagingCore>

#include "gnomonViewForm.h"

#include "gnomonPolyDataCellImage.h"
#include "gnomonActorPolyData.h"
#include "gnomonActor2DPolyData.h"

#include <vtkCellData.h>
#include <vtkPointData.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>


// /////////////////////////////////////////////////////////////////
// gnomonVisualizationCellImageMarchingCubesPrivate
// /////////////////////////////////////////////////////////////////

class gnomonVisualizationCellImageMarchingCubesPrivate
{
public:
    gnomonCellImage *cellImage;

public:
    gnomonPolyDataCellImage *polydata = nullptr;
    gnomonActorPolyData *actor = nullptr;
    gnomonActor2DPolyData *actor2D = nullptr;
};

// /////////////////////////////////////////////////////////////////
// gnomonVisualizationCellImageMarchingCubes
// /////////////////////////////////////////////////////////////////

gnomonVisualizationCellImageMarchingCubes::gnomonVisualizationCellImageMarchingCubes(void) : gnomonAbstractVisualization(), dd(new gnomonVisualizationCellImageMarchingCubesPrivate)
{
    dd->cellImage = Q_NULLPTR;

    d->parameters["property_name"] = new gnomonCoreParameterString("", {""}, "CellImage property to be displayed");
    d->parameters["value_range"] = new gnomonCoreParameterDoubleRange(0., 1., 0., 1., "Value range for color adjustment");
    d->parameters["colormap"] = new gnomonCoreParameterColorMap("glasbey", "Colormap to apply to the cellImage");
    d->parameters["alpha"] = new gnomonCoreParameterDouble(1, 0, 1, 2, "Transparency value for the cellImage rendering");

    connect(d->parameters["property_name"], &gnomonCoreParameter::valueChanged, [=] () {
        if(!dd->cellImage)
            return;
        this->updateValueRange();
        emit parametersChanged();
    });
}

gnomonVisualizationCellImageMarchingCubes::~gnomonVisualizationCellImageMarchingCubes(void)
{
    delete dd;

    dd = NULL;
}

void gnomonVisualizationCellImageMarchingCubes::setCellImage(gnomonCellImage *cellImage)
{
    dd->cellImage = cellImage;

    this->setParameter("alpha",1.0);
    
    gnomonCoreParameterString *propertyParam = (gnomonCoreParameterString *)d->parameters["property_name"];
    QStringList properties = {""};
    for (const auto& propertyName : dd->cellImage->cellPropertyNames()) {
         if(dd->cellImage->cellProperty(propertyName)[dd->cellImage->cellIds()[0]].canConvert<double>()) {
                properties.append(propertyName);
         }
    }
    propertyParam->setValues(properties);
    propertyParam->setValue(QString(""));

    this->updateValueRange();
}

void gnomonVisualizationCellImageMarchingCubes::updateOpacity(void)
{
    double alpha = ((gnomonCoreParameterDouble *)d->parameters["alpha"])->value();
    
    if(dd->actor) {
        dd->actor->setOpacity(alpha);
    }

    if(dd->actor2D) {
        dd->actor2D->setOpacity(alpha);
    }
}

void gnomonVisualizationCellImageMarchingCubes::updateValueRange(void)
{
     QString property_name = ((gnomonCoreParameterString *)d->parameters["property_name"])->value();

     QMap<long, QVariant> cellProperty;
     if(dd->cellImage->cellPropertyNames().contains(property_name)) {
         cellProperty = dd->cellImage->cellProperty(property_name);
     } else {
         for (const auto& cellId : dd->cellImage->cellIds()) {
             cellProperty[cellId] = QVariant((double)cellId);
         }
     }

     QList<double> cellScalarPropertyValues;
     for (const auto& cellId : dd->cellImage->cellIds()) {
         cellScalarPropertyValues.append(cellProperty[cellId].value<double>());
     }
     auto mm = std::minmax_element(cellScalarPropertyValues.begin(),cellScalarPropertyValues.end());

     ((gnomonCoreParameterDoubleRange *)d->parameters["value_range"])->setMinimumValue(*(mm.first));
     ((gnomonCoreParameterDoubleRange *)d->parameters["value_range"])->setMaximumValue(*(mm.second));
}

QImage gnomonVisualizationCellImageMarchingCubes::imageRendering(void)
{
    d->updateOffscreenRenderer(dd->polydata->GetBounds());

    d->offscreenRenderer->AddActor(dd->actor);

    return d->offscreenImageRendering();
}

void gnomonVisualizationCellImageMarchingCubes::update(void)
{
     QString property_name = ((gnomonCoreParameterString *)d->parameters["property_name"])->value();
     QMap<double, QColor> colormap = ((gnomonCoreParameterColorMap *)d->parameters["colormap"])->value();
     QList<double> value_range = ((gnomonCoreParameterDoubleRange *)d->parameters["value_range"])->value();

    if(!dd->cellImage)
        return;

    if (dd->polydata) {
        dd->polydata->Delete();
        dd->polydata = nullptr;
    }

    if (!dd->polydata)
        dd->polydata = gnomonPolyDataCellImage::New();
    dd->polydata->setCellImage((gnomonCellImage *)dd->cellImage->clone());
    dd->polydata->setPropertyName(property_name);
    dd->polydata->update();
    

    if (dd->actor) {
        d->view->renderer3D()->RemoveActor(dd->actor);
        dd->actor->Delete();
        dd->actor = nullptr;
    }

    if (!dd->actor)
        dd->actor = gnomonActorPolyData::New();
        d->view->renderer3D()->AddActor(dd->actor);
    dd->actor->setInteractor(d->view->interactor());
    dd->actor->setPolyData(dd->polydata);
    dd->actor->setColorMap(colormap);
    dd->actor->setValueRange(value_range);

    if (dd->actor2D) {
        disconnect(d->connectSliceOrientation);
        disconnect(d->connectSlice);
        d->view->renderer2D()->RemoveActor(dd->actor2D);
        dd->actor2D->Delete();
        dd->actor2D = nullptr;
    }

    if (!dd->actor2D)
    {
        dd->actor2D = gnomonActor2DPolyData::New();
        d->view->renderer2D()->AddActor(dd->actor2D);
    }
    dd->actor2D->setInteractor(d->view->interactor());
    dd->actor2D->setSliceThickness(0.1);
    dd->actor2D->setPolyData(dd->polydata);
    dd->actor2D->setColorMap(colormap);
    dd->actor2D->setValueRange(value_range);

    d->connectSliceOrientation = connect(d->view, &gnomonViewForm::sliceOrientationChanged, [=] (int value) {
        dd->actor2D->setSliceOrientation(value);
    });

    d->connectSlice = connect(d->view, &gnomonViewForm::sliceChanged, [=] (int value) {
        dd->actor2D->setSlice(value);
        this->render();
    });

    connect(d->view, &gnomonViewForm::switchedTo3D, [=] () { this->render(); });
    connect(d->view, &gnomonViewForm::switchedTo2D, [=] () { this->render(); });
    connect(d->view, &gnomonViewForm::switchedTo2DXY, [=] () { this->render(); });
    connect(d->view, &gnomonViewForm::switchedTo2DYZ, [=] () { this->render(); });
    connect(d->view, &gnomonViewForm::switchedTo2DXZ, [=] () { this->render(); });

    double bounds[6];
    dd->polydata->GetBounds(bounds);
    d->view->setBounds(bounds);

    this->render();
}

void gnomonVisualizationCellImageMarchingCubes::render(void)
{
    this->updateOpacity();
    d->view->render();
}


QMap<QString, gnomonCoreParameter *> gnomonVisualizationCellImageMarchingCubes::parameters(void) const
{
    return d->parameters;
}

void gnomonVisualizationCellImageMarchingCubes::setParameter(const QString& parameter, const QVariant& value)
{
    if (d->parameters.contains(parameter)) {
        d->parameters[parameter]->setValue(value);
    }
    else
        qWarning()<<parameter<<"is not a valid parameter!";
}

void gnomonVisualizationCellImageMarchingCubes::setParameters(const QMap<QString, gnomonCoreParameter *>& parameters)
{
//    d->parameters = parameters;
    for (const auto& param : parameters.keys()) {
        if (d->parameters.contains(param)) {
//            d->parameters[param] = parameters[param];
            d->parameters[param]->copy(parameters[param]);
        }
    }
}

//
// gnomonVisualizationCellImageMarchingCubes.cpp ends here
