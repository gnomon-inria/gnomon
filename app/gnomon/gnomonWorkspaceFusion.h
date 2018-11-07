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

class gnomonWorkspaceFusion : public QWidget
{
    Q_OBJECT

public:
     gnomonWorkspaceFusion(QWidget *parent = nullptr);
    ~gnomonWorkspaceFusion(void);

private:
    class gnomonWorkspaceFusionPrivate *d;
};

//
// gnomonWorkspaceFusion.h ends here
