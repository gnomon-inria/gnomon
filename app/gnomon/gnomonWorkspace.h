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

#include <QtWidgets>

class gnomonWorkspace : public QWidget
{
    Q_OBJECT

public:
     gnomonWorkspace(QWidget *parent = nullptr);
    ~gnomonWorkspace(void);

public:
    virtual void enter(void) = 0;
    virtual void leave(void) = 0;

private:
    class gnomonWorkspacePrivate *d;
};

//
// gnomonWorkspace.h ends here
