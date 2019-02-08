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

#include <gnomonWidgetsExport>

#include "gnomonWorkspace.h"

class GNOMONWIDGETS_EXPORT gnomonWorkspacePythonSimulator : public gnomonWorkspace
{
    Q_OBJECT

public:
     gnomonWorkspacePythonSimulator(QWidget *parent = nullptr);
    ~gnomonWorkspacePythonSimulator(void);

public:
    void enter(void) override {};
    void leave(void) override {};
    void apply(void) override;

public:
    void run(void);
    void step(void);
    void reset(void);

public:
    void configure(QWidget *);
    void addInterpreter(QWidget *);

public:
    static gnomonWorkspacePythonSimulator *downcast(QWidget *widget)
    {
        return dynamic_cast<gnomonWorkspacePythonSimulator *>(widget);
    }

signals:
    void modelLoaded(void);

private:
    class gnomonWorkspacePythonSimulatorPrivate *d;
};

//
// gnomonWorkspacePythonSimulator.h ends here
