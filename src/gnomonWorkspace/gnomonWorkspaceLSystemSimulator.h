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

#include "gnomonWorkspace.h"

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

public slots:
    void step(void);
    void reset(void);

private:
    class gnomonWorkspaceLSystemSimulatorPrivate *d;
};

//
// gnomonWorkspaceLSystemSimulator.h ends here
