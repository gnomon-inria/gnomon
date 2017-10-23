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

class tissueCoreVisuWidget : public QWidget
{
    Q_OBJECT

public:
     tissueCoreVisuWidget(QWidget *parent = Q_NULLPTR);
    ~tissueCoreVisuWidget(void);

    void addCellComplex(tissueCellComplexData&);

private:
    tissueCoreVisuWidgetPrivate *d;
};

//
// tissueCoreVisuWidget.h ends here
