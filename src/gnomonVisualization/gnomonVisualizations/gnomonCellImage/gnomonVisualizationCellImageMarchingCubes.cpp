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

#include "gnomonVisualizations/gnomonAbstractVisualization_p.h"

#include <QtWidgets>

#include <gnomonCore>
#include <gnomonWidgets>

#include <dtkImagingCore>

#include "gnomonView/gnomonViewForm.h"

#include "gnomonActor/gnomonCellImage/gnomonPolyDataCellImage.h"
#include "gnomonActor/gnomonPolyData/gnomonActorPolyData.h"
#include "gnomonActor/gnomonPolyData/gnomonActor2DPolyData.h"

#include <vtkCellData.h>
#include <vtkCellLocator.h>
#include <vtkCellPicker.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkPointData.h>
#include <vtkProp3DCollection.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkTextActor.h>
#include <vtkTextProperty.h>

// ///////////////////////////////////////////////////////////////////
// gnomonInteractorStyleCellImageMarchingCubes
// ///////////////////////////////////////////////////////////////////

class gnomonInteractorStyleCellImageMarchingCubes : public vtkInteractorStyleTrackballCamera
{
public:
    static gnomonInteractorStyleCellImageMarchingCubes *New(void);

public:
    virtual void OnMouseMove(void) override
    {
        vtkInteractorStyleTrackballCamera::OnMouseMove();

        this->clicks = 0;

        int *pos = this->GetInteractor()->GetEventPosition();

        this->picker->Pick(pos[0], pos[1], 0, this->GetDefaultRenderer());

        long vtkId = -1;
        if (picker->GetViewProp()==this->actor) {
            vtkId = picker->GetCellId();
        }

        this->updateTextActor(vtkId);
    }

    virtual void OnLeftButtonDown(void) override
    {
        vtkInteractorStyleTrackballCamera::OnLeftButtonDown();

        int *pos = this->GetInteractor()->GetEventPosition();

        this->clicks++;

        this->picker->Pick(pos[0], pos[1], 0, this->GetDefaultRenderer());
        long vtkId = -1;
        if (picker->GetViewProp()==this->actor) {
            vtkId = picker->GetCellId();
        }

        if (vtkId == -1) {
            this->clicks = 0;

            if (this->q->view()->infoPane()->isToggled()) {
                this->q->view()->infoPane()->toggle();
                this->q->view()->infoPane()->clear();
                this->infoPaneItem = nullptr;
                this->infoLayout = nullptr;
            }
        }

        if (this->clicks == 2) {
            this->OnDoubleClick(vtkId);
            this->clicks = 0;
        }
    }

    virtual void OnLeftButtonUp(void) override
    {
        vtkInteractorStyleTrackballCamera::OnLeftButtonUp();
        
        int *pos = this->GetInteractor()->GetEventPosition();
        this->picker->Pick(pos[0], pos[1], 0, this->GetDefaultRenderer());

        long vtkId = -1;
        if (picker->GetViewProp()==this->actor) {
            vtkId = picker->GetCellId();
        }
    }

    void OnDoubleClick(long vtkId)
    {
        long cellId = q->cellId(vtkId);
        QString text = "Cell ";
        text.append(QString::number(cellId));

        if (!this->infoPaneItem) {
            this->infoPaneItem = new gnomonOverlayPaneItem((QWidget *) q->view()->parent());
            this->infoPaneItem->toggle();
        }
        this->infoPaneItem->setTitle(text);

        if (!this->infoLayout) {
            this->infoLayout = new QFormLayout;
            this->infoLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
            this->infoPaneItem->addLayout(infoLayout);
        } else {
            for(int row = 0, max_row = this->infoLayout->count(); row < max_row; ++row) {
                QLayoutItem *forDeletion = this->infoLayout->takeAt(0);
                forDeletion->widget()->disconnect();
                delete forDeletion->widget();
                delete forDeletion;
            }
        }

        QMap<QString, QVariant> cellInfo = q->cellInfo(cellId);
        for(QMap<QString, QVariant>::iterator it = cellInfo.begin(), it_end = cellInfo.end(); it != it_end; ++it) {
            infoLayout->addRow(it.key(), new QLabel(it.value().toString()));
        }

        if (!this->q->view()->infoPane()->isToggled()) {
            this->q->view()->infoPane()->toggle();
            this->q->view()->infoPane()->addWidget(this->infoPaneItem);
        }
    }

