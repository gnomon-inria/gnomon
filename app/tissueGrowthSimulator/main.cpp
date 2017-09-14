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

#include <QtCore>
#include <QtDebug>
#include <QtWidgets>

#include <QSurfaceFormat>

#include <QVTKOpenGLWidget.h>

#include <dtkComposer/dtkComposer.h>
#include <dtkComposer/dtkComposerExtension.h>

#include "tissueGrowthSimulatorMainWindow.h"

int main(int argc, char *argv[])
{
    QSurfaceFormat::setDefaultFormat(QVTKOpenGLWidget::defaultFormat());

    QApplication application(argc, argv);


    // ///////////////////////////////////////////////////////////////////
    // Prepare composer
    // ///////////////////////////////////////////////////////////////////

    dtkComposer::node::initialize();
    dtkComposer::extension::initialize();

    // ///////////////////////////////////////////////////////////////////
    // Main window
    // ///////////////////////////////////////////////////////////////////

    tissueGrowthSimulatorMainWindow *window = new tissueGrowthSimulatorMainWindow;
    window->show();
    window->raise();

    return application.exec();
}

//
// main.cpp ends here
