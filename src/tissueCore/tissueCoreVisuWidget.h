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

class tissueCoreVisuWidget : public QFrame
{
    Q_OBJECT

public:
     tissueCoreVisuWidget(QWidget *parent = Q_NULLPTR);
    ~tissueCoreVisuWidget(void);

private:
    tissueCoreVisuWidgetPrivate *d;
};

//
// tissueCoreVisuWidget.h ends here
