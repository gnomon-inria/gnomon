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

#include "tissueGrowthSimulatorMainWindow.h"

int main(int argc, char *argv[])
{
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
    // Main window
    // ///////////////////////////////////////////////////////////////////

    tissueGrowthSimulatorMainWindow *window = new tissueGrowthSimulatorMainWindow;
    window->setWindowTitle("tissueGrowthSimulator");
    window->resize(800, 600);
    window->show();
    window->raise();

    return application.exec();
}

//
// main.cpp ends here
