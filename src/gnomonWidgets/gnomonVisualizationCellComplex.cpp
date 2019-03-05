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

#include "gnomonVisualizationCellComplex.h"
#include "gnomonAbstractVisualization_p.h"

#include <QtWidgets>

#include <gnomonCore>
#include <gnomonCore/gnomonCoreParameter>
#include "gnomonCoreParameterColor.h"

#include "gnomonViewForm.h"

#include "gnomonPolyDataCellComplex.h"
#include "gnomonActorPolyData.h"
#include "gnomonActor2DPolyData.h"

#include <vtkCellData.h>
#include <vtkPointData.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>

// /////////////////////////////////////////////////////////////////
// gnomonVisualizationCellComplexPrivate
// /////////////////////////////////////////////////////////////////

class gnomonVisualizationCellComplexPrivate
{
public:
    gnomonCellComplex *cellComplex;

public:
    gnomonPolyDataCellComplex *polydata = nullptr;
    gnomonActorPolyData *actor = nullptr;
    gnomonActor2DPolyData *actor2D = nullptr;

    gnomonVisualizationCellComplex *q;

public slots:
    void updateOpacity(void);
    void updateValueRange(void);
};

void gnomonVisualizationCellComplexPrivate::updateOpacity(void)
{
    double alpha = ((gnomonCoreParameterDouble *)q->parameters()["alpha"])->value();

    if(this->actor) {
        this->actor->setOpacity(alpha);
    }

    if(this->actor2D) {
        this->actor2D->setOpacity(alpha);
    }
}

void gnomonVisualizationCellComplexPrivate::updateValueRange(void)
{
    QString property_name = ((gnomonCoreParameterString *)q->parameters()["property_name"])->value();

    QMap<long, QVariant> cellProperty;
    if(this->cellComplex->elementPropertyNames(3).contains(property_name)) {
        cellProperty = this->cellComplex->elementProperty(3,property_name);
    } else {
        for (const auto& cellId : this->cellComplex->elementIds(3)) {
            cellProperty[cellId] = QVariant((double)cellId);
        }
    }

    QList<double> cellScalarPropertyValues;
    for (const auto& cellId : this->cellComplex->elementIds(3)) {
        cellScalarPropertyValues.append(cellProperty[cellId].value<double>());
    }
    auto mm = std::minmax_element(cellScalarPropertyValues.begin(),cellScalarPropertyValues.end());

    ((gnomonCoreParameterDoubleRange *)q->parameters()["value_range"])->setMinimumValue(*(mm.first));
    ((gnomonCoreParameterDoubleRange *)q->parameters()["value_range"])->setMaximumValue(*(mm.second));
}


// /////////////////////////////////////////////////////////////////
// gnomonVisualizationCellComplex
// /////////////////////////////////////////////////////////////////

gnomonVisualizationCellComplex::gnomonVisualizationCellComplex(void) : gnomonAbstractVisualizationCellComplex(), dd(new gnomonVisualizationCellComplexPrivate)
{

    dd->q = this;
    dd->cellComplex = Q_NULLPTR;

    d->parameters["property_name"] = new gnomonCoreParameterString("", {""}, "CellComplex property to be displayed");
    d->parameters["value_range"] = new gnomonCoreParameterDoubleRange(0., 1., 0., 1., "Value range for color adjustment");
    d->parameters["colormap"] = new gnomonCoreParameterColorMap("glasbey", "Colormap to apply to the cellComplex");
    d->parameters["alpha"] = new gnomonCoreParameterDouble(1, 0, 1, 2, "Transparency value for the cellComplex rendering");
    d->parameters["scale_factor"] = new gnomonCoreParameterDouble(0.99, 0, 1, 2, "Scale for cell surface visualization");


}

gnomonVisualizationCellComplex::~gnomonVisualizationCellComplex(void)
{

    if (dd->actor) {
        d->view->renderer3D()->RemoveActor(dd->actor);
        dd->actor->Delete();
        dd->actor = nullptr;
    }

    if (dd->actor2D) {
        disconnect(d->connectSliceOrientation);
        disconnect(d->connectSlice);
        d->view->renderer2D()->RemoveActor(dd->actor2D);
        dd->actor2D->Delete();
        dd->actor2D = nullptr;
    }

    delete dd;

    dd = NULL;
}

