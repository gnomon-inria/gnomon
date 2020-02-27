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

#include <dtkFonts>
#include <dtkThemes>
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
    QPushButton *run_button;
    QPushButton *stop_button;
    QPushButton *rewind_button;
    QPushButton *animate_button;
    QPushButton *step_button;

public:
    QList<dtkWidgetsMenu *> menus;

public:
    dtkWidgetsMenu *dashboard_menu;
    dtkWidgetsMenuItemDIY *dashboard_menu_parameters;
    dtkWidgetsMenuItemDIY *dashboard_menu_controls;
    dtkWidgetsMenuBarContainer *dashboard;

public:
    QWidget *in_code = nullptr;
    QWidget *in_axiom = nullptr;
    QWidget *out_view = nullptr;

    dtkWidgetsMenuBar *in_code_bar = nullptr;
    dtkWidgetsMenuBar *in_axiom_bar = nullptr;
    dtkWidgetsMenuBar *out_view_bar = nullptr;
};

gnomonWorkspaceLSystemSimulator::gnomonWorkspaceLSystemSimulator(QWidget *parent) : dtkWidgetsWorkspace(parent)
{
    d = new gnomonWorkspaceLSystemSimulatorPrivate;

    // d->spinner = new gnomonSpinner(this);
    // d->spinner->start();

    d->lhs = new QTabWidget(this);
    d->lhs->setTabPosition(QTabWidget::South);

    d->rhs = new QTabWidget(this);
    d->rhs->setTabPosition(QTabWidget::South);

    d->params = new QTabWidget(this);
    d->params->setTabPosition(QTabWidget::South);
    d->params->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout *params_layout = new QVBoxLayout;
    params_layout->addWidget(d->params);
    params_layout->addStretch();
    
    d->dashboard_menu_parameters = new dtkWidgetsMenuItemDIY("Parameters");
    d->dashboard_menu_parameters->addLayout(params_layout);
    d->dashboard_menu_parameters->setSizePolicy(QSizePolicy::Expanding);

    d->run_button = new QPushButton;
    d->run_button->setIcon(dtkFontAwesome::instance()->icon(fa::playcircleo));
    d->run_button->setStyleSheet("background: none; border: none; color: @fg");
    d->run_button->setIconSize(QSize(32,32));

    d->stop_button = new QPushButton;
    d->stop_button->setIcon(dtkFontAwesome::instance()->icon(fa::pause));
    d->stop_button->setStyleSheet("background: none; border: none; color: @fg");
    d->stop_button->setIconSize(QSize(32,32));

    d->rewind_button = new QPushButton;
    d->rewind_button->setIcon(dtkFontAwesome::instance()->icon(fa::backward));
    d->rewind_button->setStyleSheet("background: none; border: none; color: @fg");
    d->rewind_button->setIconSize(QSize(32,32));

    dtkFontAwesome::instance()->setDefaultOption("color", QColor(Qt::red));

    d->animate_button = new QPushButton;
    d->animate_button->setIcon(dtkFontAwesome::instance()->icon(fa::play));
    d->animate_button->setStyleSheet("background: none; border: none; color: @fg");
    d->animate_button->setIconSize(QSize(32,32));

    dtkFontAwesome::instance()->setDefaultOption("color", dtkThemesEngine::instance()->color("@fg"));

    d->step_button = new QPushButton;
    d->step_button->setIcon(dtkFontAwesome::instance()->icon(fa::stepforward));
    d->step_button->setStyleSheet("background: none; border: none; color: @fg");
    d->step_button->setIconSize(QSize(32,32));

    QHBoxLayout *controls_layout = new QHBoxLayout;
    controls_layout->setContentsMargins(0, 0, 0, 0);
    controls_layout->addWidget(d->run_button);
    controls_layout->addWidget(d->stop_button);
    controls_layout->addWidget(d->rewind_button);
    controls_layout->addWidget(d->animate_button);
    controls_layout->addWidget(d->step_button);
    controls_layout->setAlignment(Qt::AlignHCenter);

    QWidget *controls = new QWidget(this);
    controls->setLayout(controls_layout);

    d->dashboard_menu_controls = new dtkWidgetsMenuItemDIY("Controls");
    d->dashboard_menu_controls->addWidget(controls);

    d->dashboard_menu = new dtkWidgetsMenu(fa::circleo, "L-System Simulator");
    d->dashboard_menu->addItem(d->dashboard_menu_parameters);
    d->dashboard_menu->addItem(d->dashboard_menu_controls);

    d->dashboard = new dtkWidgetsMenuBarContainer(this);
    d->dashboard->navigator->deleteLater();
    d->dashboard->build(QVector<dtkWidgetsMenu *>() << d->dashboard_menu);
    d->dashboard->setFixedWidth(400);

    d->splitter = new QSplitter(this);
    d->splitter->addWidget(d->lhs);
    d->splitter->addWidget(d->rhs);

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
    dtkScriptInterpreterPython::instance()->interpret("import gnomoncore", &stat);
    dtkScriptInterpreterPython::instance()->interpret("from gnomoncore import gnomonLStringSeries, gnomonLString", &stat);
    dtkScriptInterpreterPython::instance()->interpret("gnomon_lstring = gnomonLString()", &stat);
    dtkScriptInterpreterPython::instance()->interpret("gnomon_lstring_series = gnomonLStringSeries()", &stat);
    dtkScriptInterpreterPython::instance()->interpret("gnomon_lstring_series.insert(0, gnomon_lstring)", &stat);
    dtkScriptInterpreterPython::instance()->interpret("gnomon_lstring_data = gnomoncore.lStringData_pluginFactory().create('gnomonLStringDataLPy')",&stat);
    dtkScriptInterpreterPython::instance()->interpret("gnomon_lstring_data.set_lstring(lstring)",&stat);
    dtkScriptInterpreterPython::instance()->interpret("gnomon_lstring.setData(gnomon_lstring_data)",&stat);

    dtkScriptInterpreterPython::instance()->interpret("import gnomonvisualization", &stat);
    dtkScriptInterpreterPython::instance()->interpret("manager = gnomonvisualization.gnomonFormManager.instance()", &stat);

    dtkScriptInterpreterPython::instance()->interpret("import numpy as np", &stat);
    dtkScriptInterpreterPython::instance()->interpret("canvas_img = np.zeros((600, 600, 3), float)", &stat);
    dtkScriptInterpreterPython::instance()->interpret("image = [[[rgb for rgb in c] for c in r] for r in canvas_img]", &stat);

    QString red_string = QString::number(this->color.red());
    QString green_string = QString::number(this->color.green());
    QString blue_string = QString::number(this->color.blue());
    QString color_statement = "color = ["+red_string+","+green_string+","+blue_string+"]";
    dtkScriptInterpreterPython::instance()->interpret(color_statement, &stat);

    dtkScriptInterpreterPython::instance()->interpret("manager.addForm(gnomon_lstring_series, color, image)", &stat);

//    gnomonLString *lstring = new gnomonLString();
//
//    gnomonLStringSeries *lstring_series = new gnomonLStringSeries();
//    lstring_series->insert(0, lstring);
//
//    qDebug() << Q_FUNC_INFO << 3 << current_lstring << gnomonCore::lStringData::pluginFactory().keys();
//
//    gnomonAbstractLStringData *lstring_data = gnomonCore::lStringData::pluginFactory().create("gnomonLStringDataLPy");
//
//    qDebug() << Q_FUNC_INFO << lstring_data;
//
//    lstring_data->fromString(current_lstring);
//    lstring->setData(lstring_data);
//
//    qDebug() << Q_FUNC_INFO << 4 << current_lstring;
//
//    QImage image(128, 128, QImage::Format_ARGB32);
//    image.fill(Qt::black);
//    view->render(&image);
//
//    qDebug() << Q_FUNC_INFO << 5 << image;
//
//    gnomonFormManager::instance()->addForm(lstring_series, this->color, image);
//
    qDebug() << Q_FUNC_INFO << "Done";
}

