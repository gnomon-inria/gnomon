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

#include "gnomonInteractorStyleXYZ.h"
#include "gnomonInteractorStyle_p.h"

#include "gnomonView/gnomonViewForm.h"

#include <vtkCamera.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>


// ///////////////////////////////////////////////////////////////////
// gnomonInteractorStyleCellImageMarchingCubesPrivate
// ///////////////////////////////////////////////////////////////////

class gnomonInteractorStyleXYZPrivate
{
    public:
        int axis;

    public:
        gnomonInteractorStyleXYZPrivate(void);

};

gnomonInteractorStyleXYZPrivate::gnomonInteractorStyleXYZPrivate(void)
{
    this->axis=2; //Z
}

// ///////////////////////////////////////////////////////////////////
// gnomonInteractorStyleXYZ
// ///////////////////////////////////////////////////////////////////

gnomonInteractorStyleXYZ::gnomonInteractorStyleXYZ(void) : gnomonInteractorStyle(), dd(new gnomonInteractorStyleXYZPrivate)
{
}

gnomonInteractorStyleXYZ::~gnomonInteractorStyleXYZ(void)
{
    this->SetReferenceCount(0);
}

QMap<int, QString> gnomonInteractorStyleXYZ::keyMap(void) const
{
    d->keymap.clear();

    if(d->mode=="3D") {
        d->keymap[Qt::Key_X] = "Switch to YZ axes";
        d->keymap[Qt::Key_Y] = "Switch to XZ axes";
        d->keymap[Qt::Key_Z] = "Switch to XY axes";
        d->keymap[Qt::Key_Shift] = "Use opposite camera axis";
        d->keymap[Qt::Key_Alt] = "Rotate camera by 180° ";
        d->keymap[Qt::Key_R] = "Reset camera focus";
        d->keymap[-3*Qt::LeftButton] = "Zoom in and out";
    } else if(d->mode=="2D") {
        d->keymap[Qt::Key_R] = "Reset camera focus";
        d->keymap[-3*Qt::LeftButton] = "Zoom in and out";
    }

    return d->keymap;
}

void gnomonInteractorStyleXYZ::OnMouseMove(void)
{
     vtkInteractorStyle::OnMouseMove();
}

void gnomonInteractorStyleXYZ::OnKeyDown(void)
{
    vtkInteractorStyle::OnKeyDown();
}

void gnomonInteractorStyleXYZ::OnKeyPress(void)
{
    QString key = this->Interactor->GetKeySym();
    int keycode = this->Interactor->GetKeyCode();
    qDebug()<<Q_FUNC_INFO<<key<<keycode<<this->Interactor->GetShiftKey()<<this->Interactor->GetAltKey();

    if(d->mode=="3D"){
        if ((key=="X")||(key=="x")) {
            dd->axis=0;
            this->updateCamera();
        } else if ((key=="Y")||(key=="y")) {
            dd->axis=1;
            this->updateCamera();
        } else if ((key=="Z")||(key=="z")) {
            dd->axis=2;
            this->updateCamera();
        } else {
            vtkInteractorStyle::OnKeyPress();
        }
    } else {
        vtkInteractorStyle::OnKeyPress();
    }
}

void gnomonInteractorStyleXYZ::OnKeyUp(void)
{
    vtkInteractorStyle::OnKeyUp();
}

void gnomonInteractorStyleXYZ::OnKeyRelease(void)
{
    vtkInteractorStyle::OnKeyRelease();
}

void gnomonInteractorStyleXYZ::OnChar(void)
{
    QString key = this->Interactor->GetKeySym();
    int keycode = this->Interactor->GetKeyCode();

    if ((key=="W")||(key=="w")||(key=="s")||(key=="S")||(key=="f")||(key=="F")) {
        //No surface/wireframe toggling + focus interaction
    } else {
        vtkInteractorStyle::OnChar();
    }
}

void gnomonInteractorStyleXYZ::setView(gnomonViewForm *view)
{
    gnomonInteractorStyle::setView(view);
    this->updateCamera();
}

void gnomonInteractorStyleXYZ::updateCamera(void)
{
    vtkSmartPointer<vtkCamera> cam = d->view->renderer3D()->GetActiveCamera();

    double bounds[6];
    d->view->getBounds(bounds);

    cam->SetFocalPoint((bounds[0]+bounds[1])/2,(bounds[2]+bounds[3])/2,(bounds[4]+bounds[5])/2);

    if (dd->axis==0) {
        if(this->Interactor->GetShiftKey()){
            cam->SetPosition(bounds[0],(bounds[2]+bounds[3])/2,(bounds[4]+bounds[5])/2);
        } else {
            cam->SetPosition(bounds[1],(bounds[2]+bounds[3])/2,(bounds[4]+bounds[5])/2);
        }
        if(this->Interactor->GetAltKey()){
            cam->SetViewUp(0,0,-1);
        } else {
            cam->SetViewUp(0,0,1);
        }
        d->view->renderer3D()->ResetCamera();

    } else if (dd->axis==1) {
        if(this->Interactor->GetShiftKey()){
            cam->SetPosition((bounds[0]+bounds[1])/2,bounds[3],(bounds[4]+bounds[5])/2);
        } else {
            cam->SetPosition((bounds[0]+bounds[1])/2,bounds[2],(bounds[4]+bounds[5])/2);
        }
        if(this->Interactor->GetAltKey()){
            cam->SetViewUp(0,0,-1);
        } else {
            cam->SetViewUp(0,0,1);
        }
        d->view->renderer3D()->ResetCamera();

    } else if (dd->axis==2) {
        if(this->Interactor->GetShiftKey()){
            cam->SetPosition((bounds[0]+bounds[1])/2,(bounds[2]+bounds[3])/2,bounds[4]);
        } else {
            cam->SetPosition((bounds[0]+bounds[1])/2,(bounds[2]+bounds[3])/2,bounds[5]);
        }
        if(this->Interactor->GetAltKey()){
            cam->SetViewUp(0,-1,0);
        } else {
            cam->SetViewUp(0,1,0);
        }
        d->view->renderer3D()->ResetCamera();
    }
    this->GetInteractor()->Render();
}

QString gnomonInteractorStyleXYZ::description(void) const
{
    return "XYZ axes";
}

int gnomonInteractorStyleXYZ::icon(void) const
{
    return fa::squareo;
}

//
// gnomonInteractorStyleXYZ.cpp ends here