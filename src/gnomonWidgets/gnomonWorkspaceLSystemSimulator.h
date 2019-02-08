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

class GNOMONWIDGETS_EXPORT gnomonWorkspaceLSystemSimulator : public gnomonWorkspace
{
    Q_OBJECT

public:
     gnomonWorkspaceLSystemSimulator(QWidget *parent = nullptr);
    ~gnomonWorkspaceLSystemSimulator(void);

public:
    void enter(void) override {};
    void leave(void) override {};
    void apply(void) override;

public:
    void addInterpreter(QWidget *editor);

public slots:
    void step(void);
    void reset(void);

public:
    static gnomonWorkspaceLSystemSimulator *downcast(QWidget *widget)
    {
        return dynamic_cast<gnomonWorkspaceLSystemSimulator *>(widget);
    }

private:
    class gnomonWorkspaceLSystemSimulatorPrivate *d;
};

//
// gnomonWorkspaceLSystemSimulator.h ends here
