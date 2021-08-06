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

#include <QVTKOpenGLNativeWidget.h>

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

//                        gnomonOverlayButton *export_button = new gnomonOverlayButton(fa::arrowcircleup, "", widget);
//                        export_button->move(10,10);
//                        export_button->show();
//
//                        connect(export_button, &gnomonOverlayButton::iconClicked, [=] (void) -> void
//                        {
//                            // qDebug() << Q_FUNC_INFO << 0 << widget;
//
//                            foreach(QWidget *top, qApp->topLevelWidgets()) {
//
//                                // qDebug() << Q_FUNC_INFO << 1 << widget;
//
//                                foreach(gnomonWorkspaceLSystemSimulator *simulator, top->findChildren<gnomonWorkspaceLSystemSimulator *>()) {
//
//                                    // qDebug() << Q_FUNC_INFO << 2 << widget;
//
//                                    simulator->apply(widget);
//                                }
//                            }
//                        });


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
    dtk::widgets::initialize();

    vtkOpenGLRenderWindow::SetGlobalMaximumNumberOfMultiSamples(0);

    QSurfaceFormat::setDefaultFormat(QVTKOpenGLNativeWidget::defaultFormat());

    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    dtkThemesEngine::instance()->apply();

    dtkApplication *application = dtkApplication::create(argc, argv);
    application->setApplicationName("gnomon");
    application->setOrganizationName("inria");
    application->setOrganizationDomain("fr");
    application->installEventFilter(new gnomonEventFilter());

    QPixmap pixmap(":gnomon/gnomon_logo.png");
    qDebug()<<pixmap;
    QSplashScreen *splash = new QSplashScreen(pixmap);
    splash->show();

    splash->showMessage("Preparing parsers",Qt::AlignRight|Qt::AlignBottom);
    application->processEvents();

    QCommandLineParser *parser = application->parser();
    parser->setApplicationDescription("gnomon application.");

    QCommandLineOption workspaceOption("workspace", "Workspace to open on application launch", "workspace name", "Form Browser");
    parser->addOption(workspaceOption);
    application->initialize();


    splash->showMessage("Loading core functionalities",Qt::AlignRight|Qt::AlignBottom);
    application->processEvents();

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

    splash->showMessage("Initializing Python interpreter",Qt::AlignRight|Qt::AlignBottom);
    application->processEvents();

    bool redirect_io = false; int stat;

    dtkScriptInterpreterPython::instance()->init("gnomon-core");

    dtkScriptInterpreterPython::instance()->interpret("import matplotlib",&stat);
    dtkScriptInterpreterPython::instance()->interpret("matplotlib.use('qt5agg')",&stat);

    splash->showMessage("Loading reader plugins",Qt::AlignRight|Qt::AlignBottom);
    application->processEvents();

    QString workspace_name = parser->value(workspaceOption);
    gnomonMainWindow *widget = new gnomonMainWindow(workspace_name);

    splash->showMessage("Assembling application window",Qt::AlignRight|Qt::AlignBottom);
    application->processEvents();

    widget->setWindowTitle("gnomon");
    widget->show();
    widget->raise();
    splash->finish(widget);

    int status = application->exec();

    delete widget;

    dtkScriptInterpreterPython::instance()->release();
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
