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

class tissueViewPrivate;

class tissueView : public dtkViewWidget
{
    Q_OBJECT

public:
     tissueView(QWidget *parent = Q_NULLPTR);
    ~tissueView(void);

public:
    void addCellComplex(tissueCellComplexData&);

public:
    QWidget *widget(void);

private:
    tissueViewPrivate *d;
};

//
// tissueView.h ends here
