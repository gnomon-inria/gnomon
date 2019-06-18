class gnomonInterpreterJupyter : public QFrame
{
%TypeHeaderCode
#include "gnomonInterpreterJupyter.h"

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
%End

public:
    gnomonInterpreterJupyter(QWidget *parent /TransferThis/ = 0);

public:
    void addWidget(QWidget *);

public:
    static gnomonInterpreterJupyter *downcast(QWidget *widget);
};