void reparentAction(QMenuBar * menu, const char * menuLabel, const char * actionLabel, QWidget * self,  QPushButton * button)
{
    foreach(QAction *action, menu->actions()) {

        qDebug() << Q_FUNC_INFO << action->text();

        if(action->text() == menuLabel) {

            foreach(QAction *reaction, action->menu()->actions()) {

                if(reaction->text() == actionLabel) {

                    self->connect(button, &QPushButton::clicked, [=] (void) -> void
                    {
                        reaction->trigger();
                    });
                }
            }
        }
    }    
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

        d->in_code = new QWidget(this);

        d->in_code_bar = new dtkWidgetsMenuBar(d->in_code);
        d->in_code_bar->show();
        d->in_code_bar->setInteractive(false);
        d->in_code_bar->setWidth(32);
        d->in_code_bar->setMargins(6);
     // d->in_code_bar->addMenu(d->menu());
        d->in_code_bar->touch();

        // if(QTextEdit *edit = dynamic_cast<QTextEdit *>(widget))
        //     edit->setFrameShape(QFrame::NoFrame);

        QHBoxLayout *layout = new QHBoxLayout;
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);
        layout->addWidget(d->in_code_bar);
        layout->addWidget(d->in_code_bar->container());
        layout->addWidget(widget);

        d->in_code->setLayout(layout);

        d->in_code->stackUnder(d->in_code_bar);

        d->lhs->addTab(d->in_code, "Code");

        // d->in_code_bar->setFixedHeight(widget->height());
    }