    void updateTextActor(long vtkId)
    {
        if (!this->textActor) {
            this->textActor = vtkSmartPointer<vtkTextActor>::New();
            this->textActor->SetPosition2(10, 40);
            this->textActor->GetTextProperty()->SetFontSize(24);
            this->textActor->GetTextProperty()->SetColor (1.0, 1.0, 1.0);
            this->GetDefaultRenderer()->AddActor2D(textActor);
        }

        if (vtkId > -1) {
            long cellId = q->cellId(vtkId);
            QString text = "Cell ";
            text.append(QString::number(cellId));
            this->textActor->SetInput(text.toStdString().c_str());
        } else {
            this->textActor->SetInput("");
        }
        this->GetInteractor()->Render();
    }

    void setActor(vtkProp *actor)
    {
        this->actor = actor;
    }

public:
    gnomonInteractorStyleCellImageMarchingCubes(void) : vtkInteractorStyleTrackballCamera()
    {
        this->picker = vtkSmartPointer<vtkCellPicker>::New();
        this->picker->SetTolerance(0.0005);
    }

public:
    ~gnomonInteractorStyleCellImageMarchingCubes(void)
    {
        this->picker->Delete();
        this->picker = nullptr;

        if (this->q->view()->infoPane()->isToggled()) {
            this->q->view()->infoPane()->toggle();
            this->q->view()->infoPane()->clear();
        }

        delete this->infoLayout;
        this->infoLayout = nullptr;

        delete this->infoPaneItem;
        this->infoPaneItem = nullptr;

        this->q = nullptr;
    }

public:
    gnomonVisualizationCellImageMarchingCubes *q = nullptr;

public:
    gnomonOverlayPaneItem *infoPaneItem = nullptr;
    QFormLayout *infoLayout = nullptr;

public:
    vtkSmartPointer<vtkCellPicker> picker = nullptr;
    vtkSmartPointer<vtkTextActor> textActor = nullptr;
    vtkSmartPointer<vtkProp> actor = nullptr;

private:
    unsigned int clicks = 0;

};

vtkStandardNewMacro(gnomonInteractorStyleCellImageMarchingCubes);

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

public:
    gnomonInteractorStyleCellImageMarchingCubes *interactor_style = nullptr;

    gnomonVisualizationCellImageMarchingCubes *q;
    
public slots:
    void updateOpacity(void);
    void updateValueRange(void);
};


void gnomonVisualizationCellImageMarchingCubesPrivate::updateOpacity(void)
{
    double alpha = ((gnomonCoreParameterDouble *)q->parameters()["alpha"])->value();
    
    if(this->actor) {
        this->actor->setOpacity(alpha);
    }

    if(this->actor2D) {
        this->actor2D->setOpacity(alpha);
    }
}

void gnomonVisualizationCellImageMarchingCubesPrivate::updateValueRange(void)
{
     QString property_name = ((gnomonCoreParameterString *)q->parameters()["property_name"])->value();

     QMap<long, QVariant> cellProperty;
     if(this->cellImage->cellPropertyNames().contains(property_name)) {
         cellProperty = this->cellImage->cellProperty(property_name);
     } else {
         for (const auto& cellId : this->cellImage->cellIds()) {
             cellProperty[cellId] = QVariant((double)cellId);
         }
     }

     QList<double> cellScalarPropertyValues;
     for (const auto& cellId : this->cellImage->cellIds()) {
         cellScalarPropertyValues.append(cellProperty[cellId].value<double>());
     }
     auto mm = std::minmax_element(cellScalarPropertyValues.begin(),cellScalarPropertyValues.end());

     ((gnomonCoreParameterDoubleRange *)q->parameters()["value_range"])->setMinimumValue(*(mm.first));
     ((gnomonCoreParameterDoubleRange *)q->parameters()["value_range"])->setMaximumValue(*(mm.second));
     ((gnomonCoreParameterDoubleRange *)q->parameters()["value_range"])->setValue(*(mm.first),*(mm.second));

     QList<double> value_range = ((gnomonCoreParameterDoubleRange *)this->q->parameters()["value_range"])->value();
}

