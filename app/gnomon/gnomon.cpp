#include "gnomon.hpp"
#include "MaterialPreview.hpp"
#include "gnomonSessionManager.h"

#include <memory>

#include <QtCore>
#include <QtConcurrent>
#include <QtDebug>
#include <QtQml>
#include <QtQuick>
#include <QtWebEngineQuick>

#include <dtkImagingCore>
#include <dtkScript>
#include <dtkLog>

#include <gnomonCore>
#include <gnomonPipeline>
#include <gnomonVisualization>
#include <gnomonWorkspace>

#include <qobjectdefs.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <csignal>

// /////////////////////////////////////////////////////////////////////////////
// NOTE:
// /////////////////////////////////////////////////////////////////////////////

#include <xLogger.hpp>
#include <xVis.hpp>
#include <xVisViewer.hpp>

gnomonImageProvider::gnomonImageProvider() : QQuickImageProvider(QQuickImageProvider::Image)
{

}

QImage gnomonImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize) {
    QImage image = gnomonFormManager::instance()->thumbnail(id.toInt());
    return image;
}

// /////////////////////////////////////////////////////////////////////////////
// Workspace image provider
// /////////////////////////////////////////////////////////////////////////////

workspaceImageProvider::workspaceImageProvider(QObject* parent) : QQuickImageProvider(QQuickImageProvider::Image)
{
    this->setParent(parent);
}

QImage workspaceImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize) {
    QImage image = this->m_thumbnails[id.toInt()];
    return image;
}

void workspaceImageProvider::makeScreenshot(const QString &id) {
    QQuickWindow *root = static_cast<QQuickWindow *>(sender());
    QImage grabResult = root->grabWindow();

    this->m_thumbnails.insert(id.toInt(), grabResult);

    QVariant returnedValue;
    QMetaObject::invokeMethod(root, "update_thumbnail",
            Q_RETURN_ARG(QVariant, returnedValue),
            Q_ARG(QVariant, id.toInt()));
}

// /////////////////////////////////////////////////////////////////////////////
// GnomonViewerAssociator
// /////////////////////////////////////////////////////////////////////////////

class gnomonViewAssociator : public QObject
{
    Q_OBJECT

public:
    static gnomonViewAssociator *instance(void);

public:
    Q_INVOKABLE void associate(QObject *, gnomonVtkView *);

private:
    static gnomonViewAssociator *s_instance;
};

gnomonViewAssociator *gnomonViewAssociator::instance(void)
{
    if(!s_instance)
        s_instance = new gnomonViewAssociator;

    return s_instance;
}

void gnomonViewAssociator::associate(QObject *source, gnomonVtkView *destination)
{
    xVisViewer *viewer = source->findChild<xVisViewer*>("", Qt::FindDirectChildrenOnly);
    viewer->setExternalEnabled(true);

    connect(source, SIGNAL(transmit()),          destination, SLOT(transmit()));
    connect(source, SIGNAL(switchTo2D()),        destination, SLOT(switchTo2D()));
    connect(source, SIGNAL(switchTo2DXY()),      destination, SLOT(switchTo2DXY()));
    connect(source, SIGNAL(switchTo2DXZ()),      destination, SLOT(switchTo2DXZ()));
    connect(source, SIGNAL(switchTo2DYZ()),      destination, SLOT(switchTo2DYZ()));
    connect(source, SIGNAL(switchTo3D()),        destination, SLOT(switchTo3D()));
    connect(source, SIGNAL(sliceChange(double)), destination, SLOT(sliceChange(double)));

    destination->associate(viewer->GetRenderWindow());
 }

gnomonViewAssociator *gnomonViewAssociator::s_instance = 0;

class gnomonMetaDataFetcher : public QObject
{
    Q_OBJECT

public:
    Q_INVOKABLE QString workspaceMetaData(const QString& type, const QString& key);
    Q_INVOKABLE QStringList pluginGroupMetaData(const QString& key);
    Q_INVOKABLE QStringList pluginGroupMetaData(const QStringList& keys);
    static gnomonMetaDataFetcher *instance(void);

private:
    static gnomonMetaDataFetcher *s_instance;
};

