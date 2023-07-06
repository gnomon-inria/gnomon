#pragma once

#include <gnomonWorkspaceExport>

#include "gnomonAlgorithmWorkspace.h"

#include <QtCore>
#include <QtQml>

class gnomonVtkView;

class GNOMONWORKSPACE_EXPORT gnomonWorkspaceMeshFilter : public gnomonAlgorithmWorkspace
{
    Q_OBJECT
    Q_CLASSINFO("description", "\
This workspace allows to apply an processing algorithm on a Mesh form to edit \
it.\n\
\n\
Using the Mesh provided in the left input view, the algorithm will compute \
a new Mesh, generally by applying topological, geometrical or attribute-\
related filters. The processed Mesh will then be displayed in the right \
output view.\n\
\n")

public:
     gnomonWorkspaceMeshFilter(QObject *parent = nullptr);
    ~gnomonWorkspaceMeshFilter(void);

};

//
// gnomonWorkspaceMeshFilter.h ends here
