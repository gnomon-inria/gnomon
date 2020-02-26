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

#include <gnomonWorkspaceExport>

#include <dtkWidgets>

class GNOMONWORKSPACE_EXPORT gnomonWorkspaceLSystemSimulator : public dtkWidgetsWorkspace
{
    Q_OBJECT

public:
     gnomonWorkspaceLSystemSimulator(QWidget *parent = nullptr);
    ~gnomonWorkspaceLSystemSimulator(void);

public:
    void enter(void) override;
    void leave(void) override;

public:
    void apply(void) override;
    void apply(QWidget *);

public:
    void fill(QWidget *);

public:
    static const QColor color;

public:
    static bool isEmpty(void);

private:
    class gnomonWorkspaceLSystemSimulatorPrivate *d;
};

//
// gnomonWorkspaceLSystemSimulator.h ends here
