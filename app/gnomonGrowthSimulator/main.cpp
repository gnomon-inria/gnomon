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
#include <QtOpenGL>
#include <QtWidgets>

#include <dtkComposer>
#include <dtkWidgets>

#include <dtkImaging>

#include <QVTKOpenGLWidget.h>

#include "gnomonGrowthSimulatorMainWindow.h"

int main(int argc, char *argv[])
{
    QSurfaceFormat::setDefaultFormat(QVTKOpenGLWidget::defaultFormat());

    dtkApplication *application = dtkApplication::create(argc, argv);
    application->setApplicationName("TissueGrowthSimulator");
    application->setOrganizationName("inria");
    application->setOrganizationDomain("fr");
    application->setApplicationVersion("0.1.0");

    QCommandLineParser *parser = application->parser();
    parser->setApplicationDescription("Tissue Growth Simulator application.");

    application->initialize();

    QCommandLineOption verboseOption("verbose", QCoreApplication::translate("main", "verbose plugin initialization"));

    if (parser->isSet(verboseOption)) {
        dtkComposer::extension::pluginManager().setVerboseLoading(true);
    }

    // ///////////////////////////////////////////////////////////////////
    // Prepare composer
    // ///////////////////////////////////////////////////////////////////

    dtkComposer::node::initialize();
    dtkComposer::extension::initialize();

    // ///////////////////////////////////////////////////////////////////
    // Applicative layers
    // ///////////////////////////////////////////////////////////////////

    dtkImaging::initialize();

    // ///////////////////////////////////////////////////////////////////
    // Main window
    // ///////////////////////////////////////////////////////////////////

    gnomonGrowthSimulatorMainWindow *window = new gnomonGrowthSimulatorMainWindow;
    window->show();
    window->raise();

    int status = application->exec();

    delete window;

    return status;
}

//
// main.cpp ends here
