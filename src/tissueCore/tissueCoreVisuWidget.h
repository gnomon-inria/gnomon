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

#include <dtkWidgets>

class tissueCellComplexData;

class tissueCoreVisuWidgetPrivate;

class tissueCoreVisuWidget : public dtkViewWidget
{
    Q_OBJECT

public:
     tissueCoreVisuWidget(QWidget *parent = Q_NULLPTR);
    ~tissueCoreVisuWidget(void);

public:
    void addCellComplex(tissueCellComplexData&);

public:
    QWidget *widget(void);

private:
    tissueCoreVisuWidgetPrivate *d;
};

//
// tissueCoreVisuWidget.h ends here
