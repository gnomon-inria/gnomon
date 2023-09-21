#include "gnomonProject.h"
#include "gnomonProject"


#define PROJECT_INFO_FOLDER ".gnomon"
#define PROJECT_INFO_FILE ".gnomon/project.json"
#define PROJECT_BACKUP_FOLDER ".backup_gnomon"
#define PROJECT_BACKUP_MANIFEST ".backup_gnomon/manifest.json"
#define PROJECT_MANIFEST_FILE ".gnomon/manifest.json"

// /////////////////////////////////////////////////////////////////
// gnomonProjectPrivate
// /////////////////////////////////////////////////////////////////

class gnomonProjectPrivate {

public:
    explicit gnomonProjectPrivate(const QString &path);
    ~gnomonProjectPrivate();

public:
    void initManifest(const QString& url);
    QJsonObject readFromJson(const QString& url);

public:

    gnomonProjectInfo projectInfo;

    QDir projectDir;
    QDir currentDir;

    QString manifest_url;
};

gnomonProjectPrivate::gnomonProjectPrivate(const QString &path): 
    projectDir(path), 
    currentDir(path)
{
    QDir::setCurrent(path);
    projectInfo.path = path;
}

gnomonProjectPrivate::~gnomonProjectPrivate()
{

}

void gnomonProjectPrivate::initManifest(const QString& url)
{
    QFile file(url);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << Q_FUNC_INFO << "can't open file " << url;
        return;
    }
    file.close();
}

QJsonObject gnomonProjectPrivate::readFromJson(const QString& url)
{
    QByteArray storage;
    QFile f(url);
    if(f.open(QIODevice::ReadOnly| QIODevice::Text)) {
        storage = f.readAll();
        f.close();
    }
    return QJsonDocument::fromJson(storage).object();;
}
// /////////////////////////////////////////////////////////////////
// gnomonProject
// /////////////////////////////////////////////////////////////////



gnomonProject::gnomonProject(const QString &path): QObject(nullptr) {
    QString path_copy = sanitizeUrlToPath(path);
    d = new gnomonProjectPrivate(path_copy);

    if(!d->projectDir.exists()) {
        QDir::current().mkpath(path_copy);
    }

    bool isProject = isDirAProject(d->projectDir);
    if(isProject) {
        readProjectInfo();
        wasSaved = QFile::exists(d->projectDir.filePath(PROJECT_MANIFEST_FILE));
    } 
    else {
        auto pName = d->projectDir.dirName();
        populateNewProject();
        d->projectInfo.name = pName;
        d->projectInfo.lastModified = QDateTime::currentDateTime();
    };
}

gnomonProject::~gnomonProject(void)
{
    delete d;
}

QVariantMap gnomonProject::readProjectInfoFromPath(const QString &path) {
    QVariantMap pInfo;
    QString path2 = sanitizeUrlToPath(path);
    QDir projectDir(path2);
    QFile projectInfoFile(projectDir.absoluteFilePath(PROJECT_INFO_FILE));
    if(projectInfoFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&projectInfoFile);
        auto content = in.readAll().toUtf8();
        QJsonDocument doc = QJsonDocument::fromJson(content);
        QJsonObject storage = doc.object();
        pInfo = storage.toVariantMap();

        projectInfoFile.close();
    }
    return pInfo;
}

void gnomonProject::readProjectInfo() {
    auto storage = readProjectInfoFromPath(projectDir());
    auto &pInfo = d->projectInfo;
    pInfo.name = storage["name"].toString();
    pInfo.description = storage["description"].toString();
    pInfo.path = storage["path"].toString();
    pInfo.lastModified = QDateTime::fromString(storage["lastModified"].toString(), Qt::ISODate);
}

