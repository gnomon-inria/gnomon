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

#include <QtWidgets>

class GNOMONWORKSPACE_EXPORT gnomonWorkspace : public QWidget
{
    Q_OBJECT

public:
     gnomonWorkspace(QWidget *parent = nullptr);
    ~gnomonWorkspace(void) = default;

public:
    virtual void enter(void) = 0;
    virtual void leave(void) = 0;

public slots:
    virtual void apply(void) = 0;
};

//
// gnomonWorkspace.h ends here
