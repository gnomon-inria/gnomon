class gnomonViewMatplotlib : public QFrame
{
%TypeHeaderCode
#include "gnomonView/gnomonViewMatplotlib.h"

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
%End

public:
    gnomonViewMatplotlib(QWidget *parent /TransferThis/ = 0);

public:
    void addWidget(QWidget *);

public:
    static gnomonViewMatplotlib *downcast(QWidget *widget);
};
