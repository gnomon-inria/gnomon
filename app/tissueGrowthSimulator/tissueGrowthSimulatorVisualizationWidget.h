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

class tissueGrowthSimulatorVisualizationWidgetPrivate;

class tissueGrowthSimulatorVisualizationWidget : public QFrame
{
    Q_OBJECT

public:
     tissueGrowthSimulatorVisualizationWidget(QWidget *parent = Q_NULLPTR);
    ~tissueGrowthSimulatorVisualizationWidget(void);

private:
    tissueGrowthSimulatorVisualizationWidgetPrivate *d;
};

//
// tissueGrowthSimulatorVisualizationWidget.h ends here
