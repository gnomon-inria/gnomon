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

class GNOMONWORKSPACE_EXPORT gnomonWorkspacePythonSimulator : public dtkWidgetsWorkspace
{
    Q_OBJECT

public:
     gnomonWorkspacePythonSimulator(QWidget *parent = nullptr);
    ~gnomonWorkspacePythonSimulator(void);

public:
    void enter(void) override;
    void leave(void) override;
    void apply(void) override;

public:
    void run(void);
    void step(void);
    void reset(void);

public:
    void configure(QWidget *);

signals:
    void modelLoaded(void);

private:
    class gnomonWorkspacePythonSimulatorPrivate *d;
};

//
// gnomonWorkspacePythonSimulator.h ends here