QString gnomonMetaDataFetcher::workspaceMetaData(const QString& type, const QString& key)
{
    auto metaT = QMetaType::fromName((type+" *").toLocal8Bit());
    if(metaT.isValid()) {
        const QMetaObject *object = metaT.metaObject();
        int info_id = object->indexOfClassInfo(key.toLocal8Bit());
        if (info_id > -1) {
            QMetaClassInfo info = object->classInfo(info_id);
            return QString(info.value());
        } else {
            dtkWarn()<<Q_FUNC_INFO<<"No"<<key<<"found in metatype info for type"<<type;
            return "";
        }
    }

    dtkWarn()<<Q_FUNC_INFO<<"No valid metatype found for type"<<type;
    return "";
}

QStringList gnomonMetaDataFetcher::pluginGroupMetaData(const QString& key)
{
    if(key.isEmpty())
        return {""};
    else
        return availablePluginsFromGroup(key);
}

QStringList gnomonMetaDataFetcher::pluginGroupMetaData(const QStringList& keys)
{
    QStringList plugins;
    for (const auto& key: keys) {
        plugins += this->pluginGroupMetaData(key);
    }
    return plugins;
}

gnomonMetaDataFetcher* gnomonMetaDataFetcher::instance(void)
{
    if(!s_instance)
        s_instance = new gnomonMetaDataFetcher;
    return s_instance;

}
gnomonMetaDataFetcher *gnomonMetaDataFetcher::s_instance = 0;

// /////////////////////////////////////////////////////////////////////////////

