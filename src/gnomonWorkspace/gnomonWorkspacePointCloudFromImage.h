#pragma once

#include <gnomonWorkspaceExport>

#include "gnomonAlgorithmWorkspace.h"

class GNOMONWORKSPACE_EXPORT gnomonWorkspacePointCloudFromImage : public gnomonAlgorithmWorkspace
{
    Q_OBJECT
    Q_CLASSINFO("description", "\
This workspace allows to detect punctual objects in a 3D image such as spots, \
or object centers, and represents them as a Point Cloud form.\n\
\n\
The points are detected from the input Image in the left view, and the \
detected point cloud will be displayed in the right view.\n\
\n")

public:
     gnomonWorkspacePointCloudFromImage(QObject *parent = nullptr);
    ~gnomonWorkspacePointCloudFromImage(void);

};

//
// gnomonWorkspacePointCloudFromImage.h ends here
