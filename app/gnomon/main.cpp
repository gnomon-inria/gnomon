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

#include <QtWidgets>

#include <dtkLog>

#include <vtkGenericOpenGLRenderWindow.h>

#include <QVTKOpenGLWidget.h>

#include "gnomonMainWindow.h"

#include <dtkScript>
#include <dtkWidgets>

#include <dtkImagingCore>

QString gnomonReadFile(const QString& path)
{
    QFile file(path);

    if(!file.open(QIODevice::ReadOnly))
        return QString();

    QString contents = file.readAll();

    file.close();

    return contents;
}

int main(int argc, char **argv)
{
    vtkOpenGLRenderWindow::SetGlobalMaximumNumberOfMultiSamples(0);

    QSurfaceFormat::setDefaultFormat(QVTKOpenGLWidget::defaultFormat());

    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    dtkApplication *application = dtkApplication::create(argc, argv);
    application->setApplicationName("gnomon");
    application->setOrganizationName("inria");
    application->setOrganizationDomain("fr");
    application->setApplicationVersion("0.9.1");

    QCommandLineParser *parser = application->parser();
    parser->setApplicationDescription("gnomon application.");

    QCommandLineOption jupyterOption("jupyter", QCoreApplication::translate("main", "start jupyter console"));
    parser->addOption(jupyterOption);

    application->initialize();

    QCommandLineOption verboseOption("verbose", QCoreApplication::translate("main", "verbose plugin initialization"));

    if (parser->isSet(verboseOption)) {
        dtkImaging::setVerboseLoading(true);;
    }

    int stat;

    dtkImaging::initialize();
    dtkScriptInterpreterPython::instance()->init();
    if (parser->isSet(jupyterOption)) {
        dtkScriptInterpreterPython::instance()->interpret(gnomonReadFile(":gnomon/gnomon_console.py"), &stat);
    }

    gnomonMainWindow *window = new gnomonMainWindow;
    window->setWindowTitle("gnomon");
    window->show();
    window->raise();

    int status = application->exec();

    delete window;

    dtkImaging::uninitialize();
    dtkScriptInterpreterPython::instance()->release();

    return status;
}

//
// main.cpp ends here
