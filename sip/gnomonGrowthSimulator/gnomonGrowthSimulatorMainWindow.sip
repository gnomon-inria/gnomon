class gnomonGrowthSimulatorMainWindow : public QMainWindow
{
%TypeHeaderCode
#include "gnomonGrowthSimulatorMainWindow.h"

#include <dtkViewManager.h>

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
%End

public:
    void addEditor(QWidget *);

public:
    static gnomonGrowthSimulatorMainWindow *downcast(QWidget *widget);

public:
    dtkViewManager *manager();
};
