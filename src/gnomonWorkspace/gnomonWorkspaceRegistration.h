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

#include <QtCore>
#include <QtQml>

class gnomonViewForm;
class gnomonViewFormList;

class GNOMONWORKSPACE_EXPORT gnomonWorkspaceRegistration : public gnomonAlgorithmWorkspace
{
    Q_OBJECT
    Q_CLASSINFO("description", "Registration Workspace description from cxx")

public:
     gnomonWorkspaceRegistration(QObject *parent = nullptr);
    ~gnomonWorkspaceRegistration(void);
};

//
// gnomonWorkspaceRegistration.h ends here
