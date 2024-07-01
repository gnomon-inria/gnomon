#pragma once

#include <gnomonWorkspaceExport>

#include "gnomonAlgorithmWorkspace.h"

#include <QtCore>
#include <QtQml>

class GNOMONWORKSPACE_EXPORT gnomonWorkspaceCellImageFilter : public gnomonAlgorithmWorkspace
{
    Q_OBJECT
    Q_CLASSINFO("description", "\
This workspace allows to apply filters on a cell image, producing a new output cell image.\n\
\n\
Using the input cell image from the left view, a filtered image will be computed \
and displayed in the right view.\n\
\n")

public:
    gnomonWorkspaceCellImageFilter(QObject *parent = nullptr);
    ~gnomonWorkspaceCellImageFilter(void);
};

//
// gnomonWorkspaceCellImageFilter.h ends here
