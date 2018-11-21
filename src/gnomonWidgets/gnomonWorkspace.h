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

#include <QtWidgets>

class GNOMONWIDGETS_EXPORT gnomonWorkspace : public QWidget
{
    Q_OBJECT

public:
     gnomonWorkspace(QWidget *parent = nullptr);
    ~gnomonWorkspace(void) = default;

public:
    virtual void enter(void) = 0;
    virtual void leave(void) = 0;
};

//
// gnomonWorkspace.h ends here
