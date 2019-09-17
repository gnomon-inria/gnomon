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

#include "gnomonWorkspacePythonSimulator.h"

#include <gnomonCore>
#include <gnomonWidgets>

// /////////////////////////////////////////////////////////////////////////////
// TODO: Visualization
// /////////////////////////////////////////////////////////////////////////////

// #include <gnomonVisualization>

// /////////////////////////////////////////////////////////////////////////////
// TODO: Script
// /////////////////////////////////////////////////////////////////////////////

// #include <dtkScript>

#include <dtkFonts>

// ///////////////////////////////////////////////////////////////////
// gnomonCodeEditorToolBar
// ///////////////////////////////////////////////////////////////////

class gnomonCodeEditorToolBar : public QFrame
{
    Q_OBJECT

public:
     gnomonCodeEditorToolBar(QWidget *parent);
    ~gnomonCodeEditorToolBar(void);

public:
    void addAction(const QIcon &icon, const QObject *receiver, const QString& tool_tip, std::function<void(void)> function);
    void addStretch(void);

private:
    QHBoxLayout *layout;
};

// ///////////////////////////////////////////////////////////////////
// gnomonCodeEditorToolBar
// ///////////////////////////////////////////////////////////////////

gnomonCodeEditorToolBar::gnomonCodeEditorToolBar(QWidget *parent) : QFrame(parent)
{
    this->layout = new QHBoxLayout(this);
    this->layout->setContentsMargins(10, 0, 10, 0);
    this->layout->setAlignment(Qt::AlignRight);

    this->setFixedHeight(36);
}

gnomonCodeEditorToolBar::~gnomonCodeEditorToolBar(void)
{

}

void gnomonCodeEditorToolBar::addAction(const QIcon &icon, const QObject *receiver, const QString & tool_tip, std::function<void(void)> function)
{
    QToolButton *button = new QToolButton(this);
    button->setIconSize(QSize(16, 16));
    button->setIcon(icon);
    button->setToolTip(tool_tip);

    connect(button, &QToolButton::clicked, receiver, function);

    this->layout->addWidget(button);
}

void gnomonCodeEditorToolBar::addStretch(void)
{
    this->layout->addStretch();
}

// ///////////////////////////////////////////////////////////////////
// gnomonWorkspacePythonSimulatorPrivate
// ///////////////////////////////////////////////////////////////////

class gnomonWorkspacePythonSimulatorPrivate
{
public:
    // gnomonCodeEditor *editor;
    // gnomonCodeEditorToolBar *editor_toolbar;

public:
    // gnomonInterpreterJupyter *terminal;

public:
    // gnomonViewForm *view;

public:
    // gnomonOverlayPane *pane;

    QFormLayout *parameter_layout = nullptr;
    QVBoxLayout *viewer_layout = nullptr;

public:
    // QMap<QString, gnomonCoreParameter *> parameters;

public:
    gnomonAbstractEvolutionModel * model = nullptr;
};