bool gnomonSessionLoader::runNodes(QStringList scheduled_nodes, QObject *window, std::shared_ptr<gnomonPipeline> pipeline, double progress_increment = -1.) {
    res = true;

    if(progress_increment == -1.) {
        progress_increment = 1. / scheduled_nodes.size();
    }

    if(scheduled_nodes.isEmpty()) {
        emit finished();
        return true;
    }

    QString n_name = scheduled_nodes.takeFirst();
    gnomonPipelineNode *node = pipeline->node(n_name);

    if(node->algorithmClass().contains("Reader", Qt::CaseInsensitive)) {
        qInfo() << "Reader node : " << node->algorithmClass();
        gnomonWorkspaceBrowser *browser;
        QMetaObject::invokeMethod(window, "workspaceBrowser",
            Q_RETURN_ARG(gnomonWorkspaceBrowser *, browser));

        if(!browser) {
            dtkWarn() << Q_FUNC_INFO << " cannot create worksace browser";
            emit failed(file_path);
            return false;
        }
        browser->view()->clear();

        auto tmp = QDir::currentPath();
        QDir::setCurrent(gnomonQMLUtils::instance()->dataPath());
        QString read_path = node->path();
        QStringList paths = read_path.split(",");
        for (size_t i = 0; i < paths.size(); i++) {
            QDir path(paths[i]);
            if(path.isRelative()) {
                paths[i] = path.absolutePath();
            }
        }
        QDir::setCurrent(tmp);

        callbackConnection = QObject::connect(browser, &gnomonAbstractWorkspace::finished, [=]() {
            disconnect(callbackConnection);
            if(browser->view()->empty()) {
                emit failed(file_path);
                return false;
            } else {
                browser->view()->transmit();
            }
            setProgress(progress() + progress_increment);
            res = runNodes(scheduled_nodes, window, pipeline, progress_increment);
        });

        browser->setReaderPath(paths.join(","));
        browser->readWith(node->algorithmPlugin());

    } else if(node->algorithmClass().contains("morphonetCellImage", Qt::CaseInsensitive)) {
        qInfo() << "Moprphonet node : " << node->algorithmClass();
        gnomonWorkspaceMorphonet * morphonet = nullptr;
        QVariant morphonet_data_var = node->morphonet_data().toVariantMap();
        bool ok = QMetaObject::invokeMethod(window, "workspaceMorphonet",
                Q_RETURN_ARG(gnomonWorkspaceMorphonet *, morphonet),
                Q_ARG(QVariant, morphonet_data_var));

        if(!morphonet || !ok) {
            emit failed(file_path);
            return false;
        }
        callbackConnection = QObject::connect(morphonet, &gnomonAbstractWorkspace::finished, [=]() {
            disconnect(callbackConnection);
            if(morphonet->view()->empty()) {
                emit failed(file_path);
                res = false;
                return;
            } else {
                morphonet->view()->transmit();
            }
            setProgress(progress() + progress_increment);
            res = runNodes(scheduled_nodes, window, pipeline, progress_increment);
        });

    } else if(node->algorithmClass().contains("Writer", Qt::CaseInsensitive)) {
        dtkInfo() << "Writer node (not creating anything) : " << node->algorithmClass();
        setProgress(progress() + progress_increment);
        res = runNodes(scheduled_nodes, window, pipeline, progress_increment);
    } else if(node->algorithmClass() == "task") {
        qInfo() << "Task node : " << node->algorithmPlugin();
        // auto node = dynamic_cast<gnomonPipelineNodeTask>(node);

        if(node->algorithmPlugin() == "compose") {
            auto inputPortsNames = node->inputPortsNames();
            int first = node->inputPort(inputPortsNames.first())->formIndex();
            if(node->inputPort(inputPortsNames[1])->formIndex() >=0) {
                int second = node->inputPort(inputPortsNames[1])->formIndex();
                qDebug() << "COMPOSE " << first << " -> " << second;
                gnomonFormManager::instance()->compose(first, second);
            }
        }

        setProgress(progress() + progress_increment);
        res = runNodes(scheduled_nodes, window, pipeline, progress_increment);
    } else if(node->algorithmClass().contains("formAlgorithm", Qt::CaseInsensitive)) {
        dtkInfo() << "Node : Python Workspace";
        gnomonWorkspacePythonAlgorithm * w_p = nullptr;
        QVariant p_var = node->parameters().toVariantMap();
        bool ok = QMetaObject::invokeMethod(window, "workspacePython",
                Q_RETURN_ARG(gnomonWorkspacePythonAlgorithm *, w_p),
                Q_ARG(QVariant, p_var));

        if(!w_p || !ok) {
            emit failed(file_path);
            return false;
        }

        QList<gnomonVtkView *> views = w_p->sources()->views();
        for(auto *view: views)
            view->clear();

        //TOCHECK how to put into different views? now always view[0]
        for(QString in : node->inputPortsNames()) {
            if(node->inputPort(in)->formIndex() >=0) {
                views[0]->drop(node->inputPort(in)->formIndex());
            }
        }

        callbackConnection = QObject::connect(w_p, &gnomonAbstractWorkspace::finished, [=]() {
            disconnect(callbackConnection);
            //TOCHECK only transmit from one target!
            if((*w_p->targets())[0]->empty()) {
                res = false;
                emit failed(file_path);
                return;
            } else {
                (*w_p->targets())[0]->transmit();
            }
            setProgress(progress() + progress_increment);
            res = runNodes(scheduled_nodes, window, pipeline, progress_increment);
        });

        w_p->run();
    }  else if(node->algorithmClass().contains("lStringEvolutionModel", Qt::CaseInsensitive)) {
        gnomonWorkspaceLSystemModel * w_p = nullptr;
        QVariant p_var = node->parameters().toVariantMap();
        bool ok = QMetaObject::invokeMethod(window, "workspaceLSystem",
                                            Q_RETURN_ARG(gnomonWorkspaceLSystemModel *, w_p),
                                            Q_ARG(QVariant, p_var));
        if(!w_p || !ok) {
            emit failed(file_path);
            return false;
        }

        callbackConnection = connect(w_p, &gnomonAbstractWorkspace::finished, [=]() {
            disconnect(callbackConnection);
            if(w_p->view()->empty()) {
                res = false;
                emit failed(file_path);
                return;
            } else {
                w_p->view()->transmit();
            }
            setProgress(progress() + progress_increment);
            res = runNodes(scheduled_nodes, window, pipeline, progress_increment);
        });

        w_p->run();
    } else {
        qInfo() << "Node : " << node->algorithmClass();
        gnomonAlgorithmWorkspace * w_d = nullptr;
        QVariant p_var = node->parameters().toVariantMap();
        bool ok = QMetaObject::invokeMethod(window, "loadWorkspace",
                Q_RETURN_ARG(gnomonAlgorithmWorkspace *, w_d),
                Q_ARG(QString, node->algorithmPlugin()),
                Q_ARG(QVariant, p_var));

        if(!w_d || !ok) {
            emit failed(file_path);
            return false;
        }

        QList<gnomonVtkView *> views = w_d->sources()->views();
        for(auto *view: views)
            view->clear();

        //if nodePortNames is not empty use it to check if it's the good view,
        //if it's not set then try to use the view!
        for(QString in : node->inputPortsNames()) {
            if(node->inputPort(in)->formIndex() >=0) {
                for(auto *view: views){
                    if(view->nodePortNames().isEmpty() || view->nodePortNames().contains(in)) {
                        view->drop(node->inputPort(in)->formIndex());
                    }
                }
            }
        }

        if (node->algorithmClass().contains("imageRegistration", Qt::CaseInsensitive)) {
            if(node->inputPortsNames().contains("initialTransformation") && node->inputPort("initialTransformation")->formIndex() >=0) {
                w_d->textView()->drop(node->inputPort("initialTransformation")->formIndex());
            }
        }

        callbackConnection = QObject::connect(w_d, &gnomonAbstractWorkspace::finished, [=]() {
            disconnect(callbackConnection);
            QStringList form_types;
            gnomonVtkViewList *targets = w_d->targets();
            bool has_target = targets && !targets->views().empty();
            if (has_target) {
                form_types.append(targets->views()[0]->acceptedForms());
            }
            gnomonMplView *figure = w_d->figure();
            bool has_figure = figure;
            if (has_figure) {
                form_types.append(figure->acceptedForms());
            }
            gnomonQmlView *text_view = w_d->textView();
            bool has_text_view = text_view;
            if (has_text_view) {
                form_types.append(text_view->acceptedForms());
            }

            form_types.removeDuplicates();
            form_types.sort();

            bool transmit = false;
            for (auto form_type : form_types) {
                //qDebug()<<Q_FUNC_INFO<<form_type;
                if (has_target) {
                    if (targets->views()[0]->contains(form_type)) {
                        targets->views()[0]->transmitForm(form_type);
                        transmit = true;
                    }
                }
                if (has_figure) {
                    if (figure->contains(form_type)) {
                        figure->transmitForm(form_type);
                        transmit = true;
                    }
                }
                if (has_text_view) {
                    if (text_view->contains(form_type)) {
                        text_view->transmitForm(form_type);
                        transmit = true;
                    }
                }
            }

            // Do not delete the next line as it seems, somehow, to prevent a crash caused by the GIL when calling nucleiDetectionTimagetk
            qInfo()<<"";

            if (!transmit) {
                res = false;
                emit failed(file_path);
                return;
            }
            setProgress(progress() + progress_increment);
            res = runNodes(scheduled_nodes, window, pipeline, progress_increment);
        });

        w_d->run();
    }
    return res;
}


