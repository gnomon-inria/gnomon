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

        static bool first = true;

        qDebug() << Q_FUNC_INFO << object->objectName();

        if(QMainWindow *window = dynamic_cast<QMainWindow *>(object)) {

            if (first) {
                first = false;
                embedded << window;
                return false;
            }

            if(!embedded.contains(window)) {

                window->statusBar()->setSizeGripEnabled(false);

                qDebug() << Q_FUNC_INFO << "OHHHH YEAH ---------" << 0 << window->objectName();

                if(window->objectName() == "PGLMainWindow") {

                    qDebug() << Q_FUNC_INFO << "OHHHH YEAH ---------" << 1;

                    foreach(QWidget *widget, window->findChildren<QWidget*>()) {

                        if(widget->objectName() == "PGLFrameGL") {

                            qDebug() << Q_FUNC_INFO << "OHHHH YEAH ---------" << 2;

                            gnomonOverlayButton *export_button = new gnomonOverlayButton(fa::arrowcircleup, "", widget);
                            export_button->move(10,10);
                            export_button->show();

                            connect(export_button, &gnomonOverlayButton::iconClicked, [=] (void) -> void
                            {
                                // gnomonFormManager::instance()->addForm(QString("prout"), QColor(Qt::green), widget);

                                qDebug() << "Exporting";
                            });
                        }
                    }
                }

                foreach(QWidget *top, qApp->topLevelWidgets()) {
                    foreach(gnomonWorkspaceLSystemSimulator *simulator, top->findChildren<gnomonWorkspaceLSystemSimulator *>()) {
                            simulator->fill(window);
                    }
                }

                embedded << window;
            }
        }
       
        return false;
    }

private:
    QList<QMainWindow *> embedded;
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

// /////////////////////////////////////////////////////////////////////////////

#include "main.moc"

//
// main.cpp ends here
