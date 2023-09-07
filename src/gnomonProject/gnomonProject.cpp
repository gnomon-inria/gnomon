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
    d = new gnomonProjectPrivate(path);

    if(!d->projectDir.exists()) {
        QDir::current().mkpath(path);
    }

    bool isProject = isDirAProject(d->projectDir);
    if(isProject) {
        readProjectInfo();
    } 
    connect(this, &gnomonProject::projectDirChanged, [=](){
        auto pName = d->projectDir.dirName();
        populateNewProject();
        d->projectInfo.name = pName;
        d->projectInfo.lastModified = QDateTime::currentDateTime();
    });
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
    return d->projectDir.path();
}

void gnomonProject::setProjectDir(const QString& url)
{
    auto path = QString(url);
    path.remove("file://");
    if(path != d->currentDir.path()) {
        d->projectDir.setPath(path);
        emit projectDirChanged();
    }
}

bool gnomonProject::loadSessionFromPipeline(const QString &path, QObject *window)
{
    GNOMON_SESSION->loadFromPipeline(path, window);
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

gnomonAbstractSessionManager* gnomonProject::currentSession(void)
{
    return GNOMON_SESSION;
}
//
// gnomonProject.cpp ends here