bool gnomonSessionLoader::load(const QString& url, QObject *window)
{
    auto pipeline = std::make_shared<gnomonPipeline>();
    file_path = url;
    pipeline->readFromJson(url, true);
    gnomonPipelineManager::instance()->pipeline()->setName(pipeline->name());
    gnomonPipelineManager::instance()->pipeline()->setDescription(pipeline->description());
    auto scheduled_nodes = pipeline->scheduledNodeNames(true);
    return runNodes(scheduled_nodes, window, pipeline);
}

double gnomonSessionLoader::progress()
{
    return this->m_progress;
}

void gnomonSessionLoader::setProgress(double progress)
{
    this->m_progress = progress;
    qInfo() << "Session Loading at "<< int(100*this->m_progress) << "%";
    emit progressChanged();
}

// /////////////////////////////////////////////////////////////////////////////
// gnomonQMLUtils
// /////////////////////////////////////////////////////////////////////////////

QString gnomonQMLUtils::dataPath()
{
    if(!qEnvironmentVariableIsSet("GNOMON_DATA") || qEnvironmentVariableIsEmpty("GNOMON_DATA")) {
        //qputenv("GNOMON_DATA", QDir::currentPath().toLocal8Bit());
        return ".";
    }
    return qEnvironmentVariable("GNOMON_DATA", QDir::currentPath());
}

void gnomonQMLUtils::setDataPath(QString new_path)
{
    auto previous = qgetenv("GNOMON_DATA");
    QByteArray encoded = new_path.toLocal8Bit();
    if(!encoded.isNull()) {
        bool res = qputenv("GNOMON_DATA", encoded);
        if(!res) {
            qWarning() << Q_FUNC_INFO << "Could not change GNOMON_DATA environment variable.";
            qputenv("GNOMON_DATA", previous);
        }
    }
}

bool gnomonQMLUtils::isValidPath(QString path)
{
    QDir dir(path);
    return dir.exists() && dir.isAbsolute();
}

