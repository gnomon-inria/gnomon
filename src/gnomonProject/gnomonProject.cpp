#include "gnomonProject.h"
#include "gnomonProject"


#define PROJECT_INFO_FOLDER ".gnomon"
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
    QString path_copy(path);
    path_copy.remove("file://");
    d = new gnomonProjectPrivate(path_copy);

    if(!d->projectDir.exists()) {
        QDir::current().mkpath(path);
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

void gnomonProject::readProjectInfo() 
{
    qWarning()<<Q_FUNC_INFO<<"Not implemented.";
}
void gnomonProject::populateNewProject() {
    d->projectDir.mkdir(PROJECT_INFO_FOLDER);

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
    auto path = QString(url);
    path.remove("file://");
    if(path != d->currentDir.path()) {
        d->currentDir.setPath(path);
        emit currentDirChanged();
    }
}

void gnomonProject::loadSessionFromPipeline(const QString &path) {
    GNOMON_SESSION->loadFromPipeline(path);
}

gnomonProject *gnomonProject::newProject(const QString &path, const QString &name) {
    QDir current_dir = QDir::current();
    if(!current_dir.exists(path)) {
        current_dir.mkpath(path);
    }
    auto project = new gnomonProject(path);
    project->d->projectInfo.name = name;
    return project;
}

void gnomonProject::close() {

}

QString gnomonProject::currentDir(void) {
    return d->currentDir.path();
}
//
// gnomonProject.cpp ends here
