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

#include <gnomonCore>
#include <gnomonVisualization>
#include <gnomonWidgets>

class gnomonWorkspaceTreeAnalysisPrivate
{
public:
    gnomonViewForm *source = nullptr;
    gnomonViewForm *target = nullptr;
};

gnomonWorkspaceTreeAnalysis::gnomonWorkspaceTreeAnalysis(QWidget *parent) : gnomonWorkspace(parent)
{
    d = new gnomonWorkspaceTreeAnalysisPrivate;

    d->source = new gnomonViewForm(this);
    d->target = new gnomonViewForm(this);

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
