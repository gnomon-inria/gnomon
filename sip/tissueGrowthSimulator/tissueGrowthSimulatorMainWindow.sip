class tissueGrowthSimulatorMainWindow : public QMainWindow
{
%TypeHeaderCode
#include "tissueGrowthSimulatorMainWindow.h"

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
%End

public:
    void addEditor(QWidget *);

public:
    static tissueGrowthSimulatorMainWindow *downcast(QWidget *widget);
};
