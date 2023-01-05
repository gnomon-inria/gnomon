#include "gnomonInteractorStyleCellImageMarchingCubes.h"
#include "gnomonInteractorStyle/gnomonInteractorStyle_p.h"

#include "gnomonVisualizations/gnomonCellImage/gnomonAbstractVisualizationCellImage.h"

#include "gnomonView/gnomonViewForm.h"

#include "gnomonActor/gnomonCellImage/gnomonPolyDataCellImage.h"
#include "gnomonActor/gnomonPolyData/gnomonActorPolyData.h"
#include "gnomonActor/gnomonPolyData/gnomonActor2DPolyData.h"

#include "gnomonInteractorStyle/gnomonCellImage/gnomonInteractorStyleCellImageMarchingCubes.h"

#include <vtkCellPicker.h>
#include <vtkProp3DCollection.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkTextActor.h>
#include <vtkTextProperty.h>


// ///////////////////////////////////////////////////////////////////
// gnomonInteractorStyleCellImageMarchingCubesPrivate
// ///////////////////////////////////////////////////////////////////

class gnomonInteractorStyleCellImageMarchingCubesPrivate
{
    public:
        gnomonAbstractVisualizationCellImage *visu = nullptr;

    public:
        vtkSmartPointer<vtkCellPicker> picker = nullptr;
        vtkSmartPointer<vtkTextActor> textActor = nullptr;
        vtkSmartPointer<vtkProp> actor = nullptr;

    public:
        unsigned int clicks = 0;
};

// ///////////////////////////////////////////////////////////////////
// gnomonInteractorStyleCellImageMarchingCubes
// ///////////////////////////////////////////////////////////////////


gnomonInteractorStyleCellImageMarchingCubes::gnomonInteractorStyleCellImageMarchingCubes(void) : gnomonInteractorStyle(), dd(new gnomonInteractorStyleCellImageMarchingCubesPrivate)
{
    dd->picker = vtkSmartPointer<vtkCellPicker>::New();
    dd->picker->SetTolerance(0.0005);
}

gnomonInteractorStyleCellImageMarchingCubes::~gnomonInteractorStyleCellImageMarchingCubes(void)
{
    dd->picker->Delete();
    dd->picker = nullptr;

    dd->visu = nullptr;
}

void gnomonInteractorStyleCellImageMarchingCubes::OnMouseMove(void)
{
    gnomonInteractorStyle::OnMouseMove();
    dd->clicks = 0;
    this->updateTextActor(-1);
}

void gnomonInteractorStyleCellImageMarchingCubes::OnLeftButtonDown(void)
{
    gnomonInteractorStyle::OnLeftButtonDown();

    int *pos = this->GetInteractor()->GetEventPosition();

    dd->clicks++;

    dd->picker->Pick(pos[0], pos[1], 0, this->GetDefaultRenderer());

    long vtkId = -1;
    if (dd->picker->GetViewProp()==dd->actor) {
        vtkId = dd->picker->GetCellId();
    }

    if (vtkId == -1) {
        dd->clicks = 0;

        // if (this->visu->view()->infoPane()->isToggled()) {
        //     this->visu->view()->infoPane()->toggle();
        //     this->visu->view()->infoPane()->clear();
        //     this->infoPaneItem = nullptr;
        //     this->infoLayout = nullptr;
        // }
    }
}

void gnomonInteractorStyleCellImageMarchingCubes::OnLeftButtonUp(void)
{
    gnomonInteractorStyle::OnLeftButtonUp();

    int *pos = this->GetInteractor()->GetEventPosition();
    dd->picker->Pick(pos[0], pos[1], 0, this->GetDefaultRenderer());

    long vtkId = -1;
    if (dd->picker->GetViewProp()==dd->actor) {
        vtkId = dd->picker->GetCellId();
    }

    this->updateTextActor(vtkId);

    if (dd->clicks == 1) {

        int *pos = this->GetInteractor()->GetEventPosition();

        dd->picker->Pick(pos[0], pos[1], 0, this->GetDefaultRenderer());

        long vtkId = -1;
        if (dd->picker->GetViewProp()==dd->actor) {
            vtkId = dd->picker->GetCellId();
        }
    }

    if (dd->clicks == 2) {
        this->OnDoubleClick(vtkId);
        dd->clicks = 0;
    }
}

void gnomonInteractorStyleCellImageMarchingCubes::OnDoubleClick(long vtkId)
{
    // this->visu->view()->infoPane()->clear();
    qDebug() << Q_FUNC_INFO << "DESACTIVATED FOR NOW";
        // long cellId = dd->visu->cellId(vtkId);
        // QString text = "        Cell ";
        // text.append(QString::number(cellId));

    // QVariantMap cellInfo = q->cellInfo(cellId);

    // if(!this->visu->view()->infoPane()->isToggled())
    //     this->visu->view()->infoPane()->toggle();

    // this->visu->view()->infoPane()->addInfoPaneItem(text, cellInfo);
}

QMap<int, QString> gnomonInteractorStyleCellImageMarchingCubes::keyMap(void) const
{
    d->keymap[-Qt::LeftButton] = "Display cell ID";
    return gnomonInteractorStyle::keyMap();
}


void gnomonInteractorStyleCellImageMarchingCubes::updateTextActor(long vtkId)
{
    if (!dd->textActor) {
        dd->textActor = vtkSmartPointer<vtkTextActor>::New();
        dd->textActor->SetPosition2(40, 40);
        dd->textActor->GetTextProperty()->SetFontSize(24);
        dd->textActor->GetTextProperty()->SetColor (1.0, 1.0, 1.0);
    }
    this->GetDefaultRenderer()->AddActor2D(dd->textActor);

    if (vtkId > -1) {
        qDebug() << Q_FUNC_INFO << "DESACTIVATED FOR NOW";
        long cellId = 1;
        //long cellId = dd->visu->cellId(vtkId);
        QString text = "        Cell ";
        text.append(QString::number(cellId));
        dd->textActor->SetInput(text.toStdString().c_str());
    } else {
        dd->textActor->SetInput("");
    }
    this->GetInteractor()->Render();
}

void gnomonInteractorStyleCellImageMarchingCubes::setActor(vtkProp *actor)
{
    dd->actor = actor;
}

void gnomonInteractorStyleCellImageMarchingCubes::setVisualization(gnomonAbstractVisualizationCellImage *visu)
{
    dd->visu = visu;
}

void gnomonInteractorStyleCellImageMarchingCubes::disable(void)
{
    if (dd->textActor) {
        this->GetDefaultRenderer()->RemoveActor2D(dd->textActor);
    }
    gnomonInteractorStyle::disable();
}

QString gnomonInteractorStyleCellImageMarchingCubes::description(void) const
{
    return "Cell picker";
}

int gnomonInteractorStyleCellImageMarchingCubes::icon(void) const
{
    return 0; // fa::crosshairs;
}
