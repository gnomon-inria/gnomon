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

#include <dtkWidgets/dtkApplication.h>
#include <dtkComposer/dtkComposer.h>
#include <dtkComposer/dtkComposerExtension.h>

#include "tissueGrowthSimulatorMainWindow.h"

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
    // Main window
    // ///////////////////////////////////////////////////////////////////

    tissueGrowthSimulatorMainWindow *window = new tissueGrowthSimulatorMainWindow;
    window->show();
    window->raise();

    return application->exec();
}

//
// main.cpp ends here
