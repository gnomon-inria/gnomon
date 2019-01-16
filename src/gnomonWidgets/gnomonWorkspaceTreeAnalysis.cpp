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
#include "gnomonViewVolumicPool.h"

class gnomonWorkspaceTreeAnalysisPrivate
{
public:
    gnomonViewVolumic *source = nullptr;
    gnomonViewVolumic *target = nullptr;
};

gnomonWorkspaceTreeAnalysis::gnomonWorkspaceTreeAnalysis(QWidget *parent) : gnomonWorkspace(parent)
{
    d = new gnomonWorkspaceTreeAnalysisPrivate;

    d->source = new gnomonViewVolumic(this);
    d->target = new gnomonViewVolumic(this);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(d->source);
    layout->addWidget(d->target);
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
