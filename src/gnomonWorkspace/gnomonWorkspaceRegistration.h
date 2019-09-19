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

#include "gnomonWorkspace.h"

#include <dtkWidgets>

class GNOMONWORKSPACE_EXPORT gnomonWorkspaceRegistration : public dtkWidgetsWorkspace
{
    Q_OBJECT

public:
     gnomonWorkspaceRegistration(QWidget *parent = nullptr);
    ~gnomonWorkspaceRegistration(void);

public:
    void enter(void) override;
    void leave(void) override;

public slots:
    void apply(void) override;
    void configure(const QString& text);

private:
    class gnomonWorkspaceRegistrationPrivate *d;
};

//
// gnomonWorkspaceRegistration.h ends here