QString gnomonQMLUtils::initDataPath(QString defaultPath) {
    // Set $GNOMON_DATA env
    // Priority: 1. $GNOMON_DATA already set, 2. defaultPath, 3. current directory
    QString path;
    if(qEnvironmentVariableIsSet("GNOMON_DATA") && !qEnvironmentVariableIsEmpty("GNOMON_DATA")) {
        path = qEnvironmentVariable("GNOMON_DATA", defaultPath);
        if(path != defaultPath) {
            return path;
        }
    }
    if(isValidPath(defaultPath)) {
        setDataPath(defaultPath);
        path = qEnvironmentVariable("GNOMON_DATA", "");
        if(path == defaultPath) {
            return defaultPath;
        }
    }
    setDataPath(QDir::currentPath());
    return dataPath();

}

gnomonQMLUtils* gnomonQMLUtils::instance(void)
{
    if(!s_instance)
        s_instance = new gnomonQMLUtils;
    return s_instance;
}

gnomonQMLUtils *gnomonQMLUtils::s_instance = nullptr;

// /////////////////////////////////////////////////////////////////////////////
// Handler to prevent python from stealing the default signal handler
// Note: try to use  PyConfig with python >=3.8.
// /////////////////////////////////////////////////////////////////////////////
void (*old_handler)(int);
void signal_handler(int signal)
{
    std::signal(SIGINT, old_handler);
    raise(signal);
}

void gnomonInitLogServer()
{
    gnomonLogCaptureServer::instance();
}

