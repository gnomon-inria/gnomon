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

class tissueGrowthSimulatorEditorsPanelPrivate;

class tissueGrowthSimulatorEditorsPanel : public QFrame
{
    Q_OBJECT

public:
     tissueGrowthSimulatorEditorsPanel(QWidget *parent = Q_NULLPTR);
    ~tissueGrowthSimulatorEditorsPanel(void);

private:
    tissueGrowthSimulatorEditorsPanelPrivate *d;
};

//
// tissueGrowthSimulatorEditorsPanel.h ends here
