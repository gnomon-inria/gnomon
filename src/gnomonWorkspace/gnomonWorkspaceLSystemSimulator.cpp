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

public:
    QList<dtkWidgetsMenu *> menus;
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

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

    loadPluginGroup("lStringData");
}

gnomonWorkspaceLSystemSimulator::~gnomonWorkspaceLSystemSimulator(void)
{
    delete d;
}

void gnomonWorkspaceLSystemSimulator::enter(void)
{
    qDebug() << Q_FUNC_INFO << 1;

    foreach(dtkWidgetsMenu *menu, d->menus)
        dtkApp->window()->menubar()->addMenu(menu);

    qDebug() << Q_FUNC_INFO << 2;

    dtkApp->window()->menubar()->touch();

    qDebug() << Q_FUNC_INFO << 3;
}

void gnomonWorkspaceLSystemSimulator::leave(void)
{
    foreach(dtkWidgetsMenu *menu, d->menus)
        dtkApp->window()->menubar()->removeMenu(menu);

    dtkApp->window()->menubar()->touch();
}

void gnomonWorkspaceLSystemSimulator::apply(void)
{

}

void gnomonWorkspaceLSystemSimulator::apply(QWidget *view)
{
    qDebug() << Q_FUNC_INFO << 1 << view;

    int stat;
    QString current_lstring = dtkScriptInterpreterPython::instance()->interpret("print(str(lstring))", &stat);

    qDebug() << Q_FUNC_INFO << 2 << current_lstring;

    gnomonLString *lstring = new gnomonLString();

    gnomonLStringSeries *lstring_series = new gnomonLStringSeries();
    lstring_series->insert(0, lstring);

    qDebug() << Q_FUNC_INFO << 3 << current_lstring << gnomonCore::lStringData::pluginFactory().keys();

    gnomonAbstractLStringData *lstring_data = gnomonCore::lStringData::pluginFactory().create("gnomonLStringDataLPy");

    qDebug() << Q_FUNC_INFO << lstring_data;

    lstring_data->fromString(current_lstring);
    lstring->setData(lstring_data);

    qDebug() << Q_FUNC_INFO << 4 << current_lstring;

    QImage image(128, 128, QImage::Format_ARGB32);
    image.fill(Qt::black);
    view->render(&image);

    qDebug() << Q_FUNC_INFO << 5;

    gnomonFormManager::instance()->addForm(lstring_series, gnomonToolBar::lsystem_color, image);

    qDebug() << Q_FUNC_INFO << "Done";
}

void gnomonWorkspaceLSystemSimulator::fill(QWidget *widget)
{
    static QList<QWidget *> filled;

    if(filled.contains(widget))
        return;

    d->spinner->stop();
    d->spinner->hide();
    d->splitter->show();

    widget->setParent(d->splitter);

    if(QMainWindow *window = dynamic_cast<QMainWindow *>(widget)) {

        d->menus << dtkWidgetsMenuBar::build(window->objectName(), window->menuBar());

        window->menuBar()->hide();
        window->menuWidget()->hide();

        this->enter();
    }

    d->splitter->addWidget(widget);

    widget->show();

    filled << widget;
}

//
// gnomonWorkspaceLSystemSimulator.cpp ends here
