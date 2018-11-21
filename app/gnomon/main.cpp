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

    QApplication application(argc, argv);
    application.setApplicationName("gnomon");
    application.setOrganizationName("inria");
    application.setOrganizationDomain("fr");
    application.setApplicationVersion("0.9.0");

    int stat;

    dtkLogger::instance().setLevel(dtkLog::Level::Info);
    dtkImaging::initialize();
    dtkScriptInterpreterPython::instance()->init();
    dtkScriptInterpreterPython::instance()->interpret(gnomonReadFile(":gnomon/gnomon_console.py"), &stat);

    gnomonMainWindow *window = new gnomonMainWindow;
    window->setWindowTitle("gnomon");
    window->show();
    window->raise();

    int status = application.exec();

    delete window;

    dtkImaging::uninitialize();
    dtkScriptInterpreterPython::instance()->release();

    return status;
}

//
// main.cpp ends here
