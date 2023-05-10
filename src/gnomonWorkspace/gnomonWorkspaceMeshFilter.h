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
This workspace allows to apply an algorithm on a mesh to edit it \n\
\n")

public:
     gnomonWorkspaceMeshFilter(QObject *parent = nullptr);
    ~gnomonWorkspaceMeshFilter(void);

};

//
// gnomonWorkspaceMeshFilter.h ends here
