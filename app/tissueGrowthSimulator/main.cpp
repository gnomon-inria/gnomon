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
    // Main window
    // ///////////////////////////////////////////////////////////////////

    QWidget *widget = new QWidget();
    widget->setWindowTitle("tissueGrowthSimulator");
    widget->resize(800, 600);
    widget->show();
    widget->raise();

    return application.exec();
}

//
// main.cpp ends here
