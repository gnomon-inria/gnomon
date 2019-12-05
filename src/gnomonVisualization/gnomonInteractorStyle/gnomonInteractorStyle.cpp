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

#include "gnomonInteractorStyle.h"
#include "gnomonInteractorStyle_p.h"

#include "gnomonView/gnomonViewForm.h"

#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkInteractorStyleImage.h>

// ///////////////////////////////////////////////////////////////////
// gnomonInteractorStylePrivate
// ///////////////////////////////////////////////////////////////////

gnomonInteractorStylePrivate::gnomonInteractorStylePrivate(void)
{
    this->keymap.clear();
}

int gnomonInteractorStylePrivate::upperCase(int keycode)
{
    if((keycode>96)&&(keycode<130)) {
        return keycode-32;
    } else {
        return keycode;
    }
}

// ///////////////////////////////////////////////////////////////////
// gnomonInteractorStyle
// ///////////////////////////////////////////////////////////////////

gnomonInteractorStyle::gnomonInteractorStyle(void) : vtkInteractorStyleTrackballCamera()
{
    d = new gnomonInteractorStylePrivate;
}

gnomonInteractorStyle::~gnomonInteractorStyle(void)
{
    delete d;
    this->SetReferenceCount(0);
}

QMap<int, QString> gnomonInteractorStyle::keyMap(void) const
{
    return d->keymap;
}

void gnomonInteractorStyle::OnMouseMove(void)
{
    if(d->mode == "3D") {
        vtkInteractorStyleTrackballCamera::OnMouseMove();
    } else if (d->mode == "2D") {
        if(this->Interactor->GetShiftKey()) {
            vtkInteractorStyleTrackballCamera::OnMouseMove();
        } else {
            vtkInteractorStyle::OnMouseMove();
        }
    }
}

void gnomonInteractorStyle::OnKeyDown(void)
{
    vtkInteractorStyleTrackballCamera::OnKeyDown();
}

void gnomonInteractorStyle::OnKeyPress(void)
{
    vtkInteractorStyleTrackballCamera::OnKeyPress();
}

void gnomonInteractorStyle::OnKeyUp(void)
{
    vtkInteractorStyleTrackballCamera::OnKeyUp();
}

void gnomonInteractorStyle::OnKeyRelease(void)
{
     vtkInteractorStyleTrackballCamera::OnKeyRelease();
}

void gnomonInteractorStyle::OnChar(void)
{
    QString key = this->Interactor->GetKeySym();
    int keycode = this->Interactor->GetKeyCode();

    if ((key=="f")||(key=="F")) {
        //avoid focus interaction
    } else {
        vtkInteractorStyleTrackballCamera::OnChar();
    }
}

void gnomonInteractorStyle::setView(gnomonViewForm *view)
{
    d->view = view;

    view->interactor()->SetInteractorStyle(this);

    connect(view, &gnomonViewForm::switchedTo3D, [=] (void) {
        this->setMode("3D");
        this->SetDefaultRenderer(d->view->renderer3D());
        d->view->updateShortcutKeys();
    });

    connect(view, &gnomonViewForm::switchedTo2D, [=] (void) {
        this->setMode("2D");
        this->SetDefaultRenderer(d->view->renderer2D());
        d->view->updateShortcutKeys();
    });
}

void gnomonInteractorStyle::setMode(QString mode)
{
    d->mode = mode;
    if(d->mode == "2D") {
        d->keymap.clear();
        d->keymap[Qt::Key_R] = "Reset camera focus";
        d->keymap[Qt::Key_Shift] = "Translate parallel to camera";
        d->keymap[Qt::Key_S] = "Switch to surface rendering";
        d->keymap[Qt::Key_W] = "Switch to wireframe rendering";
        d->keymap[-3*Qt::LeftButton] = "Zoom in and out";
    } else if(d->mode == "3D") {
        d->keymap.clear();
        d->keymap[Qt::Key_R] = "Reset camera focus";
        d->keymap[Qt::Key_Control] = "Rotate around camera axis";
        d->keymap[Qt::Key_Shift] = "Translate parallel to camera";
        d->keymap[Qt::Key_S] = "Switch to surface rendering";
        d->keymap[Qt::Key_W] = "Switch to wireframe rendering";
        d->keymap[-3*Qt::LeftButton] = "Zoom in and out";
    }
}

QString gnomonInteractorStyle::description(void) const
{
    return "Default interactor";
}

fa::icon gnomonInteractorStyle::icon(void) const
{
    return fa::mousepointer;
}

//
// gnomonInteractorStyle.cpp ends here