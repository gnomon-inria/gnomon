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

#include <dtkLog>
#include <dtkComposer>
#include <dtkWidgets>

#include <vtkGenericOpenGLRenderWindow.h>

#include <QVTKOpenGLWidget.h>

#include "gnomonGrowthSimulatorMainWindow.h"

int main(int argc, char *argv[])
{
    vtkOpenGLRenderWindow::SetGlobalMaximumNumberOfMultiSamples(0);

    QSurfaceFormat::setDefaultFormat(QVTKOpenGLWidget::defaultFormat());

    dtkApplication *application = dtkApplication::create(argc, argv);
    application->setApplicationName("gnomon Growth Simulator");
    application->setOrganizationName("inria");
    application->setOrganizationDomain("fr");
    application->setApplicationVersion("0.9.0");

    QCommandLineParser *parser = application->parser();
    parser->setApplicationDescription("gnomon Growth Simulator application.");

    application->initialize();

    QCommandLineOption verboseOption("verbose", QCoreApplication::translate("main", "verbose plugin initialization"));

    if (parser->isSet(verboseOption)) {
        dtkComposer::extension::pluginManager().setVerboseLoading(true);
    }

    // ///////////////////////////////////////////////////////////////////
    // Prepare composer
    // ///////////////////////////////////////////////////////////////////

    dtkComposer::node::initialize();
    dtkComposer::extension::activateObjectManager();
    dtkComposer::extension::initialize();

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