void gnomonProject::saveProjectInfo() {
    QFile projectInfoFile(d->projectDir.absoluteFilePath(PROJECT_INFO_FILE));
    if(projectInfoFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        auto &pInfo = d->projectInfo;
        QJsonObject storage;
        storage["name"] = pInfo.name;
        storage["description"] = pInfo.description;
        storage["path"] = pInfo.path;
        pInfo.lastModified.setSecsSinceEpoch(QDateTime::currentSecsSinceEpoch());
        storage["lastModified"] = pInfo.lastModified.toString("yyyy-MM-ddTHH:mm:ss");
        QJsonDocument doc(storage);
        QTextStream out(&projectInfoFile);
        out << doc.toJson();
        out.flush();
        projectInfoFile.close();
    }
}

void gnomonProject::populateNewProject() {
    d->projectDir.mkdir(PROJECT_INFO_FOLDER);
    saveProjectInfo();
    d->projectDir.mkdir(PROJECT_BACKUP_FOLDER);

    d->manifest_url = d->projectDir.filePath(PROJECT_BACKUP_MANIFEST);
    d->initManifest(d->manifest_url);
}

bool gnomonProject::isDirAProject(const QDir &dir) {
    return dir.exists(PROJECT_INFO_FOLDER) && dir.exists(PROJECT_BACKUP_FOLDER);
}

QString gnomonProject::projectDir(void)
{
    return d->currentDir.path();
}

void gnomonProject::setCurrentDir(const QString& url)
{
    auto path = sanitizeUrlToPath(url);
    if(path != d->currentDir.path()) {
        d->currentDir.setPath(path);
        emit currentDirChanged();
    }
}

bool gnomonProject::loadSessionFromPipeline(const QString &path, QObject *window)
{
    GNOMON_SESSION->loadFromPipeline(path, window);
}

gnomonProject *gnomonProject::newProject(const QString &path, const QString &name, const QString &description) {
    auto project = new gnomonProject(path);
    project->d->projectInfo.name = name;
    project->d->projectInfo.description = description;
    project->saveProjectInfo();
    return project;
}

void gnomonProject::close() {
    qWarning()<<Q_FUNC_INFO<<"Not implemented.";
}

QString gnomonProject::currentDir(void) {
    return d->currentDir.path();
}

gnomonAbstractSessionManager* gnomonProject::currentSession(void)
{
    return GNOMON_SESSION;
}

QString gnomonProject::sanitizeUrlToPath(const QString &url) {
    QUrl _url(url);
    return QString(_url.isValid() && _url.isLocalFile() ? _url.toLocalFile() : url);
}

void gnomonProject::addToManifest(const QString &workspace, const QString &data_path, const QString &plugin_name)
{
    QFile file(d->manifest_url);
    if(file.open(QIODevice::Append | QIODevice::Text)) {
        QJsonObject session_json;
        QJsonObject data_json;
        data_json.insert("path", data_path);
        data_json.insert("plugin_name", plugin_name);
        session_json.insert(workspace, data_json);

        QJsonDocument session_doc(session_json);
        file.write(session_doc.toJson());
        file.close();
    }
}

bool gnomonProject::backupFile(const QString &fname, const QString &content)
{
    auto file_path = d->projectDir.filePath(PROJECT_BACKUP_FOLDER + QString("/") + fname);
    QFile f(file_path);
    if(f.open(QIODevice::WriteOnly| QIODevice::Text)) {
        QTextStream out(&f);
        out<<content;
        f.close();
    } else {
        return false;
    }
    return true;    
}

void gnomonProject::save(void)
{
    QFile::copy(d->projectDir.filePath(PROJECT_BACKUP_MANIFEST),
                d->projectDir.filePath(PROJECT_MANIFEST_FILE));
}

QStringList gnomonProject::restoreFiles(const QString& workspace)
{
    QStringList restore_info;
    QJsonObject doc_obj = d->readFromJson(d->projectDir.filePath(PROJECT_MANIFEST_FILE));
    QJsonObject data_info = doc_obj.value(workspace).toObject();

    restore_info.append(data_info.value("path").toString());
    restore_info.append(data_info.value("plugin_name").toString());

    return restore_info;
}
//
// gnomonProject.cpp ends here
