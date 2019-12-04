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

int upperCase(int keycode)
{
    if((keycode>96)&&(keycode<130)) {
        return keycode-32;
    } else {
        return keycode;
    }
}

// ///////////////////////////////////////////////////////////////////
// gnomonInteractorStylePrivate
// ///////////////////////////////////////////////////////////////////

gnomonInteractorStylePrivate::gnomonInteractorStylePrivate(void)
{
    this->keymap.clear();
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
        QString key = this->Interactor->GetKeySym();
        if((key=="Shift_L")||(key=="Shift_R")) {
            vtkInteractorStyleTrackballCamera::OnMouseMove();
        } else {
            vtkInteractorStyle::OnMouseMove();
        }
    }
}

void gnomonInteractorStyle::OnKeyDown(void)
{
    QString key = this->Interactor->GetKeySym();
    int keycode = this->Interactor->GetKeyCode();

    qDebug()<<Q_FUNC_INFO<<key<<keycode;

    if ((keycode==Qt::Key_F)||(upperCase(keycode)==Qt::Key_F)) {
        qDebug()<<Q_FUNC_INFO<<key<<keycode;
    } else {
        vtkInteractorStyleTrackballCamera::OnKeyDown();
    }
}

void gnomonInteractorStyle::OnKeyPress(void)
{
    QString key = this->Interactor->GetKeySym();
    int keycode = this->Interactor->GetKeyCode();

    if ((keycode==Qt::Key_F)||(upperCase(keycode)==Qt::Key_F)) {
        qDebug()<<Q_FUNC_INFO<<key<<keycode;
    } else {
        vtkInteractorStyleTrackballCamera::OnKeyPress();
    }
}

void gnomonInteractorStyle::OnKeyUp(void)
{
    QString key = this->Interactor->GetKeySym();
    int keycode = this->Interactor->GetKeyCode();

    if ((keycode==Qt::Key_F)||(upperCase(keycode)==Qt::Key_F)) {
        qDebug()<<Q_FUNC_INFO<<key<<keycode;
    } else {
        vtkInteractorStyleTrackballCamera::OnKeyUp();
    }
}

void gnomonInteractorStyle::OnKeyRelease(void)
{
    QString key = this->Interactor->GetKeySym();
    int keycode = this->Interactor->GetKeyCode();

    if ((keycode==Qt::Key_F)||(upperCase(keycode)==Qt::Key_F)) {
        qDebug()<<Q_FUNC_INFO<<key<<keycode;
    } else {
        vtkInteractorStyleTrackballCamera::OnKeyRelease();
    }
}

void gnomonInteractorStyle::OnChar(void)
{
    QString key = this->Interactor->GetKeySym();
    int keycode = this->Interactor->GetKeyCode();

    if ((keycode==Qt::Key_F)||(upperCase(keycode)==Qt::Key_F)) {
        qDebug()<<Q_FUNC_INFO<<key<<keycode;
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
        d->keymap[Qt::Key_R] = "Reset camera";
        d->keymap[Qt::Key_Shift] = "Translate parallel to camera";
        d->keymap[Qt::Key_S] = "Switch to surface rendering";
        d->keymap[Qt::Key_W] = "Switch to wireframe rendering";
        d->keymap[-2*Qt::LeftButton] = "Nothing";
        d->keymap[-3*Qt::LeftButton] = "Zoom in and out";
    } else if(d->mode == "3D") {
        d->keymap.clear();
        d->keymap[Qt::Key_R] = "Reset camera";
        d->keymap[Qt::Key_Control] = "Rotate around camera axis";
        d->keymap[Qt::Key_Shift] = "Translate parallel to camera";
        d->keymap[Qt::Key_S] = "Switch to surface rendering";
        d->keymap[Qt::Key_W] = "Switch to wireframe rendering";
        d->keymap[-2*Qt::LeftButton] = "Nothing";
        d->keymap[-3*Qt::LeftButton] = "Zoom in and out";
    }
}

//
// gnomonInteractorStyle.cpp ends here