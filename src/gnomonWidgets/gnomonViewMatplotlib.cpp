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

#include "gnomonViewMatplotlib.h"

#include <gnomonFonts>
#include <gnomonStyle>

#include <dtkScript>


// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class gnomonViewMatplotlibPrivate
{
public:
    QVBoxLayout *layout;
};

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

gnomonViewMatplotlib::gnomonViewMatplotlib(QWidget *parent) : QFrame(parent)
{
    d = new gnomonViewMatplotlibPrivate;

    d->layout = new QVBoxLayout(this);
    d->layout->setAlignment(Qt::AlignTop);
    d->layout->setContentsMargins(0, 0, 0, 0);

//    QWidget *widget = new QWidget(this);
//    widget->setLayout(d->layout);

    this->setObjectName("ViewMatplotlib");

    QFile file(":gnomon/matplotlib_figure.py");

    if (file.open(QIODevice::ReadOnly)) {
        int stat;
        QString matplotlib_script  = file.readAll();
        file.close();
        dtkScriptInterpreterPython::instance()->interpret(matplotlib_script, &stat);
    } else {
        qWarning() << "Can't open matplotlib figure script";
    }
}

gnomonViewMatplotlib::~gnomonViewMatplotlib(void)
{
    delete d;
}

void gnomonViewMatplotlib::addWidget(QWidget *widget)
{
    widget->setStyleSheet(gnomonStyleSheet());
    d->layout->addWidget(widget);
    this->resize(800,this->height());
}


//
// gnomonViewMatplotlib.cpp ends here
