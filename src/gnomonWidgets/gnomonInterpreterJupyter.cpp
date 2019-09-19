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

#include "gnomonInterpreterJupyter.h"

#include <dtkFonts>
#include <dtkThemes>
#include <dtkScript>

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class gnomonInterpreterJupyterPrivate
{
public:
    QVBoxLayout *layout;
};

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

gnomonInterpreterJupyter::gnomonInterpreterJupyter(QWidget *parent) : QFrame(parent)
{
    d = new gnomonInterpreterJupyterPrivate;

    d->layout = new QVBoxLayout(this);
    d->layout->setAlignment(Qt::AlignTop);
    d->layout->setContentsMargins(0, 0, 0, 0);

    this->setObjectName("InterpreterJupyter");

    QFile file(":gnomon/jupyter_console.py");

    if (file.open(QIODevice::ReadOnly)) {
        int stat; Q_UNUSED(stat);
        QString jupyter_script  = file.readAll();
        file.close();
        dtkScriptInterpreterPython::instance()->interpret(jupyter_script, &stat);
    } else {
        qWarning() << "Can't open jupyter console script";
    }
}

gnomonInterpreterJupyter::~gnomonInterpreterJupyter(void)
{
    delete d;
}

void gnomonInterpreterJupyter::output(const QString& result)
{
    qDebug() << Q_FUNC_INFO << result;
}

void gnomonInterpreterJupyter::addWidget(QWidget *widget)
{
    d->layout->addWidget(widget);

    this->resize(800, this->height());
}

//
// gnomonInterpreterJupyter.cpp ends here
