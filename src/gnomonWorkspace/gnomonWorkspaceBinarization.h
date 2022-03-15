
#pragma once

#include <gnomonWorkspaceExport>

#include "gnomonAlgorithmWorkspace.h"

class GNOMONWORKSPACE_EXPORT gnomonWorkspaceBinarization : public gnomonAlgorithmWorkspace
{
    Q_OBJECT
    Q_CLASSINFO("description", "\
This workspace allows to compute a BinaryImage form using the intensity \
signal of an Image form passed as input.\n\
\n\
The form that is computed is a binary mask, an image-like data containing \
only 0 and 1 values, and will be rendered in the right output view. Some \
plugins, typically using iterative algorithms, might benefit from an \
initialization, which can be provided as an optional BinaryImage in the left \
view.\n\
\n")

public:
    explicit gnomonWorkspaceBinarization(QObject *parent = nullptr);
    ~gnomonWorkspaceBinarization(void);

};

