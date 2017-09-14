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

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

class tissueGrowthSimulatorMainWidgetPrivate;

class tissueGrowthSimulatorMainWidget : public QFrame
{
    Q_OBJECT

public:
     tissueGrowthSimulatorMainWidget(QWidget *parent = Q_NULLPTR);
    ~tissueGrowthSimulatorMainWidget(void);

private:
    tissueGrowthSimulatorMainWidgetPrivate *d;
};

//
// tissueGrowthSimulatorMainWidget.h ends here
