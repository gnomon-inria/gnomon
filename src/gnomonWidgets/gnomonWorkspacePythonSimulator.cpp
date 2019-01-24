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

#include <dtkConfig.h>

#if defined(DTK_BUILD_WRAPPERS)
#include <dtkScript>
#endif

#include <dtkWidgets>
#include "gnomonWorkspacePythonSimulator.h"

#include <gnomonFonts>
#include <gnomonStyle>
#include <gnomonCore/gnomonAbstractEvolutionModel>

#include "gnomonCodeEditor.h"
#include "gnomonFinder.h"
#include "gnomonOverlayPane.h"
#include "gnomonOverlayPaneItem.h"
#include "gnomonViewForm.h"

#include "gnomonCoreParameter.h"
#include "gnomonWidgetsParameter.h"


#include <gnomonCore/gnomonAbstractForm>
#include <gnomonCore/gnomonMesh.h>


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
    void addAction(const QIcon &icon, const QObject *receiver, std::function<void(void)> function);
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

void gnomonCodeEditorToolBar::addAction(const QIcon &icon, const QObject *receiver, std::function<void(void)> function)
{
    QToolButton *button = new QToolButton(this);
    button->setIconSize(QSize(16, 16));
//    button->setFlat(true);
    button->setIcon(icon);

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
    gnomonFinder *finder;
    gnomonFinderPathBar *path;
    gnomonFinderToolBar *toolbar;

public:
    gnomonCodeEditor *editor;
    gnomonCodeEditorToolBar *editor_toolbar;

public:
    dtkInterpreter *terminal;

public:
    gnomonViewForm *view;

public:
    gnomonOverlayPane *pane;

    QFormLayout *parameter_layout;

public:
    gnomonFontAwesome *font_awesome;
    gnomonFontSourceCodePro *font_source_code_pro;

public:
    QMap<QString, gnomonCoreParameter *> parameters;
    gnomonAbstractEvolutionModel * model = nullptr;
};

