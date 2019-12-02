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

#include <vtkRenderWindowInteractor.h>

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

class gnomonInteractorStylePrivate
{
    public:
        QMap<int, QString> keymap;

    public:
        gnomonInteractorStylePrivate(void);
};

gnomonInteractorStylePrivate::gnomonInteractorStylePrivate(void)
{
    this->keymap[Qt::Key_R] = "Reset camera";
    this->keymap[Qt::Key_Control] = "Rotate around camera axis";
    this->keymap[Qt::Key_Shift] = "Translate parallel to camera";
    this->keymap[Qt::Key_S] = "Switch to surface rendering";
    this->keymap[Qt::Key_W] = "Switch to wireframe rendering";
}

// ///////////////////////////////////////////////////////////////////
// gnomonInteractorStyle
// ///////////////////////////////////////////////////////////////////

gnomonInteractorStyle::gnomonInteractorStyle(void)
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
    vtkInteractorStyleTrackballCamera::OnMouseMove();
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

//
// gnomonInteractorStyle.cpp ends here