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

class GNOMONWIDGETS_EXPORT gnomonWorkspaceSimulation : public gnomonWorkspace
{
    Q_OBJECT

public:
     gnomonWorkspaceSimulation(QWidget *parent = nullptr);
    ~gnomonWorkspaceSimulation(void);

public:
    void enter(void) override {}
    void leave(void) override {}

public slots:
    void apply(void) override;
    void configure(const QString& text);

private:
    class gnomonWorkspaceSimulationPrivate *d;
};

//
// gnomonWorkspaceSimulation.h ends here