gnomonWorkspacePythonSimulator::gnomonWorkspacePythonSimulator(QWidget *parent) : gnomonWorkspace(parent)
{
    d = new gnomonWorkspacePythonSimulatorPrivate;

    d->parameters["initial_time"] = new gnomonCoreParameterDouble(0., 0., 1000., 2., "Starting time for the simulation of the model");
    d->parameters["final_time"] = new gnomonCoreParameterDouble(1., 0., 1000., 2., "Last time for the simulation of the model");
    d->parameters["dt"] = new gnomonCoreParameterDouble(1., 0., 1., 2., "Time increment used for the step function of the model");
    d->parameters["animate"] = new gnomonCoreParameterBool(true, "Whether to display the model results at each step");


    d->font_awesome = new gnomonFontAwesome(this);
    d->font_awesome->initFontAwesome();
    d->font_awesome->setDefaultOption("color", QColor(GNOMON_STYLE_ACCENTCOLOR));

    d->font_source_code_pro = new gnomonFontSourceCodePro(this);
    d->font_source_code_pro->initFontSourceCodePro();

    d->finder = new gnomonFinder(this);
    d->finder->switchToTreeView();

    d->path = new gnomonFinderPathBar(this);
    d->path->setPath(QDir::currentPath());
    d->path->setFixedHeight(32);

    d->toolbar = new gnomonFinderToolBar(this);
    d->toolbar->setPath(QDir::currentPath());

    // -- Organizing the finder column --
    QHBoxLayout *toolbar_layout = new QHBoxLayout;
    toolbar_layout->setContentsMargins(0, 0, 0, 0);
    toolbar_layout->setSpacing(0);
    toolbar_layout->addWidget(d->toolbar);
    toolbar_layout->addWidget(d->path);

    QVBoxLayout *finder_layout = new QVBoxLayout;
    finder_layout->setContentsMargins(0, 0, 0, 0);
    finder_layout->setSpacing(0);
    finder_layout->addLayout(toolbar_layout);
    finder_layout->addWidget(d->finder);

    QWidget *finder = new QWidget(this);
    finder->setLayout(finder_layout);

    connect(d->finder, SIGNAL(changed(QString)), d->path,    SLOT(setPath(QString)));
    connect(d->finder, SIGNAL(changed(QString)), d->toolbar, SLOT(setPath(QString)));

    connect(d->path, SIGNAL(changed(QString)), d->finder,  SLOT(setPath(QString)));
    connect(d->path, SIGNAL(changed(QString)), d->toolbar, SLOT(setPath(QString)));

    connect(d->toolbar, SIGNAL(changed(QString)), d->finder, SLOT(setPath(QString)));
    connect(d->toolbar, SIGNAL(changed(QString)), d->path,   SLOT(setPath(QString)));
    connect(d->toolbar, SIGNAL(treeView()),       d->finder, SLOT(switchToTreeView()));
    connect(d->toolbar, SIGNAL(listView()),       d->finder, SLOT(switchToListView()));

    d->editor = new gnomonCodeEditor(this);

    connect(d->editor, SIGNAL(scriptLoaded()), this, SLOT(apply()));

    d->editor_toolbar = new gnomonCodeEditorToolBar(this);
    d->editor_toolbar->addAction(d->font_awesome->icon(fa::folderopen), this, [=] () {
        d->editor->openScript();
    });
    d->editor_toolbar->addAction(d->font_awesome->icon(fa::file), this, [=] () {
        qWarning() << "Save not implemented";
    });
    d->editor_toolbar->addAction(d->font_awesome->icon(fa::play), this, [=] () {
        this->apply();
    });

    // -- Organizing the editor column --
    QVBoxLayout *editor_layout = new QVBoxLayout;
    editor_layout->setContentsMargins(0, 0, 0, 0);
    editor_layout->setSpacing(0);
    editor_layout->addWidget(d->editor_toolbar);
    editor_layout->addWidget(d->editor);

    QWidget *editor_widget = new QWidget(this);
    editor_widget->setLayout(editor_layout);
    editor_widget->resize(800, editor_widget->height());

    d->view = new gnomonViewForm(this);

    d->terminal = new dtkInterpreter(this);
    d->terminal->setFont(d->font_source_code_pro->font(12));
    d->terminal->registerInterpreter(dtkScriptInterpreterPython::instance());

    // -- Organizing the viewer column --
    QVBoxLayout *viewer_layout = new QVBoxLayout;
    viewer_layout->setContentsMargins(0, 0, 0, 0);
    viewer_layout->setSpacing(0);
    viewer_layout->addWidget(d->view);
    viewer_layout->addWidget(d->terminal);

    QWidget *viewer = new QWidget(this);
    viewer->setLayout(viewer_layout);

    d->pane = new gnomonOverlayPane(this);

    gnomonOverlayPaneItem * ubi_param_pane = new gnomonOverlayPaneItem(this);

    QFormLayout * ubi_param_layout = new QFormLayout;
    ubi_param_layout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);

    ubi_param_pane->setTitle("Ubiquitous Simulation Parameters");
    ubi_param_pane->addLayout(ubi_param_layout);
    ubi_param_pane->toggle();

    for(QMap<QString, gnomonCoreParameter*>::iterator it = d->parameters.begin(), it_end = d->parameters.end(); it != it_end; ++it) {
        QWidget *widget = gnomonWidgetsParameter::widget(it.value(), parent);
        if (widget) {
            ubi_param_layout->addRow(it.key(), widget);
        }
    }
    d->pane->addWidget(ubi_param_pane);

    d->parameter_layout = new QFormLayout;
    d->parameter_layout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);

    gnomonOverlayPaneItem * param_pane = new gnomonOverlayPaneItem(this);

    param_pane->setTitle("Model Parameters");
    param_pane->addLayout(d->parameter_layout);
    param_pane->toggle();

    d->pane->addWidget(param_pane);

    QObject::connect(this, &gnomonWorkspacePythonSimulator::modelLoaded, [=] () {
        this->configure(parent);
    });

    QPushButton *button = new QPushButton("Run", parent);
    button->setCheckable(true);

    QObject::connect(button, &QPushButton::clicked, [=] () {
        parent->setCursor(Qt::BusyCursor);
        this->run();
        parent->setCursor(Qt::ArrowCursor);
    });


    QPushButton *button_s = new QPushButton("Step", parent);
    button->setCheckable(true);

    QObject::connect(button_s, &QPushButton::clicked, [=] () {
        parent->setCursor(Qt::BusyCursor);
        this->step();
        parent->setCursor(Qt::ArrowCursor);
    });

    QPushButton *button_r = new QPushButton("Reset", parent);
    button->setCheckable(true);

    QObject::connect(button_r, &QPushButton::clicked, [=] () {
        parent->setCursor(Qt::BusyCursor);
        this->reset();
        parent->setCursor(Qt::ArrowCursor);
    });

    gnomonOverlayPaneItem *pane_item_button = new gnomonOverlayPaneItem(parent);
    pane_item_button->setTitle("Simulation");
    pane_item_button->addWidget(button);
    pane_item_button->addWidget(button_s);
    pane_item_button->addWidget(button_r);
    pane_item_button->toggle();

    d->pane->addWidget(pane_item_button);

    // -- Organizing the whole workspace --
    QSplitter *splitter = new QSplitter(this);
    splitter->addWidget(finder);
    splitter->addWidget(editor_widget);
    splitter->addWidget(viewer);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(splitter);
    layout->addWidget(d->pane);
}

