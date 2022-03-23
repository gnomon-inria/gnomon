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

#include "gnomonAlgorithmWorkspace.h"

class GNOMONWORKSPACE_EXPORT gnomonWorkspaceCellImageTracking :  public gnomonAlgorithmWorkspace
{
Q_OBJECT
    Q_CLASSINFO("description", "\
This workspace is used to compute the lineage of cells from an intensity image series \
and a segmented image series (or cell image series).\n\
\n\
It outputs the lineage tree and a cell image series where each cell knows its parents. \
\n")

public:
     gnomonWorkspaceCellImageTracking(QObject *parent = nullptr);
    ~gnomonWorkspaceCellImageTracking(void);
};

//
// gnomonWorkspaceCellImageTracking.h ends here