void gnomonVisualizationCellComplex::setCellComplex(gnomonCellComplex *cellComplex)
{
    dd->cellComplex = cellComplex;

    this->setParameter("alpha",1.0);
    connect(d->parameters["property_name"], &gnomonCoreParameter::valueChanged, [=] () {
        if(!dd->cellComplex)
            return;
        dd->updateValueRange();
        emit parametersChanged();
    });

    gnomonCoreParameterString *propertyParam = (gnomonCoreParameterString *)d->parameters["property_name"];
    QString property_name = ((gnomonCoreParameterString *)d->parameters["property_name"])->value();

    QStringList properties = {""};
    for (const auto& prop : dd->cellComplex->elementPropertyNames(3)) {
        if(dd->cellComplex->elementProperty(3,prop)[dd->cellComplex->elementIds(3)[0]].canConvert<double>()) {
            properties.append(prop);
        }
    }
    propertyParam->setValues(properties);
    if (properties.contains(property_name)) {
        propertyParam->setValue(property_name);
    } else {
        propertyParam->setValue(QString(""));
    }

    dd->updateValueRange();
}

QImage gnomonVisualizationCellComplex::imageRendering(void)
{
    d->updateOffscreenRenderer(dd->polydata->GetBounds());

    d->offscreenRenderer->AddActor(dd->actor);

    return d->offscreenImageRendering();
}

void gnomonVisualizationCellComplex::update(void)
{
    QString property_name = ((gnomonCoreParameterString *)d->parameters["property_name"])->value();
    QMap<double, QColor> colormap = ((gnomonCoreParameterColorMap *)d->parameters["colormap"])->value();
    QList<double> value_range = ((gnomonCoreParameterDoubleRange *)d->parameters["value_range"])->value();
    double scale = ((gnomonCoreParameterDouble *)d->parameters["scale_factor"])->value();


    if(!dd->cellComplex)
        return;

    qDebug()<<Q_FUNC_INFO<<dd->cellComplex<<dd->cellComplex->elementCount(2)<<"Faces";

    if (dd->polydata) {
        dd->polydata->Delete();
        dd->polydata = nullptr;
    }

    if (!dd->polydata)
        dd->polydata = gnomonPolyDataCellComplex::New();
    dd->polydata->setCellComplex((gnomonCellComplex *)dd->cellComplex->clone());
    dd->polydata->setPropertyName(property_name);
    dd->polydata->setScaleFactor(scale);
    dd->polydata->update();


    if (dd->actor) {
        d->view->renderer3D()->RemoveActor(dd->actor);
        dd->actor->Delete();
        dd->actor = nullptr;
    }

    if (!dd->actor) {
        dd->actor = gnomonActorPolyData::New();
        d->view->renderer3D()->AddActor(dd->actor);
    }
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
    dd->actor2D->setSliceThickness(0.5);
    dd->actor2D->setPolyData(dd->polydata);
    dd->actor2D->setColorMap(colormap);
    dd->actor2D->setValueRange(value_range);

    d->connectSliceOrientation = connect(d->view, &gnomonViewForm::sliceOrientationChanged, [=] (int value) {
        dd->actor2D->setSliceOrientation(value);
    });
//
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

void gnomonVisualizationCellComplex::render(void)
{
    dd->updateOpacity();
    d->view->render();
}

QMap<QString, gnomonCoreParameter *> gnomonVisualizationCellComplex::parameters(void) const
{
    return d->parameters;
}

void gnomonVisualizationCellComplex::setParameter(const QString& parameter, const QVariant& value)
{
    if (d->parameters.contains(parameter)) {
        d->parameters[parameter]->setValue(value);
    }
    else
        qWarning()<<parameter<<"is not a valid parameter!";
}

void gnomonVisualizationCellComplex::setParameters(const QMap<QString, gnomonCoreParameter *>& parameters)
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
// gnomonVisualizationCellComplex.cpp ends here
