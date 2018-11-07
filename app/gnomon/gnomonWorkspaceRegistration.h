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

class gnomonWorkspaceRegistration : public QWidget
{
    Q_OBJECT

public:
     gnomonWorkspaceRegistration(QWidget *parent = nullptr);
    ~gnomonWorkspaceRegistration(void);

private:
    class gnomonWorkspaceRegistrationPrivate *d;
};

//
// gnomonWorkspaceRegistration.h ends here