// /////////////////////////////////////////////////////////////////
// gnomonVisualizationCellImageMarchingCubes
// /////////////////////////////////////////////////////////////////

gnomonVisualizationCellImageMarchingCubes::gnomonVisualizationCellImageMarchingCubes(void) : gnomonAbstractVisualizationCellImage(), dd(new gnomonVisualizationCellImageMarchingCubesPrivate)
{
    dd->q = this;
    dd->cellImage = Q_NULLPTR;

    d->parameters["property_name"] = new gnomonCoreParameterString("", {""}, "CellImage property to be displayed");
    d->parameters["value_range"] = new gnomonCoreParameterDoubleRange(0., 1., 0., 1., "Value range for color adjustment");
    d->parameters["colormap"] = new gnomonCoreParameterColorMap("glasbey", "Colormap to apply to the cellImage");
    d->parameters["alpha"] = new gnomonCoreParameterDouble(1, 0, 1, 2, "Transparency value for the cellImage rendering");

    d->parameters["x_range"] = new gnomonCoreParameterDoubleRange(0., 100., 0., 100., "Range of x positions of cells to display");
    d->parameters["y_range"] = new gnomonCoreParameterDoubleRange(0., 100., 0., 100., "Range of y positions of cells to display");
    d->parameters["z_range"] = new gnomonCoreParameterDoubleRange(0., 100., 0., 100., "Range of z positions of cells to display");

    dd->interactor_style = gnomonInteractorStyleCellImageMarchingCubes::New();
    dd->interactor_style->q = this;
}

gnomonVisualizationCellImageMarchingCubes::~gnomonVisualizationCellImageMarchingCubes(void)
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

    disconnect(d->connect3D);
    disconnect(d->connect2D);
    disconnect(d->connectXY);
    disconnect(d->connectXZ);
    disconnect(d->connectYZ);

    dd->interactor_style->Delete();

    delete dd;

    dd = NULL;
}

