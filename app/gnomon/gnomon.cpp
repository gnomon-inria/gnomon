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

#include <dtkScript>
#include <dtkLog>

#include <gnomonCore>
#include <gnomonPipeline>
#include <gnomonProject>
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

bool gnomonQMLUtils::fileExists(QString file_path)
{
    QFile file(file_path);
    return file.exists();
}

gnomonQMLUtils* gnomonQMLUtils::instance(void)
{
    if(!s_instance)
        s_instance = new gnomonQMLUtils;
    return s_instance;
}

QJsonObject gnomonQMLUtils::projectInfo(const QString &path)
{
    QVariantMap project_info = gnomonProject::readProjectInfoFromPath(path);
    QJsonObject project_info_json;
    for (auto key: project_info.keys()) {
        project_info_json.insert(key, project_info[key].toString());
    }
    return project_info_json;
}

bool gnomonQMLUtils::fileBelongsToProject(const QString &relative_path, const QString &project_path)
{
    gnomonProject *project = new gnomonProject(project_path);
    QString absolute_path = project->findFile(relative_path);
    delete project;
    return !absolute_path.isEmpty();
}

QString gnomonQMLUtils::findProjectFile(const QString &relative_path, const QString &project_path)
{
    gnomonProject *project = new gnomonProject(project_path);
    QString absolute_path = project->findFile(relative_path);
    delete project;
    return absolute_path;
}

QString gnomonQMLUtils::projectRelativePath(const QString &absolute_path, const QString &project_path)
{
    gnomonProject *project = new gnomonProject(project_path);
    QString relative_path = project->relativePath(absolute_path);
    delete project;
    return relative_path;
}

void gnomonQMLUtils::makeProjectThumbnail(const QString &project_path, const QString &thumbnail_path, QPointF top_left, QPointF bottom_right)
{
    QImage image(thumbnail_path);

    int x = int(top_left.x()*image.width());
    int y = int(top_left.y()*image.height());
    int _width = int((bottom_right.x() - top_left.x())*image.width());
    int _height = int((bottom_right.y() - top_left.y())*image.height());
    int width = _width < _height ? _width : _height;
    int height = _width < _height ? _width : _height;

    QImage thumbnail = image.copy(x, y, width, height);
    thumbnail.save(project_path + "/.gnomon/thumbnail.png");
}

void gnomonQMLUtils::removeProjectThumbnail(const QString &project_path)
{
    QFile file(project_path + "/.gnomon/thumbnail.png");
    if (file.exists()) {
        file.remove();
    }
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
    //qputenv("LC_ALL", "C");
    //QLocale::setDefault(QLocale::c());

    dtkLogger::instance().setLevel("info");
    dtkLogger::instance().attachConsole();

    old_handler = std::signal(SIGINT, signal_handler);

    // x_logger
    x_logger_initialise();
    x_logger_append_console();

    // x_vis
    x_vis_initialise();

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

    qmlRegisterType<gnomonProject>("gnomon.Project", 1, 0, "Project");
    qmlRegisterType<gnomonFileSystemModel>("gnomon.Project", 1, 0, "FileSystemModel");
    qmlRegisterSingletonInstance("gnomon.Project", 1, 0, "ProjectManager", gnomonProjectManager::instance());
    qmlRegisterSingletonInstance("gnomon.Project", 1, 0, "SessionManager", gnomonAbstractSessionManager::instance());

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
