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

#include "gnomonWorkspace.h"

#include <QtWidgets>

class gnomonWorkspacePreprocess : public gnomonWorkspace
{
    Q_OBJECT

public:
     gnomonWorkspacePreprocess(QWidget *parent = nullptr);
    ~gnomonWorkspacePreprocess(void);

public:
    void enter(void) override {}
    void leave(void) override {}

public slots:
    void apply(void);

private:
    class gnomonWorkspacePreprocessPrivate *d;
};

//
// gnomonWorkspacePreprocess.h ends here
