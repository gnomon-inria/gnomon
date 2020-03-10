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

class dtkComposerScenePort;
class dtkComposerWidget;
class gnomonComposerWidget;
class gnomonComposerSceneNode;
class gnomonComposerNodeWorkspace;

class gnomonOverlayButton;

class gnomonComposerWidgetPrivate : public QObject
{
    Q_OBJECT

public:
    enum State {
        Collapsed,
        Expanded
    };

public:
    bool maySave(void);

public slots:
    void setModified(bool modified);
    void setCurrentFile(const QString &file);
  
public:
    dtkComposerWidget *composer;

public:
    bool closing;

public:
    QString current_composition;

public:
    bool inside = false;

public:
    State state = Collapsed;

public:
    gnomonOverlayButton *save_button = nullptr;

public:
    gnomonComposerWidget *q;
    dtkComposerSceneNode *last_node = nullptr;
//    gnomonComposerNodeWorkspace *last_node = nullptr;

public:
    dtkComposerScenePort *last_port = nullptr;
};

//
// gnomonComposerWidget_p.h ends here
