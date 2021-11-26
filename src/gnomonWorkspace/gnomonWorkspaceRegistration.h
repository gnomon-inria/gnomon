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

public:
     gnomonWorkspaceRegistration(QObject *parent = nullptr);
    ~gnomonWorkspaceRegistration(void);
};

//
// gnomonWorkspaceRegistration.h ends here
