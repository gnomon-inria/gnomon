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

#pragma once

#include <QtCore>
#include <QtWidgets>

class dtkComposerWidget;
class dtkComposerControls;
class dtkComposerNodeFactoryView;
class dtkComposerGraphView;
class dtkComposerSceneModel;
class dtkComposerSceneNodeEditor;
class dtkComposerSceneView;
class dtkComposerStackView;

class dtkDistributor;

class dtkRecentFilesMenu;

class dtkSplitter;

class dtkWidgetsLogView;

class dtkComposerViewManager;
class dtkPlotViewSettings;

class tissueComposerWidget;

class tissueComposerWidgetPrivate : public QObject
{
    Q_OBJECT

public:
    bool maySave(void);

public:
    void setCurrentFile(const QString& file);

public slots:
    void setModified(bool modified);

public:
    dtkComposerWidget *composer;
    dtkComposerControls *controls;
    dtkComposerNodeFactoryView *nodes;
    dtkComposerGraphView *graph;
    dtkComposerSceneModel *model;
    dtkComposerSceneNodeEditor *editor;
    dtkComposerSceneView *scene;
    dtkComposerStackView *stack;

public:
    dtkDistributor *distributor;

public:
    dtkComposerViewManager *view_manager;
    dtkPlotViewSettings *plot_view_settings;

public:
    dtkWidgetsLogView *log_view;

public:
    dtkSplitter *inner;

public:
    QMenu *composition_menu;
    QAction *composition_open_action;
    QAction *composition_save_action;
    QAction *composition_saveas_action;
    QAction *composition_insert_action;
    QAction *composition_quit_action;

    QMenu *edit_menu;
    QAction *undo_action;
    QAction *redo_action;

    dtkRecentFilesMenu *recent_compositions_menu;

public:
    QPushButton *compo_button;
    QPushButton *distr_button;
    QPushButton *debug_button;
    QPushButton *view_button;

public:
    bool closing;

public:
    QString current_composition;

public:
    tissueComposerWidget *q;
};

//
// tissueComposerWidget_p.h ends here
