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

#include "gnomonVisualizations/gnomonLString/gnomonAbstractMatplotlibVisualizationLString.h"

// /////////////////////////////////////////////////////////////////////////////
// Helper functions
// /////////////////////////////////////////////////////////////////////////////

void build(QMenu *menu, dtkWidgetsMenu *w_menu)
{
    foreach(QAction *action, menu->actions()) {

        dtkWidgetsMenu *w_s_menu = 0;

        if(QMenu *s_menu = action->menu()) {

            w_s_menu = w_menu->addMenu(fa::circle, action->text());

            build(s_menu, w_s_menu);
        } else {

            if(action->isSeparator()) {
                w_menu->addSeparator();
            } else {

                dtkWidgetsMenuItem *item = w_menu->addItem(fa::dashcube, action->text());

                QObject::connect(item, SIGNAL(clicked()), action, SLOT(trigger()));
            }
        }
    }
}

QList<dtkWidgetsMenu *> build(const QString& prefix, QMenuBar *bar)
{
    QList<dtkWidgetsMenu *> menus;

    foreach(QAction *action, bar->actions()) {

        if(action->text().isEmpty())
            continue;

        dtkWidgetsMenu *w_menu;

        if(action->text().contains("File"))
            w_menu = new dtkWidgetsMenu(fa::file, QString(action->text().remove("&")));

        if(action->text().contains("Edit"))
            w_menu = new dtkWidgetsMenu(fa::edit, QString(action->text().remove("&")));

        if(action->text().contains("View"))
            w_menu = new dtkWidgetsMenu(fa::eye, QString(action->text().remove("&")));

        if(action->text().contains("Tool"))
            w_menu = new dtkWidgetsMenu(fa::gear, QString(action->text().remove("&")));

        if(action->text().contains("Help"))
            w_menu = new dtkWidgetsMenu(fa::questioncircle, QString(action->text().remove("&")));

        ::build(action->menu(), w_menu);

        menus << w_menu;
    }

    return menus;
}

dtkWidgetsMenu *build(int icon, QMenu *menu)
{
    dtkWidgetsMenu *w_menu = new dtkWidgetsMenu(icon, menu->title());

    foreach(QAction *action, menu->actions()) {

        if(action->text().isEmpty())
            continue;

        // ::build(action, w_menu);

        dtkWidgetsMenu *w_s_menu = 0;

        if(QMenu *s_menu = action->menu()) {

            w_s_menu = w_menu->addMenu(fa::circle, action->text());

            ::build(s_menu, w_s_menu);
        } else {

            if(action->isSeparator()) {
                w_menu->addSeparator();
            } else {

                dtkWidgetsMenuItem *item = w_menu->addItem(fa::dashcube, action->text());

                QObject::connect(item, SIGNAL(clicked()), action, SLOT(trigger()));
            }
        }
    }

    return w_menu;
}

// /////////////////////////////////////////////////////////////////////////////
//  gnomonHighlighterLString
// /////////////////////////////////////////////////////////////////////////////

class gnomonHighlighterLString : public QSyntaxHighlighter
{

public:
     gnomonHighlighterLString(QTextDocument *parent = nullptr);
    ~gnomonHighlighterLString(void);

public:
    void setModuleNames(const QStringList& module_names);

protected:
    void highlightBlock(const QString& text) override;

protected:
    QStringList module_names;
};

gnomonHighlighterLString::gnomonHighlighterLString(QTextDocument *parent) : QSyntaxHighlighter(parent)
{
}

gnomonHighlighterLString::~gnomonHighlighterLString(void)
{
}

void gnomonHighlighterLString::setModuleNames(const QStringList& module_names)
{
    this->module_names.clear();
    for (const auto& module_name : module_names) {
        this->module_names.append(module_name);
    }
}

void gnomonHighlighterLString::highlightBlock(const QString& text)
{
    QStringList theme_colors;
    theme_colors << "@red"  << "@green" << "@violet" <<  "@yellow"  << "@darkblue" << "@magenta";
    theme_colors << "@cyan" << "@teal"<< "@orange" << "@darkcyan" << "@blue";

    int i_module = 0;
    for (const auto& module_name : this->module_names) {
        QTextCharFormat module_format;
        module_format.setFontPointSize(24);
        module_format.setFontWeight(QFont::Bold);
        module_format.setForeground(dtkThemesEngine::instance()->color(theme_colors[i_module % theme_colors.size()]));
        i_module ++;

        QRegularExpression module_pattern(module_name);
        QRegularExpressionMatchIterator matchIterator = module_pattern.globalMatch(text);
        while (matchIterator.hasNext())
        {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(),match.capturedLength(),module_format);
        }
    }
}

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
    QWidget *rhs_area;