// /////////////////////////////////////////////////////////////////////////////
// LPYShell
// /////////////////////////////////////////////////////////////////////////////

    if(widget->objectName() == "LPYShell") {
        d->rhs->addTab(widget, "Shell");
    }

    else if(widget->objectName() == "LPYDebug") {
        d->rhs->addTab(static_cast<QDockWidget *>(widget)->widget(), "Debug");
    }

    else if(widget->objectName() == "LPYParameters") {
        d->params->addTab(static_cast<QDockWidget *>(widget)->widget(), "Parameters");
    }

    else if(widget->objectName() == "LPYScalars") {
        d->params->addTab(static_cast<QDockWidget *>(widget)->widget(), "Scalars");
        d->dashboard->update();
    }

    else if(widget->objectName() == "LPYCurves") {
        d->params->addTab(static_cast<QDockWidget *>(widget)->widget(), "Graphical Objects");
        d->dashboard->update();
    }

    else if(widget->objectName() == "LPYMaterials") {
        d->params->addTab(static_cast<QDockWidget *>(widget)->widget(), "Materials");
        d->dashboard->update();
    }

    else if(widget->objectName() == "LPYMainWindow") {

        if(QMainWindow *window = dynamic_cast<QMainWindow *>(widget)) {

            window->setParent(this);

            // d->menus << dtkWidgetsMenuBar::build(window->objectName(), window->menuBar());

            foreach(QAction *action, window->menuBar()->actions()) {

                qDebug() << Q_FUNC_INFO << action->text();

                if(action->text() == "File") {

                    d->menus << dtkWidgetsMenuBar::build(action->menu());
                }

                if(action->text() == "Edit") {

                    d->in_code_bar->addMenu(dtkWidgetsMenuBar::build(action->menu()));
                    d->in_code_bar->touch();
                }
            }

            reparentAction(window->menuBar(), "L-systems", "Run", this, d->run_button);
            reparentAction(window->menuBar(), "L-systems", "Step", this, d->step_button);
            reparentAction(window->menuBar(), "L-systems", "Rewind", this, d->rewind_button);
            reparentAction(window->menuBar(), "L-systems", "Animate", this, d->animate_button);
            reparentAction(window->menuBar(), "L-systems", "Stop", this, d->stop_button);


            window->menuBar()->hide();
            window->menuWidget()->hide();

            this->enter();
        }
    }

    if(widget->objectName() == "PGLFrameGL") {

        // QWidget *window = widget->window();

        d->out_view = new QWidget(this);

        d->out_view_bar = new dtkWidgetsMenuBar(d->out_view);
        d->out_view_bar->show();
        d->out_view_bar->setInteractive(false);
        d->out_view_bar->setWidth(32);
        d->out_view_bar->setMargins(6);
        // d->view_menubar->addMenu(d->menu());
        d->out_view_bar->touch();

        QHBoxLayout *layout = new QHBoxLayout;
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);
        layout->addWidget(d->out_view_bar);
        layout->addWidget(d->out_view_bar->container());
        layout->addWidget(widget->parentWidget());

        d->out_view->setLayout(layout);

        d->rhs->addTab(d->out_view, "3D");

        if(QMainWindow *window = dynamic_cast<QMainWindow *>(widget->parentWidget())) {

            foreach(dtkWidgetsMenu *menu, dtkWidgetsMenuBar::build(window->objectName(), window->menuBar()))
                d->out_view_bar->addMenu(menu);

            d->out_view_bar->touch();
        }
            // d->menus << dtkWidgetsMenuBar::build(window->objectName(), window->menuBar());


        // d->out_view_bar->setFixedHeight(d->out_view->height());
       
        // window->hide();
    }

    if(widget->objectName() == "LPYViewer") {

        // QWidget *window = widget->window();

        d->rhs->addTab(widget, "3D");

        // window->hide();
    }

    if(widget->objectName() == "LPYAxiomViewer") {

        // QWidget *window = widget->window();

        d->in_axiom = new QWidget;

        d->in_axiom_bar = new dtkWidgetsMenuBar(d->in_axiom);
        d->in_axiom_bar->show();
        d->in_axiom_bar->setInteractive(false);
        d->in_axiom_bar->setWidth(32);
        d->in_axiom_bar->setMargins(6);
        // d->view_menubar->addMenu(d->menu());
        d->in_axiom_bar->touch();

        QHBoxLayout *layout = new QHBoxLayout;
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);
        layout->addWidget(d->in_axiom_bar);
        layout->addWidget(d->in_axiom_bar->container());
        layout->addWidget(widget);

        d->in_axiom->setLayout(layout);

        d->lhs->addTab(d->in_axiom, "Axiom");

        // d->in_axiom_bar->setFixedHeight(d->in_axiom->height() + 150);
       
        // window->hide();
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

bool gnomonWorkspaceLSystemSimulator::isEmpty(void)
{
    int stat;
    dtkScriptInterpreterPython::instance()->interpret("import openalea.lpy", &stat);
    return (stat == 1) && (gnomonCore::lStringData::pluginFactory().keys().contains("gnomonLStringDataLPy"));
}

void gnomonWorkspaceLSystemSimulator::resizeEvent(QResizeEvent *event)
{
    d->params->setFixedHeight(event->size().height() - 290);

    // if (d->in_code && d->in_code_bar)
    //     d->in_code_bar->setFixedHeight(d->in_code->height());

    // if (d->in_axiom && d->in_axiom_bar)
    //     d->in_axiom_bar->setFixedHeight(d->in_axiom->height());

    // if (d->out_view && d->out_view_bar)
    //     d->out_view_bar->setFixedHeight(d->out_view->height());

    dtkWidgetsWorkspace::resizeEvent(event);
}

//
// gnomonWorkspaceLSystemSimulator.cpp ends here