void gnomon_rinit()
{
    qputenv("QTWEBENGINE_DISABLE_GPU_THREAD", "1");
    qputenv("QTWEBENGINE_CHROMIUM_FLAGS", "--disable-gpu --disable-gpu-compositing --num-raster-threads=1 --enable-viewport --main-frame-resizes-are-orientation-changes --disable-composited-antialiasing");

    QtWebEngineQuick::initialize();

    //qputenv("QT_PLUGIN_PATH", "1");
    qputenv("LC_ALL", "C");
    QLocale::setDefault(QLocale::c());

    dtkLogger::instance().setLevel("info");
    dtkLogger::instance().attachConsole();

    old_handler = std::signal(SIGINT, signal_handler);

    // x_logger
    x_logger_initialise();
    x_logger_append_console();

    // x_vis
    x_vis_initialise();

    // --
    dtkImaging::setVerboseLoading(false);
    dtkImaging::initialize();

    //--
    dtkScriptInterpreterPython::instance()->init("gnomon-core");

    // --
    gnomonCore::setVerboseLoading(false);
    gnomonCore::activateObjectManager();
    gnomonCore::initialize();
    // --
    gnomonSessionManager::initialize();

    gnomonVisualization::initialize();
    // --
    int stat;
    // Initialize the matplotlib config and register FigureCanvas QML type
    dtkScriptInterpreterPython::instance()->interpret("import gnomon.utils.matplotlib_tools", &stat);
    dtkScriptInterpreterPython::instance()->interpret("import gnomon.utils.jupyter_tools", &stat);

    qmlRegisterType<gnomonPipeline>("gnomon.Pipeline", 1, 0, "Pipeline");
    qmlRegisterUncreatableType<gnomonPipelineEdge>("gnomon.Pipeline", 1, 0, "PipelineEdge", "Can only be created by the pipeline manager");
    qmlRegisterUncreatableType<gnomonPipelineNode>("gnomon.Pipeline", 1, 0, "PipelineNode", "Can only be created by the pipeline manager");
    qmlRegisterUncreatableType<gnomonPipelinePort>("gnomon.Pipeline", 1, 0, "PipelinePort", "Can only be created by the pipeline manager");
    qmlRegisterSingletonInstance("gnomon.Pipeline", 1, 0, "PipelineManager", gnomonPipelineManager::instance());

    // -- Project Management
    qmlRegisterType<gnomonSessionLoader>("gnomon.Pipeline", 1, 0, "SessionLoader");

    // -- Visualization logic registration
    qmlRegisterType<gnomonVtkView>("gnomon.Visualization", 1, 0, "View");
    qmlRegisterType<gnomonVtkViewList>("gnomon.Visualization", 1, 0, "ViewList");
    qmlRegisterType<gnomonMplView>("gnomon.Visualization", 1, 0, "Figure");
    qmlRegisterSingletonInstance("gnomon.Visualization", 1, 0, "World", gnomonFormManager::instance());
    qmlRegisterType<MaterialPreview>("gnomon.Visualization", 1, 0, "MaterialPreview");

    // -- Algorithms logs
    qmlRegisterSingletonType<gnomonLogCaptureServer>("gnomon.Visualization", 1, 0, "LogServer", [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
        Q_UNUSED(scriptEngine)
        gnomonLogCaptureServer * s_instance = gnomonLogCaptureServer::instance();
        Q_ASSERT(engine->thread() == s_instance->thread()); //has to be in the same thread
         QJSEngine::setObjectOwnership(s_instance, QJSEngine::CppOwnership);
        return s_instance;
    });
    qmlRegisterUncreatableType<gnomonLogConnection>("gnomon.Visualization", 1, 0, "LogConnection", "Can only be created by the LogServer");

    // -- Workspaces logic registration
    qmlRegisterType<gnomonAlgorithmWorkspace>("gnomon.Workspaces", 1, 0, "AlgorithmWorkspace");
    qmlRegisterType<gnomonPythonAlgorithmPluginCode>("gnomon.Workspaces", 1, 0, "PythonAlgorithmPluginCode");
    qmlRegisterType<gnomonWorkspaceBrowser>("gnomon.Workspaces", 1, 0, "WorkspaceBrowser");
    qmlRegisterType<gnomonWorkspaceBinarization>("gnomon.Workspaces", 1, 0, "WorkspaceBinarization");
    qmlRegisterType<gnomonWorkspaceCellImageQuantification>("gnomon.Workspaces", 1, 0, "WorkspaceCellImageQuantification");
    qmlRegisterType<gnomonWorkspaceCellImageTracking>("gnomon.Workspaces", 1, 0, "WorkspaceCellImageTracking");
    qmlRegisterType<gnomonWorkspaceImageMeshing>("gnomon.Workspaces", 1, 0, "WorkspaceImageMeshing");
    qmlRegisterType<gnomonWorkspaceLSystemModel>("gnomon.Workspaces", 1, 0, "WorkspaceLSystemModel");
    qmlRegisterType<gnomonWorkspaceMeshFilter>("gnomon.Workspaces", 1, 0, "WorkspaceMeshFilter");
    qmlRegisterType<gnomonWorkspaceMorphonet>("gnomon.Workspaces", 1, 0, "WorkspaceMorphonet");
    qmlRegisterType<gnomonWorkspacePointCloudQuantification>("gnomon.Workspaces", 1, 0, "WorkspacePointCloudQuantification");
    qmlRegisterType<gnomonWorkspacePointDetection>("gnomon.Workspaces", 1, 0, "WorkspacePointDetection");
    qmlRegisterType<gnomonWorkspacePreprocess>("gnomon.Workspaces", 1, 0, "WorkspacePreprocess");
    qmlRegisterType<gnomonWorkspacePythonAlgorithm>("gnomon.Workspaces", 1, 0, "WorkspacePythonAlgorithm");
    qmlRegisterType<gnomonWorkspaceRegistration>("gnomon.Workspaces", 1, 0, "WorkspaceRegistration");
    qmlRegisterType<gnomonWorkspaceSegmentation>("gnomon.Workspaces", 1, 0, "WorkspaceSegmentation");
    qmlRegisterType<gnomonWorkspaceSimulation>("gnomon.Workspaces", 1, 0, "WorkspaceSimulation");


    // -- Link helper
    qmlRegisterSingletonInstance("gnomon.Utils", 1, 0, "Associator", gnomonViewAssociator::instance());

    // -- MetaData helper
    qmlRegisterSingletonInstance("gnomon.MetaData", 1, 0, "MetaData", gnomonMetaDataFetcher::instance());
    qmlRegisterUncreatableType<gnomonDynamicFormMetadata>("gnomon.MetaData", 1, 0, "DynamicFormMetadata", "from gnomonDynamicForm");

    // -- Utils
    qmlRegisterSingletonInstance("gnomon.Utils", 1, 0, "GUtils", gnomonQMLUtils::instance());

    QLocale::setDefault(QLocale::English);
}

// /////////////////////////////////////////////////////////////////////////////

#include "gnomon.moc"
