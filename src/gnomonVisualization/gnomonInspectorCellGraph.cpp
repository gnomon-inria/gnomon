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

#include "gnomonInspectorCellGraph.h"
#include "gnomonClutEditor.h"

// /////////////////////////////////////////////////////////////////
// gnomonInspectorCellGraphPrivate
// /////////////////////////////////////////////////////////////////

class gnomonInspectorCellGraphPrivate
{
public:
    gnomonClutEditor *editor;
};

// /////////////////////////////////////////////////////////////////
// gnomonInspectorCellGraph
// /////////////////////////////////////////////////////////////////

gnomonInspectorCellGraph::gnomonInspectorCellGraph(void) : gnomonInspector(), d(new gnomonInspectorCellGraphPrivate)
{
    d->editor = new gnomonClutEditor();

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(d->editor);
    this->setLayout(layout);
}

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

gnomonInspectorCellGraph::~gnomonInspectorCellGraph(void)
{
    delete d->editor;
    delete d;

    d = NULL;
}

gnomonClutEditor *gnomonInspectorCellGraph::editor(void) const
{
    return d->editor;
}

//
// gnomonInspectorCellGraph.cpp ends here