void gnomonVisualizationCellImageMarchingCubes::setCellImage(gnomonCellImage *cellImage)
{
    dd->cellImage = cellImage;

    this->setParameter("alpha",1.0);
    connect(d->parameters["property_name"], &gnomonCoreParameter::valueChanged, [=] () {
        if(!dd->cellImage)
            return;
        dd->updateValueRange();
        emit parametersChanged();
    });

    gnomonCoreParameterString *propertyParam = (gnomonCoreParameterString *)d->parameters["property_name"];
    QString property_name = ((gnomonCoreParameterString *)d->parameters["property_name"])->value();

    QStringList properties = {""};
    for (const auto& propertyName : dd->cellImage->cellPropertyNames()) {
         QVariant property_variant = dd->cellImage->cellProperty(propertyName)[dd->cellImage->cellIds()[0]];
         if(property_variant.canConvert<double>() || property_variant.canConvert<int>()) {
                properties.append(propertyName);
         }
    }
    propertyParam->setValues(properties);
    if (properties.contains(property_name)) {
        propertyParam->setValue(property_name);
    } else {
        propertyParam->setValue(QString(""));
    }

    dd->updateValueRange();

    gnomonCoreParameterDoubleRange *xRangeParam = (gnomonCoreParameterDoubleRange *)d->parameters["x_range"];
    xRangeParam->setMinimumValue(0);
    xRangeParam->setMaximumValue(dd->cellImage->image()->xDim()*dd->cellImage->image()->spacing()[0]);
    xRangeParam->setValue(0,dd->cellImage->image()->xDim()*dd->cellImage->image()->spacing()[0]);

    gnomonCoreParameterDoubleRange *yRangeParam = (gnomonCoreParameterDoubleRange *)d->parameters["y_range"];
    yRangeParam->setMinimumValue(0);
    yRangeParam->setMaximumValue(dd->cellImage->image()->yDim()*dd->cellImage->image()->spacing()[1]);
    yRangeParam->setValue(0,dd->cellImage->image()->yDim()*dd->cellImage->image()->spacing()[1]);

    gnomonCoreParameterDoubleRange *zRangeParam = (gnomonCoreParameterDoubleRange *)d->parameters["z_range"];
    zRangeParam->setMinimumValue(0);
    zRangeParam->setMaximumValue(dd->cellImage->image()->zDim()*dd->cellImage->image()->spacing()[2]);
    zRangeParam->setValue(0,dd->cellImage->image()->zDim()*dd->cellImage->image()->spacing()[2]);

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
     qDebug()<<Q_FUNC_INFO<<property_name<<value_range;

     QList<double> x_range = ((gnomonCoreParameterDoubleRange *)d->parameters["x_range"])->value();
     QList<double> y_range = ((gnomonCoreParameterDoubleRange *)d->parameters["y_range"])->value();
     QList<double> z_range = ((gnomonCoreParameterDoubleRange *)d->parameters["z_range"])->value();

    if(!dd->cellImage)
        return;

    if (!dd->polydata)
        dd->polydata = gnomonPolyDataCellImage::New();
    dd->polydata->setCellImage(dd->cellImage);
    dd->polydata->setPropertyName(property_name);
    dd->polydata->setSliceRanges(x_range, y_range, z_range);
    dd->polydata->update();

    if (!dd->actor)
    {
        dd->actor = gnomonActorPolyData::New();
        d->view->renderer3D()->AddActor(dd->actor);
    }
    dd->actor->setInteractor(d->view->interactor());
    dd->actor->setPolyData(dd->polydata);
    dd->actor->setColorMap(colormap);
    dd->actor->setValueRange(value_range);

    dd->interactor_style->setActor(dd->actor);

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

    d->connect3D = connect(d->view, &gnomonViewForm::switchedTo3D, [=] () { this->render(); });
    d->connect2D = connect(d->view, &gnomonViewForm::switchedTo2D, [=] () { this->render(); });
    d->connectXY = connect(d->view, &gnomonViewForm::switchedTo2DXY, [=] () { this->render(); });
    d->connectXZ = connect(d->view, &gnomonViewForm::switchedTo2DYZ, [=] () { this->render(); });
    d->connectYZ = connect(d->view, &gnomonViewForm::switchedTo2DXZ, [=] () { this->render(); });

    double bounds[6];
    dd->polydata->GetBounds(bounds);
    d->view->setBounds(bounds);

    this->render();
}

void gnomonVisualizationCellImageMarchingCubes::render(void)
{
    dd->interactor_style->SetDefaultRenderer(d->view->renderer3D());
    d->view->interactor()->SetInteractorStyle(dd->interactor_style);
    d->view->interactor()->Enable();

    dd->updateOpacity();
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

long gnomonVisualizationCellImageMarchingCubes::cellId(long vtkId)
{
    return dd->polydata->cellId(vtkId);
}

QMap<QString, QVariant> gnomonVisualizationCellImageMarchingCubes::cellInfo(long cellId)
{
    QMap<QString, QVariant> info;
//    info["label"] = QVariant(int(cellId));
    for (const auto& propertyName : dd->cellImage->cellPropertyNames()) {
        QMap<long, QVariant> property = dd->cellImage->cellProperty(propertyName);
        if (property.contains(cellId)) {
            info[propertyName] = property[cellId];
        }
    }

    return info;
}

//
// gnomonVisualizationCellImageMarchingCubes.cpp ends here
