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

#include <gnomonWidgetsExport>

#include "gnomonWorkspace.h"

class GNOMONWIDGETS_EXPORT gnomonWorkspaceRegistration : public gnomonWorkspace
{
    Q_OBJECT

public:
     gnomonWorkspaceRegistration(QWidget *parent = nullptr);
    ~gnomonWorkspaceRegistration(void);

public:
    void enter(void) override {}
    void leave(void) override {}

public slots:
    void apply(void);

private:
    class gnomonWorkspaceRegistrationPrivate *d;
};

//
// gnomonWorkspaceRegistration.h ends here
