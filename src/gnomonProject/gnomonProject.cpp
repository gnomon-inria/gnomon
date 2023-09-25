#include "gnomonProject.h"
#include "gnomonProject"


#define PROJECT_INFO_FOLDER ".gnomon"
#define PROJECT_INFO_FILE ".gnomon/project.json"
#define PROJECT_BACKUP_FOLDER ".backup_gnomon"

// /////////////////////////////////////////////////////////////////
// gnomonProjectPrivate
// /////////////////////////////////////////////////////////////////

class gnomonProjectPrivate {

public:
    explicit gnomonProjectPrivate(const QString &path);
    ~gnomonProjectPrivate();

public:

    gnomonProjectInfo projectInfo;

    QDir projectDir;
    QDir currentDir;

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
    GNOMON_SESSION->loadFromPipeline(path);
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

//
// gnomonProject.cpp ends here
