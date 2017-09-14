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

class tissueCoreVisuWidgetPrivate;

class tissueCellComplexData;  // in fine tissueCellComplex

class tissueCoreVisuWidget : public QFrame
{
    Q_OBJECT

public:
     tissueCoreVisuWidget(QWidget *parent = Q_NULLPTR);
    ~tissueCoreVisuWidget(void);

    void addCellComplex(tissueCellComplexData &);  //  in fine tissueCellComplex

private:
    tissueCoreVisuWidgetPrivate *d;
};

//
// tissueCoreVisuWidget.h ends here
