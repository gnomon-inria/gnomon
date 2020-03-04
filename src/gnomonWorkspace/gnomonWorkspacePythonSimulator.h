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
    static const QColor color;

public:
    static bool isEmpty(void);

private:
    class gnomonWorkspacePythonSimulatorPrivate *d;
};

//
// gnomonWorkspacePythonSimulator.h ends here
