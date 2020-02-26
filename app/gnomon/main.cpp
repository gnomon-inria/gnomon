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
// TODO: Generic event filter
// /////////////////////////////////////////////////////////////////////////////

class gnomonEventFilter: public QObject
{
    Q_OBJECT

public:
     gnomonEventFilter(void) {}
    ~gnomonEventFilter(void) {}

protected:
    bool eventFilter(QObject *object, QEvent *event)
    {
        if(event->type() != QEvent::Show)
            return false;

        // static bool first = true;

        // qDebug() << Q_FUNC_INFO << object->objectName();

        if(QWidget *widget = dynamic_cast<QWidget *>(object)) {

            // if (first) {
            //     first = false;
            //     embedded << widget;
            //     return true;
            // }

            if(!embedded.contains(widget)) {

                embedded << widget;

                // widget->statusBar()->setSizeGripEnabled(false);

                // qDebug() << Q_FUNC_INFO << "OHHHH YEAH ---------" << 0 << widget->objectName();

                if(widget->objectName().startsWith("PGL") || widget->objectName().startsWith("LPY")) {

                    // qDebug() << Q_FUNC_INFO << "OHHHH YEAH ---------" << 1;

                    if(widget->objectName() == "PGLFrameGL") {

                        // qDebug() << "GOT THE VIEW";

                        // qDebug() << Q_FUNC_INFO << "OHHHH YEAH ---------" << 2;

                        gnomonOverlayButton *export_button = new gnomonOverlayButton(fa::arrowcircleup, "", widget);
                        export_button->move(10,10);
                        export_button->show();

                        connect(export_button, &gnomonOverlayButton::iconClicked, [=] (void) -> void
                        {
                            // qDebug() << Q_FUNC_INFO << 0 << widget;

                            foreach(QWidget *top, qApp->topLevelWidgets()) {

                                // qDebug() << Q_FUNC_INFO << 1 << widget;

                                foreach(gnomonWorkspaceLSystemSimulator *simulator, top->findChildren<gnomonWorkspaceLSystemSimulator *>()) {

                                    // qDebug() << Q_FUNC_INFO << 2 << widget;

                                    simulator->apply(widget);
                                }
                            }
                        });

                       
                    }

                    foreach(QWidget *top, qApp->topLevelWidgets()) {
                        foreach(gnomonWorkspaceLSystemSimulator *simulator, top->findChildren<gnomonWorkspaceLSystemSimulator *>()) {
                            simulator->fill(widget);
                        }
                    }
                }

                if(widget->objectName().startsWith("PS3D")) {
                    foreach(QWidget *top, qApp->topLevelWidgets()) {
                        foreach(gnomonWorkspacePlantScan3D *scanner, top->findChildren<gnomonWorkspacePlantScan3D *>()) {
                            ; // scanner->fill(widget);
                        }
                    }
                }

                return true;
            }
        }
       
        return false;
    }

private:
    QList<QWidget *> embedded;
};

// /////////////////////////////////////////////////////////////////////////////
// Entry point
// /////////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv)
{
    dtk::core::registerParameters();

    dtk::widgets::initialize();

    vtkOpenGLRenderWindow::SetGlobalMaximumNumberOfMultiSamples(0);

    QSurfaceFormat::setDefaultFormat(QVTKOpenGLWidget::defaultFormat());

    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    dtkThemesEngine::instance()->apply();

    dtkApplication *application = dtkApplication::create(argc, argv);
    application->setApplicationName("gnomon");
    application->setOrganizationName("inria");
    application->setOrganizationDomain("fr");
    application->installEventFilter(new gnomonEventFilter());

    QCommandLineParser *parser = application->parser();
    parser->setApplicationDescription("gnomon application.");

    application->initialize();

    QCommandLineOption verboseOption("verbose", QCoreApplication::translate("main", "verbose plugin initialization"));

    if (parser->isSet(verboseOption)) {

        dtkImaging::setVerboseLoading(true);

        dtk::widgets::setVerboseLoading(true);

        gnomonCore::setVerboseLoading(true);
        gnomonVisualization::setVerboseLoading(true);
        gnomonWidgets::setVerboseLoading(true);
    }

    dtkImaging::initialize();
    gnomonCore::initialize();
    gnomonVisualization::initialize();
    gnomonWidgets::initialize();

    bool redirect_io = false; int stat;

    dtkScriptInterpreterPython::instance()->init("gnomon-core");

    gnomonMainWindow *widget = new gnomonMainWindow;
    widget->setWindowTitle("gnomon");
    widget->show();
    widget->raise();

    int status = application->exec();

    delete widget;

    dtkWidgetsController::instance()->clear();
    dtkImaging::uninitialize();

    gnomonCore::uninitialize();
    gnomonVisualization::uninitialize();
    gnomonWidgets::uninitialize();

    // dtkScriptInterpreterPython::instance()->release();

    return status;
}

// /////////////////////////////////////////////////////////////////////////////

#include "main.moc"

//
// main.cpp ends here