gnomonWorkspacePythonSimulator::gnomonWorkspacePythonSimulator(QWidget *parent) : dtkWidgetsWorkspace(parent)
{
    d = new gnomonWorkspacePythonSimulatorPrivate;

    // d->parameters["initial_time"] = new gnomonCoreParameterDouble(0., 0., 1000., 2., "Starting time for the simulation of the model");
    // d->parameters["final_time"] = new gnomonCoreParameterDouble(1., 0., 1000., 2., "Last time for the simulation of the model");
    // d->parameters["dt"] = new gnomonCoreParameterDouble(1., 0., 1., 2., "Time increment used for the step function of the model");
    // d->parameters["animate"] = new gnomonCoreParameterBool(true, "Whether to display the model results at each step");

    // d->editor = new gnomonCodeEditor(this);

    // connect(d->editor, SIGNAL(scriptLoaded()), this, SLOT(apply()));

    // d->editor_toolbar = new gnomonCodeEditorToolBar(this);
    // d->editor_toolbar->addAction(d->font_awesome->icon(fa::folderopen), this, "Open script", [=] () {
    //     d->editor->openScript();
    // });
    // d->editor_toolbar->addAction(d->font_awesome->icon(fa::file), this, "Save script", [=] () {
    //     d->editor->saveScript();
    // });
    // d->editor_toolbar->addAction(d->font_awesome->icon(fa::play), this, "Play script", [=] () {
    //     this->apply();
    // });

    // -- Organizing the editor column --
    
    // QVBoxLayout *editor_layout = new QVBoxLayout;
    // editor_layout->setContentsMargins(0, 0, 0, 0);
    // editor_layout->setSpacing(0);
    // editor_layout->addWidget(d->editor_toolbar);
    // editor_layout->addWidget(d->editor);

    // d->terminal = new gnomonInterpreterJupyter(this);
    // d->terminal->registerInterpreter(dtkScriptInterpreterPython::instance());

    // editor_layout->addWidget(d->terminal);

    // QWidget *editor_widget = new QWidget(this);
    // editor_widget->setLayout(editor_layout);
    // editor_widget->resize(800, editor_widget->height());

    // d->view = new gnomonViewForm(this);

    // -- Organizing the viewer column --
    // d->viewer_layout = new QVBoxLayout;
    // d->viewer_layout->setContentsMargins(0, 0, 0, 0);
    // d->viewer_layout->setSpacing(0);
    // d->viewer_layout->addWidget(d->view);

    // QWidget *viewer = new QWidget(this);
    // viewer->setLayout(d->viewer_layout);

//     d->pane = new gnomonOverlayPane(this);

//     gnomonOverlayPaneItem * ubi_param_pane = new gnomonOverlayPaneItem(this);

//     QFormLayout * ubi_param_layout = new QFormLayout;
//     ubi_param_layout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);

//     ubi_param_pane->setTitle("Ubiquitous Simulation Parameters");
//     ubi_param_pane->addLayout(ubi_param_layout);
//     ubi_param_pane->toggle();

//     for(QMap<QString, gnomonCoreParameter*>::iterator it = d->parameters.begin(), it_end = d->parameters.end(); it != it_end; ++it) {
//         QWidget *widget = gnomonWidgetsParameter::widget(it.value(), parent);
//         if (widget) {
//             ubi_param_layout->addRow(it.key(), widget);
//         }
//     }
//     d->pane->addWidget(ubi_param_pane);

//     d->parameter_layout = new QFormLayout;
//     d->parameter_layout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);

//     gnomonOverlayPaneItem * param_pane = new gnomonOverlayPaneItem(this);

//     param_pane->setTitle("Model Parameters");
//     param_pane->addLayout(d->parameter_layout);
//     param_pane->toggle();

//     d->pane->addWidget(param_pane);

//     QObject::connect(this, &gnomonWorkspacePythonSimulator::modelLoaded, [=] () {
//         this->configure(parent);
//     });

//     QPushButton *button = new QPushButton("Run", parent);
//     button->setCheckable(true);

//     QObject::connect(button, &QPushButton::clicked, [=] () {
//         parent->setCursor(Qt::BusyCursor);
//         this->run();
//         parent->setCursor(Qt::ArrowCursor);
//     });

//     QPushButton *button_s = new QPushButton("Step", parent);
//     button->setCheckable(true);

//     QObject::connect(button_s, &QPushButton::clicked, [=] () {
//         parent->setCursor(Qt::BusyCursor);
//         this->step();
//         parent->setCursor(Qt::ArrowCursor);
//     });

//     QPushButton *button_r = new QPushButton("Reset", parent);
//     button->setCheckable(true);

//     QObject::connect(button_r, &QPushButton::clicked, [=] () {
//         parent->setCursor(Qt::BusyCursor);
//         this->reset();
//         parent->setCursor(Qt::ArrowCursor);
//     });

//     gnomonOverlayPaneItem *pane_item_button = new gnomonOverlayPaneItem(parent);
//     pane_item_button->setTitle("Simulation");
//     pane_item_button->addWidget(button);
//     pane_item_button->addWidget(button_s);
//     pane_item_button->addWidget(button_r);
//     pane_item_button->toggle();

//     d->pane->addWidget(pane_item_button);

//     // -- Organizing the whole workspace --
//     QSplitter *splitter = new QSplitter(this);
// //    splitter->addWidget(finder);
//     splitter->addWidget(editor_widget);
//     splitter->addWidget(viewer);

//     QHBoxLayout *layout = new QHBoxLayout(this);
//     layout->setContentsMargins(0, 0, 0, 0);
//     layout->setSpacing(0);
//     layout->addWidget(splitter);
//     layout->addWidget(d->pane);
}

gnomonWorkspacePythonSimulator::~gnomonWorkspacePythonSimulator(void)
{
    delete d;
}