public:
    gnomonViewMatplotlib *axiom = nullptr;
    gnomonViewMatplotlib *target = nullptr;

public:
    QTextEdit *axiom_editor = nullptr;
    gnomonHighlighterLString *highlighter = nullptr;

    QStackedWidget *axiom_stack = nullptr;
    QWidget *axiom_widget = nullptr;

public:
    QPushButton *run_button;
    QPushButton *stop_button;
    QPushButton *rewind_button;
    QPushButton *animate_button;
    QPushButton *step_button;

    QCheckBox *use_axiom;

public:
    QList<dtkWidgetsMenu *> menus;

    dtkWidgetsMenu *tools_menu;

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

public:
    void exportAxiom(void);
};

void gnomonWorkspaceLSystemSimulatorPrivate::exportAxiom(void)
{
    QString edited_axiom = this->axiom_editor->toPlainText();

    if (edited_axiom != "") {
        gnomonLString *lstring = new gnomonLString();

        gnomonLStringSeries *lstring_series = new gnomonLStringSeries();
        lstring_series->insert(0, lstring);

        gnomonAbstractLStringData *lstring_data = gnomonCore::lStringData::pluginFactory().create("gnomonLStringDataLPy");
        lstring_data->fromString(edited_axiom);
        lstring->setData(lstring_data);

        this->axiom->setForm("gnomonLString",lstring_series);
    } else {
        if (this->axiom->form("gnomonLString")) {
            this->axiom->clearForm("gnomonLString");
        }
    }
}

