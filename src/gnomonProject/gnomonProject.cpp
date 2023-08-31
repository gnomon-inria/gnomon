#include "gnomonProject.h"


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

    QDir projectDir;
    QDir currentDir;

};

gnomonProjectPrivate::gnomonProjectPrivate(const QString &path): projectDir(path), currentDir(path) {
    QDir::setCurrent(path);
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
    } else {
        populateNewProject();
    }
}

gnomonProject::~gnomonProject(void)
{
    delete d;
}

void gnomonProject::populateNewProject() {
    d->projectDir.mkdir(PROJECT_INFO_FOLDER);

    d->projectDir.mkdir(PROJECT_BACKUP_FOLDER);
}

bool gnomonProject::isDirAProject(const QDir &dir) {
    return dir.exists(PROJECT_INFO_FOLDER) && dir.exists(PROJECT_BACKUP_FOLDER);
}

//
// gnomonProject.cpp ends here