void gnomonWorkspacePythonSimulator::apply(void)
{
//    gnomonCore::evolutionModel::pluginFactory().clear();

    // int stat;

    // QString output = dtkScriptInterpreterPython::instance()->interpret(d->editor->toPlainText(), &stat);

    // qDebug()<< output;

    // QString key = gnomonCore::evolutionModel::pluginFactory().keys()[0];

    // qDebug()<<Q_FUNC_INFO<<key;

    // if (d->model) {
    //     delete d->model;
    //     d->model = nullptr;
    // }

    // d->model = gnomonCore::evolutionModel::pluginFactory().create(key);

    // if(d->view->mesh()){
    //     d->model->setForm("mesh", d->view->mesh());
    // }

    // if(d->view->cellComplex()){
    //     d->model->setForm("cellComplex", d->view->cellComplex());
    // }

    // d->model->reset();

    // QMap<QString, gnomonAbstractDynamicForm *> forms = d->model->forms();
    // for (const auto& name : forms.keys())
    // {
    //     d->view->setForm(name,forms[name]);
    // }

    // emit modelLoaded();
}

void  gnomonWorkspacePythonSimulator::configure(QWidget *parent)
{
    // if(d->model) {
    //     for(int row = 0, max_row = d->parameter_layout->count(); row < max_row; ++row) {
    //         QLayoutItem *forDeletion = d->parameter_layout->takeAt(0);
    //         forDeletion->widget()->disconnect();
    //         delete forDeletion->widget();
    //         delete forDeletion;
    //     }

    //     QMap<QString, gnomonCoreParameter *> parameters = d->model->parameters();
    //     for(QMap<QString, gnomonCoreParameter*>::iterator it = parameters.begin(), it_end = parameters.end(); it != it_end; ++it) {
    //         QWidget *widget = gnomonWidgetsParameter::widget(it.value(), parent);
    //         if (widget) {
    //             d->parameter_layout->addRow(it.key(), widget);
    //         }
    //     }
    // }
}

void gnomonWorkspacePythonSimulator::run(void)
{
    // double dt = ((gnomonCoreParameterDouble *)d->parameters["dt"])->value();
    // double initial_time = ((gnomonCoreParameterDouble *)d->parameters["initial_time"])->value();
    // double final_time = ((gnomonCoreParameterDouble *)d->parameters["final_time"])->value();
    // bool animate = ((gnomonCoreParameterBool *)d->parameters["animate"])->value();

    // qDebug()<<"-----> Animate: "<<animate;

    // // d->model->run(initial_time,final_time,dt);

    // QMap<QString, gnomonAbstractDynamicForm *> forms = d->model->forms();

    // double t = initial_time;

    // while (t < final_time) {
    //     qDebug()<<"Step: "<<t;
    //     d->model->step(t,dt);
    //     t = t + dt;

    //     if (animate) {
    //         forms = d->model->forms();
    //         for (const auto& name : forms.keys())
    //         {
    //             d->view->setForm(name,forms[name]);
    //         }
    //         d->view->onTimeChanged(t);
    //         QCoreApplication::processEvents();
    //     }
    // }

    // if(!animate) {
    //     forms = d->model->forms();
    //     for (const auto& name : forms.keys())
    //     {
    //         d->view->setForm(name,forms[name]);
    //     }
    //     d->view->onTimeChanged(t);
    // }
}

void gnomonWorkspacePythonSimulator::step(void)
{
    // double dt = ((gnomonCoreParameterDouble *)d->parameters["dt"])->value();
    // double initial_time = ((gnomonCoreParameterDouble *)d->parameters["initial_time"])->value();
    // double final_time = ((gnomonCoreParameterDouble *)d->parameters["final_time"])->value();

    // double t = initial_time;

    // d->model->step(t,dt);
    // t = t + dt;

    // QMap<QString, gnomonAbstractDynamicForm *> forms = d->model->forms();


    // for (const auto& name : forms.keys())
    // {
    //     d->view->setForm(name,forms[name]);
    // }
    //         d->view->onTimeChanged(t);
}

void gnomonWorkspacePythonSimulator::reset(void)
{
    // d->model->reset();
    // qDebug()<<"Reset model";
    // QMap<QString, gnomonAbstractDynamicForm *> forms = d->model->forms();

    // for (const auto& name : forms.keys())
    // {
    //     d->view->setForm(name,forms[name]);
    // }
}

// ///////////////////////////////////////////////////////////////////

#include "gnomonWorkspacePythonSimulator.moc"

//
// gnomonWorkspacePythonSimulator.cpp ends here
