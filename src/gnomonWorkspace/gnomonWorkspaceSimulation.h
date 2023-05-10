#pragma once

#include <gnomonWorkspaceExport>

#include "gnomonAlgorithmWorkspace.h"

#include <QtCore>
#include <QtQml>

class gnomonVtkView;

class GNOMONWORKSPACE_EXPORT gnomonWorkspaceSimulation : public gnomonAlgorithmWorkspace
{
    Q_OBJECT
    Q_CLASSINFO("description", "\
This workspace allows to run a solver on a mesh to edit it (adding fields, ..) \n\
\n")


public:
     gnomonWorkspaceSimulation(QObject *parent = nullptr);
    ~gnomonWorkspaceSimulation(void);
};

//
// gnomonWorkspaceSimulation.h ends here