gnomonWorkspacePythonSimulator::~gnomonWorkspacePythonSimulator(void)
{
    delete d;
}

void gnomonWorkspacePythonSimulator::apply(void)
{
//    gnomonCore::evolutionModel::pluginFactory().clear();

    int stat;
    if (d->terminal)
    {
        d->terminal->output(dtkScriptInterpreterPython::instance()->interpret(d->editor->toPlainText(), &stat));
    }

    // QString key = dtkScriptInterpreterPython::instance()->interpret("print(__all__[0])", &stat);

    QString key = gnomonCore::evolutionModel::pluginFactory().keys().first();
    qDebug()<<Q_FUNC_INFO<<key;
    if (d->model) {
        delete d->model;
        d->model = nullptr;
    }
    d->model = gnomonCore::evolutionModel::pluginFactory().create(key);

    if(d->view->mesh()){
        // d->model->setForm("mesh", d->view->form("gnomonMesh"));
            d->model->setForm("mesh", d->view->mesh());
    }

    d->model->reset();
    QMap<QString, gnomonAbstractForm *> forms = d->model->forms();
    for (const auto& name : forms.keys())
    {
        d->view->setForm(name,forms[name]);
    }

    emit modelLoaded();
}

void  gnomonWorkspacePythonSimulator::configure(QWidget *parent)
{

    if(d->model) {
        for(int row = 0, max_row = d->parameter_layout->count(); row < max_row; ++row) {
            QLayoutItem *forDeletion = d->parameter_layout->takeAt(0);
            forDeletion->widget()->disconnect();
            delete forDeletion->widget();
            delete forDeletion;
        }

        QMap<QString, gnomonCoreParameter *> parameters = d->model->parameters();
        for(QMap<QString, gnomonCoreParameter*>::iterator it = parameters.begin(), it_end = parameters.end(); it != it_end; ++it) {
            QWidget *widget = gnomonWidgetsParameter::widget(it.value(), parent);
            if (widget) {
                d->parameter_layout->addRow(it.key(), widget);
            }
        }
    }
}

void gnomonWorkspacePythonSimulator::run(void)
{
    double dt = ((gnomonCoreParameterDouble *)d->parameters["dt"])->value();
    double initial_time = ((gnomonCoreParameterDouble *)d->parameters["initial_time"])->value();
    double final_time = ((gnomonCoreParameterDouble *)d->parameters["final_time"])->value();
    bool animate = ((gnomonCoreParameterBool *)d->parameters["animate"])->value();

    qDebug()<<"-----> Animate: "<<animate;

    // d->model->run(initial_time,final_time,dt);

    QMap<QString, gnomonAbstractForm *> forms = d->model->forms();

    double t = initial_time;
    while (t<final_time ) {
        t = t + dt;
        qDebug()<<"Step: "<<t;
        d->model->step(t,dt);
        if (animate) {
            forms = d->model->forms();
            for (const auto& name : forms.keys())
            {
                d->view->setForm(name,forms[name]);
            }
            QCoreApplication::processEvents();
        }
    }

    if(!animate) {
        forms = d->model->forms();
        for (const auto& name : forms.keys())
        {
            d->view->setForm(name,forms[name]);
        }
    }

}

void gnomonWorkspacePythonSimulator::step(void)
{
    double dt = ((gnomonCoreParameterDouble *)d->parameters["dt"])->value();
    double initial_time = ((gnomonCoreParameterDouble *)d->parameters["initial_time"])->value();
    double final_time = ((gnomonCoreParameterDouble *)d->parameters["final_time"])->value();

    double t = initial_time;

    d->model->step(t,dt);

    QMap<QString, gnomonAbstractForm *> forms = d->model->forms();


    for (const auto& name : forms.keys())
    {
        d->view->setForm(name,forms[name]);
    }
}

void gnomonWorkspacePythonSimulator::reset(void)
{
    d->model->reset();
    qDebug()<<"Reset model";
    QMap<QString, gnomonAbstractForm *> forms = d->model->forms();

    for (const auto& name : forms.keys())
    {
        d->view->setForm(name,forms[name]);
    }
}


#include "gnomonWorkspacePythonSimulator.moc"

//
// gnomonWorkspacePythonSimulator.cpp ends here
