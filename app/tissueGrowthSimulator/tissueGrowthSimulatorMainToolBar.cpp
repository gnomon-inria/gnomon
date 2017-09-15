#include "tissueGrowthSimulatorMainToolBar.h"
#include "tissueGrowthSimulatorPythonInterpreterWidget.h"

#include <dtkComposerGraphNode.h>
#include <dtkComposerScene.h>
#include <dtkComposerWidget.h>
#include <dtkComposerEvaluator.h>
#include <dtkNotificationDisplay.h>

#include <vector>

class tissueGrowthSimulatorMainToolBarPrivate
{
public:

    dtkComposerWidget* composer;

    int current_mode;

    QAction* exit_action;

    QAction* run_composition_action;
    QAction* stop_composition_action;
    QAction* step_composition_action;
    QAction* continue_composition_action;
    QAction* next_composition_action;
    QAction* reset_composition_action;
    QAction* pause_composition_action;

    QAction* open_script_action;
    QAction* run_script_action;

    std::vector<QAction*> composer_specific_actions;
    std::vector<QAction*> script_specific_actions;
};

tissueGrowthSimulatorMainToolBar::tissueGrowthSimulatorMainToolBar(QWidget* parent) : QToolBar(parent)
{
    d = new tissueGrowthSimulatorMainToolBarPrivate;

    setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    setIconSize(QSize(32, 32));

    // Create actions and fill the toolbar.

    d->exit_action = addAction(QIcon::fromTheme("application-exit"), "Exit");

    addSeparator();

    d->run_composition_action = addAction(QIcon(":dtkComposer/pixmaps/dtkCreatorToolbarButton_Run_Active.png"), "Run");
    d->run_composition_action->setShortcut(Qt::ControlModifier + Qt::ShiftModifier + Qt::Key_R);
    d->composer_specific_actions.push_back(d->run_composition_action);

    d->step_composition_action = addAction(QIcon(":dtkComposer/pixmaps/dtkCreatorToolbarButton_Step_Active.png"), "Step");
    d->step_composition_action->setShortcut(Qt::ControlModifier + Qt::ShiftModifier + Qt::Key_N);
    d->step_composition_action->setEnabled(false);
    d->composer_specific_actions.push_back(d->step_composition_action);

    d->continue_composition_action = addAction(QIcon(":dtkComposer/pixmaps/dtkCreatorToolbarButton_Continue_Active.png"), "Cont");
    d->continue_composition_action->setShortcut(Qt::ControlModifier + Qt::ShiftModifier + Qt::Key_C);
    d->continue_composition_action->setEnabled(false);
    d->composer_specific_actions.push_back(d->continue_composition_action);

    d->next_composition_action = addAction(QIcon(":dtkComposer/pixmaps/dtkCreatorToolbarButton_Continue_Active.png"), "Next");
    d->next_composition_action->setShortcut(Qt::ControlModifier + Qt::ShiftModifier + Qt::Key_T);
    d->next_composition_action->setEnabled(false);
    d->composer_specific_actions.push_back(d->next_composition_action);

    d->pause_composition_action = addAction(QIcon(":dtkComposer/pixmaps/dtkCreatorToolbarButton_Pause_Active.png"), "Pause");
    d->pause_composition_action->setShortcut(Qt::ControlModifier + Qt::ShiftModifier + Qt::Key_Period);
    d->pause_composition_action->setEnabled(false);
    d->composer_specific_actions.push_back(d->pause_composition_action);

    d->reset_composition_action = addAction(QIcon(":dtkComposer/pixmaps/dtkCreatorToolbarButton_Reset_Active.png"), "Reset");
    d->reset_composition_action->setShortcut(Qt::ControlModifier + Qt::ShiftModifier + Qt::Key_D);
    d->composer_specific_actions.push_back(d->reset_composition_action);

    d->open_script_action = addAction(QIcon::fromTheme("document-open"), "Open script");
    d->script_specific_actions.push_back(d->open_script_action);

    d->run_script_action = addAction(QIcon::fromTheme("system-run"), "Run script");
    d->script_specific_actions.push_back(d->run_script_action);

    // Create the notification display.
    addWidget(new dtkNotificationDisplay(this));

    // Connect some actions.
    //tissueGrowthSimulatorPythonInterpreterWidget
    connect(d->exit_action, SIGNAL(triggered()), QApplication::instance(), SLOT(quit()));
}

tissueGrowthSimulatorMainToolBar::~tissueGrowthSimulatorMainToolBar()
{
    delete d;
}

void tissueGrowthSimulatorMainToolBar::setMode(int mode)
{
    d->current_mode = mode;

    bool show_composer_actions = (mode == 0);
    bool show_script_actions = (mode == 1);

    for(QAction* a : d->composer_specific_actions) {
        a->setVisible(show_composer_actions);
    }

    for(QAction* a : d->script_specific_actions) {
        a->setVisible(show_script_actions);
    }
}

void tissueGrowthSimulatorMainToolBar::setComposerWidget(dtkComposerWidget* composer)
{
    d->composer = composer;

    connect(d->run_composition_action, SIGNAL(triggered()), this, SLOT(onEvaluatorStarted()));
    connect(d->composer->evaluator(), SIGNAL(evaluationStopped()), this, SLOT(onEvaluatorFinished()));
    connect(d->composer, SIGNAL(evaluationStopped()), this, SLOT(onEvaluatorFinished()));
    connect(d->composer->evaluator(), SIGNAL(evaluationPaused(dtkComposerGraphNode *)), this, SLOT(onEvaluatorPaused(dtkComposerGraphNode *)));

    connect(d->step_composition_action, SIGNAL(triggered()), d->composer, SLOT(step()));
    connect(d->continue_composition_action, SIGNAL(triggered()), d->composer, SLOT(cont()));
    connect(d->next_composition_action, SIGNAL(triggered()), d->composer, SLOT(next()));
    connect(d->pause_composition_action, SIGNAL(triggered()), d->composer, SLOT(stop()));
    connect(d->reset_composition_action, SIGNAL(triggered()), d->composer, SLOT(reset()));
}

void tissueGrowthSimulatorMainToolBar::setScriptWidget(tissueGrowthSimulatorPythonInterpreterWidget* script_widget)
{
    connect(d->run_script_action, SIGNAL(triggered()), script_widget, SLOT(runFile()));

    connect(d->open_script_action, SIGNAL(triggered()), script_widget, SLOT(openFile()));
}

void tissueGrowthSimulatorMainToolBar::onEvaluatorPaused(dtkComposerGraphNode *node)
{
    d->step_composition_action->setEnabled(true);
    d->next_composition_action->setEnabled(true);
    d->continue_composition_action->setEnabled(true);
    d->run_composition_action->setEnabled(false);
    d->pause_composition_action->setEnabled(false);

    d->composer->scene()->onEvaluationPaused(node->wrapee());
}

void tissueGrowthSimulatorMainToolBar::onEvaluatorStarted(void)
{
    d->step_composition_action->setEnabled(false);
    d->next_composition_action->setEnabled(false);
    d->continue_composition_action->setEnabled(false);
    d->run_composition_action->setEnabled(false);
    d->pause_composition_action->setEnabled(true);

    d->composer->run();
}

void tissueGrowthSimulatorMainToolBar::onEvaluatorFinished(void)
{
    d->step_composition_action->setEnabled(false);
    d->next_composition_action->setEnabled(false);
    d->continue_composition_action->setEnabled(false);
    d->run_composition_action->setEnabled(true);
    d->pause_composition_action->setEnabled(false);

    d->composer->scene()->onEvaluationFinished();
}

