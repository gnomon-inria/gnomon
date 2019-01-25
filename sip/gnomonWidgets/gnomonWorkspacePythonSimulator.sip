class gnomonWorkspacePythonSimulator : public gnomonWorkspace
{
%TypeHeaderCode
#include "gnomonWorkspacePythonSimulator.h"
#include "gnomonWorkspace.h"

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
%End

public:
    gnomonWorkspacePythonSimulator(QWidget *parent /TransferThis/ = 0);


public:
    void enter();
    void leave();

public slots:
    void apply();

public:
    void run();
    void step();
    void reset();

signals:
    void modelLoaded();

public:
    void configure(QWidget *);
    void addInterpreter(QWidget *);

public:
    static gnomonWorkspacePythonSimulator *downcast(QWidget *widget);

};
