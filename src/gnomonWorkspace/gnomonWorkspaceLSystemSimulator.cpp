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

#include "gnomonWorkspaceLSystemSimulator.h"

#include <gnomonCore>
#include <gnomonVisualization>
#include <gnomonWidgets>

#include <dtkScript>
#include <dtkMacs>
#include <dtkWidgets>
#include <dtkWidgetsMenuBar_p.h>
#include <dtkWidgetsMenu+ux.h>

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

class gnomonWorkspaceLSystemSimulatorPrivate
{
public:
   gnomonSpinner *spinner;

public:
    QSplitter *splitter;
};

gnomonWorkspaceLSystemSimulator::gnomonWorkspaceLSystemSimulator(QWidget *parent) : dtkWidgetsWorkspace(parent)
{
    d = new gnomonWorkspaceLSystemSimulatorPrivate;

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
   
    QFile file(":lpy.py");
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

gnomonWorkspaceLSystemSimulator::~gnomonWorkspaceLSystemSimulator(void)
{
    delete d;
}

void gnomonWorkspaceLSystemSimulator::enter(void)
{

}

void gnomonWorkspaceLSystemSimulator::leave(void)
{

}

void gnomonWorkspaceLSystemSimulator::apply(void)
{
    int stat;
    QString current_lstring = dtkScriptInterpreterPython::instance()->interpret("str(lstring)",&stat);

    qDebug()<<current_lstring;
}

void gnomonWorkspaceLSystemSimulator::apply(QWidget *view)
{
// /////////////////////////////////////////////////////////////////////////////
// TODO: HERE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// /////////////////////////////////////////////////////////////////////////////

    int stat;
    QString current_lstring = dtkScriptInterpreterPython::instance()->interpret("str(lstring)",&stat);

    gnomonLStringSeries *lstring_series = new gnomonLStringSeries();
    gnomonLString *lstring = new gnomonLString();
    lstring_series->insert(0,lstring);
    gnomonAbstractLStringData *lstring_data = gnomonCore::lStringData::pluginFactory().create("gnomonLStringDataLPy");
    lstring_data->fromString(current_lstring);
    lstring->setData(lstring_data);

    QImage image(128, 128, QImage::Format_ARGB32);
    image.fill(Qt::black);
    view->render(&image);

    gnomonFormManager::instance()->addForm(lstring_series, gnomonToolBar::lsystem_color, image);
}

void gnomonWorkspaceLSystemSimulator::fill(QWidget *widget)
{
    d->spinner->stop();
    d->spinner->hide();
    d->splitter->show();

    widget->setParent(d->splitter);

    d->splitter->addWidget(widget);

    widget->show();
}

//
// gnomonWorkspaceLSystemSimulator.cpp ends here
