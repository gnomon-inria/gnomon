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

class gnomonMainWindow : public QMainWindow
{
    Q_OBJECT

public:
     gnomonMainWindow(QWidget *parent = nullptr);
    ~gnomonMainWindow(void);

private:
    class gnomonMainWindowPrivate *d;
};

//
// gnomonMainWindow.h ends here
