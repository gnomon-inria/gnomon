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

class GNOMONWORKSPACE_EXPORT gnomonWorkspaceTreeAnalysis : public gnomonWorkspace
{
    Q_OBJECT

public:
     gnomonWorkspaceTreeAnalysis(QWidget *parent = nullptr);
    ~gnomonWorkspaceTreeAnalysis(void);

public:
    void enter(void) override {};
    void leave(void) override {};
    void apply(void) override;

private:
    class gnomonWorkspaceTreeAnalysisPrivate *d;
};

//
// gnomonWorkspaceTreeAnalysis.h ends here
