#pragma once

#include <QtWidgets>

class dtkComposerWidget;
class tissueGrowthSimulatorPythonInterpreterWidget;
class tissueGrowthSimulatorMainToolBarPrivate;
class dtkComposerGraphNode;

class tissueGrowthSimulatorMainToolBar : public QToolBar
{
    Q_OBJECT
public:
    tissueGrowthSimulatorMainToolBar(QWidget* parent = nullptr);

    ~tissueGrowthSimulatorMainToolBar();

    void setComposerWidget(dtkComposerWidget* composer_widget);

    void setScriptWidget(tissueGrowthSimulatorPythonInterpreterWidget* script_widget);

public slots:
    void setMode(int);

public slots:
    void onEvaluatorFinished(void);
    void onEvaluatorStarted(void);
    void onEvaluatorPaused(dtkComposerGraphNode *node);

protected:
    tissueGrowthSimulatorMainToolBarPrivate* d;
};
