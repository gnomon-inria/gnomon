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
#include <gnomonCore/gnomonAbstractEvolutionModel>

#include "gnomonCodeEditor.h"
#include "gnomonFinder.h"
#include "gnomonOverlayPane.h"
#include "gnomonOverlayPaneItem.h"
#include "gnomonViewForm.h"

#include "gnomonCoreParameter.h"
#include "gnomonWidgetsParameter.h"


class gnomonWorkspacePythonSimulatorPrivate
{
public:
    gnomonFinder *finder;
    gnomonFinderPathBar *path;
    gnomonFinderToolBar *toolbar;

public:
    gnomonCodeEditor *editor;

public:
    dtkInterpreter *terminal;

public:
    gnomonViewForm *view;

public:
    gnomonOverlayPane *pane;

public:
    gnomonFontSourceCodePro *font_source_code_pro;

public:
    QMap<QString, gnomonCoreParameter *> parameters;
};

gnomonWorkspacePythonSimulator::gnomonWorkspacePythonSimulator(QWidget *parent) : gnomonWorkspace(parent)
{
    d = new gnomonWorkspacePythonSimulatorPrivate;

    d->parameters["initial_time"] = new gnomonCoreParameterDouble(0., 0., 1000., 2., "Starting time for the simulation of the model");
    d->parameters["final_time"] = new gnomonCoreParameterDouble(1., 0., 1000., 2., "Last time for the simulation of the model");
    d->parameters["dt"] = new gnomonCoreParameterDouble(1., 0., 1., 2., "Time increment used for the step function of the model");
    d->parameters["animate"] = new gnomonCoreParameterBool(true, "Whether to display the model results at each step");


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
    d->editor->resize(600, d->editor->height());


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


    QPushButton *button = new QPushButton("Run", parent);
    button->setCheckable(true);

    QObject::connect(button, &QPushButton::clicked, [=] () {
        parent->setCursor(Qt::BusyCursor);
        this->apply();
        parent->setCursor(Qt::ArrowCursor);
    });

    gnomonOverlayPaneItem *pane_item_button = new gnomonOverlayPaneItem(parent);
    pane_item_button->setTitle("Simulation");
    pane_item_button->addWidget(button);
    pane_item_button->toggle();

    d->pane->addWidget(pane_item_button);



    // -- Organizing the whole workspace --
    QSplitter *splitter = new QSplitter(this);
    splitter->addWidget(finder);
    splitter->addWidget(d->editor);
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
    int stat;
    if (d->terminal)
    {
        d->terminal->output(dtkScriptInterpreterPython::instance()->interpret(d->editor->toPlainText(), &stat));

        for (const auto& key : gnomonCore::evolutionModel::pluginFactory().keys())
        {
            gnomonAbstractEvolutionModel * model = gnomonCore::evolutionModel::pluginFactory().create(key);

            double dt = ((gnomonCoreParameterDouble *)d->parameters["dt"])->value();
            double initial_time = ((gnomonCoreParameterDouble *)d->parameters["initial_time"])->value();
            double final_time = ((gnomonCoreParameterDouble *)d->parameters["final_time"])->value();

            model->run(initial_time,final_time,dt);

            QMap<QString, gnomonAbstractForm *> forms = model->forms();

            for (const auto& name : forms.keys())
            {
                d->view->setForm(name,forms[name]);
            }
        }
    }
}


//
// gnomonWorkspacePythonSimulator.cpp ends here
