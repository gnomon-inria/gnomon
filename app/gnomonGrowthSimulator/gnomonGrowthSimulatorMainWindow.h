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

class dtkViewManager;

class gnomonGrowthSimulatorMainWindow : public QMainWindow
{
    Q_OBJECT

public:
     gnomonGrowthSimulatorMainWindow(QWidget *parent = Q_NULLPTR);
    ~gnomonGrowthSimulatorMainWindow(void);

public:
    void addEditor(QWidget *);

#if defined(Q_OS_MAC)
protected:
    void initialize(void);
#endif

public:
    static gnomonGrowthSimulatorMainWindow *downcast(QWidget *widget)
    {
        return dynamic_cast<gnomonGrowthSimulatorMainWindow *>(widget);
    }

public:
    dtkViewManager *manager(void);

private:
    class gnomonGrowthSimulatorMainWindowPrivate *d;
};

//
// gnomonGrowthSimulatorMainWindow.h ends here
