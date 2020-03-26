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

#include <dtkWidgets>

class gnomonMainWindow : public dtkWidgetsMainWindow
{
    Q_OBJECT

public:
     gnomonMainWindow(const QString& workspace_name, QWidget *parent = nullptr);
    ~gnomonMainWindow(void);

public:
    void populate(void);

protected:
    void resizeEvent(QResizeEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    class gnomonMainWindowPrivate *d;
};

//
// gnomonMainWindow.h ends here