gnomonWorkspaceLSystemSimulator::gnomonWorkspaceLSystemSimulator(QWidget *parent) : dtkWidgetsWorkspace(parent)
{
    d = new gnomonWorkspaceLSystemSimulatorPrivate;

    // d->spinner = new gnomonSpinner(this);
    // d->spinner->start();

/////////////////////////////////////////////////////////////////////////////

    d->axiom = new gnomonViewMatplotlib(this);

    d->axiom_editor = new QTextEdit(this);
    QFont font = d->axiom_editor->font();
    font.setPointSize(18);
    font.setFamily("Courier New");
    d->axiom_editor->setFont(font);
    d->axiom_editor->setAcceptDrops(false);

    d->highlighter = new gnomonHighlighterLString(d->axiom_editor->document());

    d->axiom_stack = new QStackedWidget(this);
    d->axiom_stack->addWidget(d->axiom);
    d->axiom_stack->addWidget(d->axiom_editor);
    d->axiom_stack->setCurrentWidget(d->axiom);

    QToolButton *axiom_figure_button = new QToolButton(this);
    axiom_figure_button->setIcon(dtkFontAwesome::instance()->icon(fa::square));
    axiom_figure_button->setToolTip("2D Form Viewer");
    QToolButton *axiom_editor_button = new QToolButton(this);
    axiom_editor_button->setIcon(dtkFontAwesome::instance()->icon(fa::edit));
    axiom_editor_button->setToolTip("Text Editor");

    QPushButton *axiom_save_button = new QPushButton("  Save  ");
    axiom_save_button->setVisible(false);

    QHBoxLayout *axiom_button_layout = new QHBoxLayout;
    axiom_button_layout->addWidget(axiom_figure_button);
    axiom_button_layout->addWidget(axiom_editor_button);
    axiom_button_layout->addStretch();
    axiom_button_layout->addWidget(axiom_save_button);

    QVBoxLayout *axiom_layout = new QVBoxLayout;
    axiom_layout->setContentsMargins(0, 0, 0, 0);
    axiom_layout->setSpacing(0);
    axiom_layout->addLayout(axiom_button_layout);
    axiom_layout->addWidget(d->axiom_stack);

    d->axiom_widget = new QWidget(this);
    d->axiom_widget->setLayout(axiom_layout);

    connect(axiom_save_button, &QToolButton::clicked, [=] (void) -> void
    {
        d->exportAxiom();
    });

    connect(axiom_figure_button, &QToolButton::clicked, [=] (void) -> void
    {
        d->exportAxiom();
        d->axiom_stack->setCurrentWidget(d->axiom);
        axiom_save_button->setVisible(false);
    });

    connect(axiom_editor_button, &QToolButton::clicked, [=] (void) -> void
    {
        d->axiom_stack->setCurrentWidget(d->axiom_editor);
        axiom_save_button->setVisible(true);
    });

    connect(d->axiom, &gnomonViewMatplotlib::formAdded, [=] (const QString& form_name)
    {
        gnomonAbstractDynamicForm *form = d->axiom->form(form_name);

        if (gnomonLStringSeries *lstring_series = dynamic_cast<gnomonLStringSeries *>(form)) {
            gnomonLString *lstring = lstring_series->current()->asLString();
            QString lstring_value = lstring->toString();

            QStringList module_names;
            for (const auto & module_id :  lstring->moduleIds()) {
                QString module_name = lstring->moduleName(module_id);
                if (! module_names.contains(module_name)) {
                    module_names.append(module_name);
                }
            }
            qDebug()<<module_names;
            d->highlighter->setModuleNames(module_names);

            d->axiom_editor->blockSignals(true);
            d->axiom_editor->setText(lstring_value);
            d->axiom_editor->blockSignals(false);
        }
    });

    connect(d->axiom, &gnomonViewMatplotlib::formRemoved, [=] (const QString& form_name)
    {
        if (form_name == "gnomonLString") {
            QStringList module_names;
            d->highlighter->setModuleNames(module_names);

            d->axiom_editor->blockSignals(true);
            d->axiom_editor->setText("");
            d->axiom_editor->blockSignals(false);
        }
    });

/////////////////////////////////////////////////////////////////////////////

    d->target = new gnomonViewMatplotlib(this);

    d->lhs = new QTabWidget(this);
    d->lhs->setTabPosition(QTabWidget::South);
    d->lhs->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    d->rhs = new QTabWidget(this);
    d->rhs->setTabPosition(QTabWidget::South);

    d->params = new QTabWidget(this);
    d->params->setTabPosition(QTabWidget::South);
    d->params->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout *params_layout = new QVBoxLayout;
    params_layout->addWidget(d->params);
    params_layout->addStretch();

    d->tools_menu = new dtkWidgetsMenu(fa::gears, "Tools");

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

    d->use_axiom = new QCheckBox("Use external axiom");
    d->use_axiom->setTristate(false);
    d->use_axiom->setChecked(true);

    d->dashboard_menu_controls = new dtkWidgetsMenuItemDIY("Controls");
    d->dashboard_menu_controls->addWidget(controls);
    d->dashboard_menu_controls->addWidget(d->use_axiom);

    d->dashboard_menu = new dtkWidgetsMenu(fa::circleo, "L-System Simulator");
    d->dashboard_menu->addItem(d->dashboard_menu_parameters);
    d->dashboard_menu->addItem(d->dashboard_menu_controls);

    d->dashboard = new dtkWidgetsMenuBarContainer(this);
    d->dashboard->navigator->deleteLater();
    d->dashboard->build(QVector<dtkWidgetsMenu *>() << d->dashboard_menu);
    d->dashboard->setFixedWidth(300);

    d->rhs_area = new QWidget(this);

    QVBoxLayout *rhs_area_layout = new QVBoxLayout(d->rhs_area);
    rhs_area_layout->setContentsMargins(0, 0, 0, 0);
    rhs_area_layout->setSpacing(0);
    rhs_area_layout->addWidget(d->rhs);

    d->splitter = new QSplitter(this);
    d->splitter->addWidget(d->lhs);
    d->splitter->addWidget(d->rhs_area);

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
    dtkScriptInterpreterPython::instance()->interpret("gnomon_lstring_data.this.disown()",&stat);
    dtkScriptInterpreterPython::instance()->interpret("gnomon_lstring.setData(gnomon_lstring_data)",&stat);

    dtkScriptInterpreterPython::instance()->interpret("import gnomonvisualization", &stat);
    dtkScriptInterpreterPython::instance()->interpret("manager = gnomonvisualization.gnomonFormManager.instance()", &stat);

    dtkScriptInterpreterPython::instance()->interpret("from openalea.lpy.gui.lpystudio import Viewer", &stat);
    dtkScriptInterpreterPython::instance()->interpret("import tempfile", &stat);

    dtkScriptInterpreterPython::instance()->interpret("dir = tempfile.TemporaryDirectory()", &stat);
    dtkScriptInterpreterPython::instance()->interpret("Viewer.saveSnapshot(dir.name+'/snapshot.png')", &stat);

    dtkScriptInterpreterPython::instance()->interpret("from imageio import imread", &stat);
    dtkScriptInterpreterPython::instance()->interpret("from PIL import Image", &stat);
    dtkScriptInterpreterPython::instance()->interpret("import numpy as np", &stat);

    dtkScriptInterpreterPython::instance()->interpret("viewer_img = imread(dir.name+'/snapshot.png')", &stat);
    dtkScriptInterpreterPython::instance()->interpret("viewer_img = np.array(Image.fromarray(viewer_img).resize((600,600)))", &stat);
    dtkScriptInterpreterPython::instance()->interpret("image = [[[rgb for rgb in c] for c in r] for r in viewer_img]", &stat);
    dtkScriptInterpreterPython::instance()->interpret("dir.cleanup()", &stat);

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

void gnomonWorkspaceLSystemSimulator::reparentAction(QMenuBar * menu, const char * menuLabel, const char * actionLabel, QPushButton * button)
{
    foreach(QAction *action, menu->actions()) {

        qDebug() << Q_FUNC_INFO << action->text();

        if(action->text() == menuLabel) {

            foreach(QAction *reaction, action->menu()->actions()) {

                if(reaction->text() == actionLabel) {

                    this->connect(button, &QPushButton::clicked, [=] (void) -> void
                    {
                        d->run_button->setEnabled(false);
                        d->step_button->setEnabled(false);
                        d->rewind_button->setEnabled(false);
                        d->animate_button->setEnabled(false);
                        d->stop_button->setEnabled(false);

                        int stat;

                        if (d->use_axiom->isChecked()) {
                            dtkScriptInterpreterPython::instance()->interpret("import gnomoncore", &stat);
                            dtkScriptInterpreterPython::instance()->interpret("from gnomoncore import gnomonLStringSeries, gnomonLString", &stat);
                            dtkScriptInterpreterPython::instance()->interpret("from gnomonvisualization import getFigureForm, addFormToFigure", &stat);

                            dtkScriptInterpreterPython::instance()->interpret("import openalea.lpy as lpy", &stat);
                            dtkScriptInterpreterPython::instance()->interpret("from openalea.lpy.gui.lpycodeeditor import LpyCodeEditor", &stat);
                            dtkScriptInterpreterPython::instance()->interpret("from PyQt5 import Qt", &stat);

                            QString get_statement = "";
                            get_statement += "form = getFigureForm('gnomonLString',";
                            get_statement += QString::number(d->axiom->figureNumber());
                            get_statement += ")";
                            dtkScriptInterpreterPython::instance()->interpret(get_statement, &stat);

                            QString axiom_statement = "";
                            axiom_statement += "if form is not None:\n";
                            axiom_statement += "  axiom_lstring = form.current().asLString()\n";
                            axiom_statement += "  gnomon_axiom = axiom_lstring.toString()\n";
                            axiom_statement += "else:\n";
                            axiom_statement += "  gnomon_axiom = None\n";
                            axiom_statement += "for top in Qt.QApplication.topLevelWidgets():\n";
                            axiom_statement += "  for editor in top.findChildren(LpyCodeEditor):\n";
                            axiom_statement += "    editor.setAxiom(gnomon_axiom)\n";
                            dtkScriptInterpreterPython::instance()->interpret(axiom_statement, &stat);
                        }

                        reaction->trigger();

                        qDebug()<<Q_FUNC_INFO<<"Run finished";

                        QString axiom_clear_statement = "";
                        axiom_clear_statement += "for top in Qt.QApplication.topLevelWidgets():\n";
                        axiom_clear_statement += "  for editor in top.findChildren(LpyCodeEditor):\n";
                        axiom_clear_statement += "    editor.setAxiom(None)\n";
                        dtkScriptInterpreterPython::instance()->interpret(axiom_clear_statement, &stat);

                        dtkScriptInterpreterPython::instance()->interpret("gnomon_lstring = gnomonLString()", &stat);
                        dtkScriptInterpreterPython::instance()->interpret("gnomon_lstring_series = gnomonLStringSeries()", &stat);
                        dtkScriptInterpreterPython::instance()->interpret("gnomon_lstring_series.insert(0, gnomon_lstring)", &stat);
                        dtkScriptInterpreterPython::instance()->interpret("gnomon_lstring_data = gnomoncore.lStringData_pluginFactory().create('gnomonLStringDataLPy')",&stat);
                        dtkScriptInterpreterPython::instance()->interpret("gnomon_lstring_data.set_lstring(lstring)",&stat);
                        dtkScriptInterpreterPython::instance()->interpret("gnomon_lstring_data.this.disown()",&stat);
                        dtkScriptInterpreterPython::instance()->interpret("gnomon_lstring.setData(gnomon_lstring_data)",&stat);

                        QString add_statement = "";
                        add_statement += "addFormToFigure(gnomon_lstring_series,'gnomonLString',";
                        add_statement += QString::number(d->target->figureNumber());
                        add_statement += ")";
                        dtkScriptInterpreterPython::instance()->interpret(add_statement, &stat);

                        d->run_button->setEnabled(true);
                        d->step_button->setEnabled(true);
                        d->rewind_button->setEnabled(true);
                        d->animate_button->setEnabled(true);
                        d->stop_button->setEnabled(true);
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

                    d->menus << ::build(fa::file, action->menu());
                }

                if(action->text() == "Help") {

                    d->menus << ::build(fa::question, action->menu());
               }

                if(action->text() == "Edit") {

                    d->in_code_bar->addMenu(::build(fa::edit, action->menu()));
                    d->in_code_bar->touch();
                }

                if(action->text() == "L-systems") {

                    foreach(QAction *reaction, action->menu()->actions()) {

                        if(reaction->text() == "Debug") {
                            dtkWidgetsMenuItem *item = d->tools_menu->addItem(fa::bug, "Debug");

                            connect(item, SIGNAL(clicked()), reaction, SLOT(trigger()));
                        }

                        if(reaction->text() == "Profile") {
                            dtkWidgetsMenuItem *item = d->tools_menu->addItem(fa::stackoverflow, "Profile");

                            connect(item, SIGNAL(clicked()), reaction, SLOT(trigger()));
                        }
                    }
                }

                if(action->text() == "View") {

                    foreach(QAction *reaction, action->menu()->actions()) {

                        if(reaction->text().contains("Tab")) {
                            reaction->trigger();
                        }
                    }
                }
            }

            this->reparentAction(window->menuBar(), "L-systems", "Run", d->run_button);
            this->reparentAction(window->menuBar(), "L-systems", "Step", d->step_button);
            this->reparentAction(window->menuBar(), "L-systems", "Rewind", d->rewind_button);
            this->reparentAction(window->menuBar(), "L-systems", "Animate", d->animate_button);
            this->reparentAction(window->menuBar(), "L-systems", "Stop", d->stop_button);

//            window->setMenuBar(0);

            d->menus << d->tools_menu;

            this->enter();
        }
    }

    if(widget->objectName() == "PGLFrameGL") {

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

        d->rhs->addTab(d->target, "LString");

        if(QMainWindow *window = dynamic_cast<QMainWindow *>(widget->parentWidget())) {

            window->statusBar()->setSizeGripEnabled(false);
            foreach(QWidget *widget, window->findChildren<QToolBar *>()) {

                if(widget->objectName() == "LocationBar")
                    continue;

                if(widget->objectName() == "LineWidthBar")
                    continue;

                if(widget->objectName() == "TransitionBar")
                    continue;

                widget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
                widget->setVisible(true); // NOTE: Does the trick! Com'on ....

                // dynamic_cast<QHBoxLayout *>(d->rhs_area->layout())->insertWidget(0, widget);

                qDebug() << "Got a toolbar!" << widget;
            }

            foreach(dtkWidgetsMenu *menu, ::build("", window->menuBar()))
                d->out_view_bar->addMenu(menu);

            d->out_view_bar->touch();
        }

        d->splitter->setSizes(QList<int>() << this->width() / 2 << this->width() / 2);
    }

    if(widget->objectName() == "LPYViewer") {

        // QWidget *window = widget->window();

        d->rhs->addTab(widget, "3D");

        // window->hide();
    }

    if(widget->objectName() == "LPYAxiomViewer") {

        // QWidget *window = widget->window();

//        d->in_axiom = new QWidget;
//
//        d->in_axiom_bar = new dtkWidgetsMenuBar(d->in_axiom);
//        d->in_axiom_bar->show();
//        d->in_axiom_bar->setInteractive(false);
//        d->in_axiom_bar->setWidth(32);
//        d->in_axiom_bar->setMargins(6);
//        // d->view_menubar->addMenu(d->menu());
//        d->in_axiom_bar->touch();
//
//        QHBoxLayout *layout = new QHBoxLayout;
//        layout->setContentsMargins(0, 0, 0, 0);
//        layout->setSpacing(0);
//        layout->addWidget(d->in_axiom_bar);
//        layout->addWidget(d->in_axiom_bar->container());
//        layout->addWidget(widget);
//
//        d->in_axiom->setLayout(layout);

//        d->lhs->addTab(d->in_axiom, "Axiom");
        d->lhs->addTab(d->axiom_widget, "Axiom");

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
    if (gnomonVisualization::matplotlibVisualizationLString::pluginFactory().keys().count() == 0) {
        return true;
    }
    int stat;
    dtkScriptInterpreterPython::instance()->interpret("import openalea.lpy", &stat);
    return (stat == 1) && (gnomonCore::lStringData::pluginFactory().keys().contains("gnomonLStringDataLPy"));
}

void gnomonWorkspaceLSystemSimulator::resizeEvent(QResizeEvent *event)
{
    d->params->setFixedHeight(event->size().height() - 225);

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
