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
    QTabWidget *lhs;
    QTabWidget *rhs;
    QTabWidget *params;
    QSplitter *splitter;

public:
    QList<dtkWidgetsMenu *> menus;

public:
    dtkWidgetsMenu *dashboard_menu;
    dtkWidgetsMenuItemDIY *dashboard_menu_parameters;
    dtkWidgetsMenuBarContainer *dashboard;
};

gnomonWorkspaceLSystemSimulator::gnomonWorkspaceLSystemSimulator(QWidget *parent) : dtkWidgetsWorkspace(parent)
{
    d = new gnomonWorkspaceLSystemSimulatorPrivate;

    // d->spinner = new gnomonSpinner(this);
    // d->spinner->start();

    d->lhs = new QTabWidget(this);
    // d->lhs->addTab(new QWidget, "Code");
    // d->lhs->addTab(new QWidget, "Axiom");

    d->rhs = new QTabWidget(this);
    // d->rhs->addTab(new QWidget, "2D");
    // d->rhs->addTab(new QWidget, "3D");

    d->splitter = new QSplitter(this);
    d->splitter->addWidget(d->lhs);
    d->splitter->addWidget(d->rhs);

    d->params = new QTabWidget(this);

    d->dashboard_menu_parameters = new dtkWidgetsMenuItemDIY("Parameters");
    d->dashboard_menu_parameters->addWidget(d->params);
    d->dashboard_menu_parameters->setSizePolicy(QSizePolicy::Expanding);

    d->dashboard_menu = new dtkWidgetsMenu(fa::circleo, "L-System Simulator");
    d->dashboard_menu->addItem(d->dashboard_menu_parameters);

    d->dashboard = new dtkWidgetsMenuBarContainer(this);
    d->dashboard->navigator->deleteLater();
    d->dashboard->build(QVector<dtkWidgetsMenu *>() << d->dashboard_menu);
    d->dashboard->setFixedWidth(300);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    // layout->addWidget(d->spinner);
    layout->addWidget(d->splitter);
    layout->addWidget(d->dashboard);

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

    QFile file(":lpy.py");
    file.open(QIODevice::ReadOnly);
    QString script = file.readAll();
    file.close();

    qDebug() << Q_FUNC_INFO << script;

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
    foreach(dtkWidgetsMenu *menu, d->menus)
        dtkApp->window()->menubar()->addMenu(menu);

    dtkApp->window()->menubar()->touch();
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

    gnomonFormManager::instance()->addForm(lstring_series, this->color, image);

    qDebug() << Q_FUNC_INFO << "Done";
}

void gnomonWorkspaceLSystemSimulator::fill(QWidget *widget)
{
    qDebug() << Q_FUNC_INFO << widget << widget->objectName();

    static QList<QWidget *> filled;

    if(filled.contains(widget))
        return;

// /////////////////////////////////////////////////////////////////////////////
// LPYCodeEditor
// /////////////////////////////////////////////////////////////////////////////

    if(widget->objectName() == "LPYCodeEditor") {
        d->lhs->addTab(widget, "Code");
    }

// /////////////////////////////////////////////////////////////////////////////
// LPYShell
// /////////////////////////////////////////////////////////////////////////////

    if(widget->objectName() == "LPYShell") {
        d->rhs->addTab(widget, "Shell");
    }

    if(widget->objectName() == "LPYDebug") {
        d->rhs->addTab(static_cast<QDockWidget *>(widget)->widget(), "Debug");
    }

    if(widget->objectName() == "LPYParameters") {
        d->params->addTab(static_cast<QDockWidget *>(widget)->widget(), "Parameters");
    }

    if(widget->objectName() == "LPYScalars") {
        d->params->addTab(static_cast<QDockWidget *>(widget)->widget(), "Scalars");
        d->dashboard->update();
    }

    if(widget->objectName() == "LPYMaterials") {
        d->params->addTab(static_cast<QDockWidget *>(widget)->widget(), "Materials");
        d->dashboard->update();
    }

    if(widget->objectName() == "LPYMainWindow") {

        if(QMainWindow *window = dynamic_cast<QMainWindow *>(widget)) {

            d->menus << dtkWidgetsMenuBar::build(window->objectName(), window->menuBar());

            window->menuBar()->hide();
            window->menuWidget()->hide();

            this->enter();
        }
    }

    if(widget->objectName() == "PGLFrameGL") {
        d->rhs->addTab(widget, "3D");
    }

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////

    // d->spinner->stop();
    // d->spinner->hide();
    // d->splitter->show();

    // widget->setParent(d->splitter);


    // d->splitter->addWidget(widget);

// /////////////////////////////////////////////////////////////////////////////

    // widget->show();

    filled << widget;
}

const QColor gnomonWorkspaceLSystemSimulator::color = QColor("#89a348");

//
// gnomonWorkspaceLSystemSimulator.cpp ends here
