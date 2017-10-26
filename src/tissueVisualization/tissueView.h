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

#include <dtkCore>
#include <dtkWidgets>

class tissueViewPrivate;

class tissueView : public dtkViewWidget
{
    Q_OBJECT

public:
     tissueView(QWidget *parent = Q_NULLPTR);
    ~tissueView(void);

public:
    QWidget *widget(void);

private:
    tissueViewPrivate *d;
};

DTK_DECLARE_OBJECT(tissueView *)

//
// tissueView.h ends here
