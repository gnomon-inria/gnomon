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

#include "gnomonWorkspacePlantScan3D.h"

#include <gnomonCore>
#include <gnomonVisualization>
#include <gnomonWidgets>

#include <dtkScript>
#include <dtkWidgets>
#include <dtkWidgetsMenuBar_p.h>
#include <dtkWidgetsMenu+ux.h>

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonWorkspacePlantScan3DPrivate
{
public:
    gnomonSpinner *spinner;

public:
    QSplitter *splitter;
};

gnomonWorkspacePlantScan3D::gnomonWorkspacePlantScan3D(QWidget *parent) : dtkWidgetsWorkspace(parent)
{
    d = new gnomonWorkspacePlantScan3DPrivate;

    d->spinner = new gnomonSpinner(this);
    d->spinner->start();

    d->splitter = new QSplitter(this);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(d->spinner);
    layout->addWidget(d->splitter);

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

    QFile file(":ps3D.py");
    file.open(QIODevice::ReadOnly);
    QString script = file.readAll();
    file.close();

    QTimer::singleShot(500, [=] (void) -> void
    {
        int stat;
        dtkScriptInterpreterPython::instance()->interpret(script, &stat);
    });

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

    this->enter();
}

gnomonWorkspacePlantScan3D::~gnomonWorkspacePlantScan3D(void)
{
    delete d;
}

void gnomonWorkspacePlantScan3D::enter(void)
{

}

void gnomonWorkspacePlantScan3D::leave(void)
{

}

void gnomonWorkspacePlantScan3D::apply(void)
{

}

void gnomonWorkspacePlantScan3D::step(void)
{

}

void gnomonWorkspacePlantScan3D::reset(void)
{

}

void gnomonWorkspacePlantScan3D::fill(QWidget *widget)
{
    d->spinner->stop();
    d->spinner->hide();
    d->splitter->show();

    widget->setParent(d->splitter);

    d->splitter->addWidget(widget);

    widget->show();
}

const QColor gnomonWorkspacePlantScan3D::color = QColor("#098412");

//
// gnomonWorkspacePlantScan3D.cpp ends here
