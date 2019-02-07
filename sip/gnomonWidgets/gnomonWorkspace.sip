class gnomonWorkspace : public QWidget /Abstract/
{
%TypeHeaderCode
#include "gnomonWorkspace.h"

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
%End

public:
    gnomonWorkspace(QWidget *parent /TransferThis/ = 0);

public:
    void enter() = 0;
    void leave() = 0;

public slots:
    void apply() = 0;

};
