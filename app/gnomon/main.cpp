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

#include <vtkGenericOpenGLRenderWindow.h>

#include <QVTKOpenGLWidget.h>

#include "gnomonMainWindow.h"

// /////////////////////////////////////////////////////////////////////////////
// TODO: Script
// /////////////////////////////////////////////////////////////////////////////

// #include <dtkScript>

#include <dtkLog>
#include <dtkThemes>
#include <dtkWidgets>
#include <dtkImagingCore>
#include <dtkScript>

#include <gnomonCore>
#include <gnomonVisualization>
#include <gnomonWidgets>
#include <gnomonWorkspace>

#include <QtWidgets>

// /////////////////////////////////////////////////////////////////////////////
// Entry point
// /////////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv)
{
    dtk::core::registerParameters();
    dtk::widgets::initialize();
    dtkImaging::initialize();

    vtkOpenGLRenderWindow::SetGlobalMaximumNumberOfMultiSamples(0);

    QSurfaceFormat::setDefaultFormat(QVTKOpenGLWidget::defaultFormat());

    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    dtkThemesEngine::instance()->apply();

    dtkApplication *application = dtkApplication::create(argc, argv);
    application->setApplicationName("gnomon");
    application->setOrganizationName("inria");
    application->setOrganizationDomain("fr");

    QCommandLineOption jupyterOption("jupyter", QCoreApplication::translate("main", "start jupyter console"));

    QCommandLineParser *parser = application->parser();
    parser->setApplicationDescription("gnomon application.");
    parser->addOption(jupyterOption);

    application->initialize();

    QCommandLineOption verboseOption("verbose", QCoreApplication::translate("main", "verbose plugin initialization"));

    if (parser->isSet(verboseOption)) {
        dtkImaging::setVerboseLoading(true);

        dtk::widgets::setVerboseLoading(true);

        gnomonCore::setVerboseLoading(true);
        gnomonVisualization::setVerboseLoading(true);
        gnomonWidgets::setVerboseLoading(true);
    }

    gnomonCore::initialize();
    gnomonVisualization::initialize();
    gnomonWidgets::initialize();

    bool redirect_io = false; int stat;

    dtkScriptInterpreterPython::instance()->init("gnomon-core");

    // if (parser->isSet(jupyterOption))
    //     dtkScriptInterpreterPython::instance()->interpret(gnomonReadFile(":gnomon/gnomon_console.py"), &stat);

    gnomonMainWindow *window = new gnomonMainWindow;
    window->setWindowTitle("gnomon");
    window->show();
    window->raise();

    int status = application->exec();

    delete window;

    dtkWidgetsController::instance()->clear();
    dtkImaging::uninitialize();

    gnomonCore::uninitialize();
    gnomonVisualization::uninitialize();
    gnomonWidgets::uninitialize();

    // dtkScriptInterpreterPython::instance()->release();

    return status;
}

//
// main.cpp ends here
