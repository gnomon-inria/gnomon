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

class tissueGrowthSimulatorMainWindow : public QMainWindow
{
    Q_OBJECT

public:
     tissueGrowthSimulatorMainWindow(QWidget *parent = Q_NULLPTR);
    ~tissueGrowthSimulatorMainWindow(void);

public:
    void addEditor(QWidget *);

#if defined(Q_OS_MAC)
protected:
    void initialize(void);
#endif

public:
    static tissueGrowthSimulatorMainWindow *downcast(QWidget *widget)
    {
        return dynamic_cast<tissueGrowthSimulatorMainWindow *>(widget);
    }

private:
    class tissueGrowthSimulatorMainWindowPrivate *d;
};

//
// tissueGrowthSimulatorMainWindow.h ends here
