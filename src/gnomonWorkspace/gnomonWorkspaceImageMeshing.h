#pragma once

#include <gnomonWorkspaceExport>

#include "gnomonAlgorithmWorkspace.h"

#include <QtCore>
#include <QtQml>

class gnomonVtkView;

class GNOMONWORKSPACE_EXPORT gnomonWorkspaceImageMeshing : public gnomonAlgorithmWorkspace
{
Q_OBJECT
    Q_CLASSINFO("description", "\
This workspace allows to extract a Mesh based on an image-based form that \
can be either an Image, a CellImage or a BinaryImage.\n\
\n\
The plugins will typically use the information of the image(s) in the left \
view to generate a mesh representing either the surface or the volume of the \
depicted object(s). This mesh will then be displayed in the right output \
view.\n\
\n")
public:
    explicit gnomonWorkspaceImageMeshing(QObject *parent = nullptr);
    ~gnomonWorkspaceImageMeshing() override;

};

//
// gnomonWorkspaceImageMeshing.h ends here
