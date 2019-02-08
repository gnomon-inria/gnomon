class gnomonWorkspaceLSystemSimulator : public gnomonWorkspace
{
%TypeHeaderCode
#include "gnomonWorkspaceLSystemSimulator.h"
#include "gnomonWorkspace.h"

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
%End

public:
    gnomonWorkspaceLSystemSimulator(QWidget *parent /TransferThis/ = 0);

public:
    void enter();
    void leave();
    void apply();

public:
    void step();
    void reset();

public:
    void addInterpreter(QWidget *);

public:
    static gnomonWorkspaceLSystemSimulator *downcast(QWidget *widget);
};
