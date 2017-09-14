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

#include <dtkDiscreteGeometryCore>
#include <dtkImaging>
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
    // Register discrete geometry concepts
    // ///////////////////////////////////////////////////////////////////

    dtkDiscreteGeometryCoreSettings geometry_settings;
    geometry_settings.beginGroup("plugins");
    dtkDiscreteGeometryCore::initialize(geometry_settings.value("plugins").toString());
    geometry_settings.endGroup();

    // ///////////////////////////////////////////////////////////////////
    // Register imaging concepts
    // ///////////////////////////////////////////////////////////////////

    dtkImagingSettings imaging_settings;
    imaging_settings.beginGroup("plugins");
    dtkImaging::initialize(imaging_settings.value("plugins").toString());
    imaging_settings.endGroup();

    // ///////////////////////////////////////////////////////////////////
    // Prepare composer
    // ///////////////////////////////////////////////////////////////////
    //
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
