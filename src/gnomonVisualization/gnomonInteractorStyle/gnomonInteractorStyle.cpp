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

class gnomonInteractorStylePrivate
{
    public:
        QString mode;

    public:
        QMap<int, QString> keymap;

    public slots:
        void setMode(QString);

    public:
        gnomonInteractorStylePrivate(void);
};

gnomonInteractorStylePrivate::gnomonInteractorStylePrivate(void)
{
    this->setMode("3D");
}

void gnomonInteractorStylePrivate::setMode(QString mode)
{
    this->mode = mode;
    if(this->mode == "2D") {
        this->keymap.clear();
        this->keymap[Qt::Key_R] = "Reset camera";
        this->keymap[Qt::Key_Shift] = "Translate parallel to camera";
        this->keymap[Qt::Key_S] = "Switch to surface rendering";
        this->keymap[Qt::Key_W] = "Switch to wireframe rendering";
    } else if(this->mode == "3D") {
        this->keymap.clear();
        this->keymap[Qt::Key_R] = "Reset camera";
        this->keymap[Qt::Key_Control] = "Rotate around camera axis";
        this->keymap[Qt::Key_Shift] = "Translate parallel to camera";
        this->keymap[Qt::Key_S] = "Switch to surface rendering";
        this->keymap[Qt::Key_W] = "Switch to wireframe rendering";
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
    view->interactor()->SetInteractorStyle(this);

    connect(view, &gnomonViewForm::switchedTo3D, [=] (void) {
        d->setMode("3D");
    });

    connect(view, &gnomonViewForm::switchedTo2D, [=] (void) {
        d->setMode("2D");
    });
}

//
// gnomonInteractorStyle.cpp ends here