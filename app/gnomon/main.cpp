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
#include <dtkScript>

#include <vtkGenericOpenGLRenderWindow.h>

#include <QVTKOpenGLWidget.h>

#include "gnomonMainWindow.h"

#include <dtkImagingCore>

int main(int argc, char **argv)
{
    vtkOpenGLRenderWindow::SetGlobalMaximumNumberOfMultiSamples(0);

    QSurfaceFormat::setDefaultFormat(QVTKOpenGLWidget::defaultFormat());

    QApplication application(argc, argv);
    application.setApplicationName("gnomon Growth Simulator");
    application.setOrganizationName("inria");
    application.setOrganizationDomain("fr");
    application.setApplicationVersion("0.1.0");

    dtkLogger::instance().setLevel(dtkLog::Level::Info);
    dtkImaging::initialize();

    dtkScriptInterpreterPython *interpreter = new dtkScriptInterpreterPython;

    QString command = "import gnomonImagesSerieReader";
    int stat;
    interpreter->interpret(command, &stat);

    gnomonMainWindow *window = new gnomonMainWindow;
    window->show();
    window->raise();

    int status = application.exec();

    delete interpreter;
    delete window;

    return status;
}

//
// main.cpp ends here
