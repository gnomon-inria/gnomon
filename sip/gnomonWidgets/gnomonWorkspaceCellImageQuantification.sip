class gnomonWorkspaceCellImageQuantification : public gnomonWorkspace
{
%TypeHeaderCode
#include "gnomonWorkspaceCellImageQuantification.h"
#include "gnomonWorkspace.h"

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
%End

public:
    gnomonWorkspaceCellImageQuantification(QWidget *parent /TransferThis/ = 0);

public:
    void enter();
    void leave();
    void apply();

public:
    void addView(QWidget *);

public:
    static gnomonWorkspaceCellImageQuantification *downcast(QWidget *widget);
};
