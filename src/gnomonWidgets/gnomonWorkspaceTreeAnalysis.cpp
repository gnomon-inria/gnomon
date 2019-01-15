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

#include "gnomonWorkspaceTreeAnalysis.h"

#include "gnomonOverlayPane.h"
#include "gnomonOverlayPaneItem.h"
#include "gnomonViewVolumic.h"

class gnomonWorkspaceTreeAnalysisPrivate
{
public:
    gnomonViewVolumic *view;
};

gnomonWorkspaceTreeAnalysis::gnomonWorkspaceTreeAnalysis(QWidget *parent) : gnomonWorkspace(parent)
{

}

gnomonWorkspaceTreeAnalysis::~gnomonWorkspaceTreeAnalysis(void)
{
    delete d;
}

void gnomonWorkspaceTreeAnalysis::apply(void)
{

}

//
// gnomonWorkspaceTreeAnalysis.cpp ends here
