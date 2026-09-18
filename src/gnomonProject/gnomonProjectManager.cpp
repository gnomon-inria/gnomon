#include <QQmlEngine>
#include "gnomonProjectManager.h"
#include "gnomonProject.h"
#include "gnomonAbstractSessionManager.h"
// /////////////////////////////////////////////////////////////////
// gnomonProjectManagerPrivate
// /////////////////////////////////////////////////////////////////
class gnomonProjectManagerPrivate
{
public:
    gnomonProject *project = nullptr;
};

// /////////////////////////////////////////////////////////////////
// gnomonProjectManager
// /////////////////////////////////////////////////////////////////

gnomonProjectManager *gnomonProjectManager::instance() {
    std::lock_guard<std::mutex> lock(s_mutex);
    if(!s_instance)
        s_instance = new gnomonProjectManager;
    return s_instance;
}

gnomonProjectManager::gnomonProjectManager(QObject *parent) : QObject(parent)
{
    d = new gnomonProjectManagerPrivate;
}

gnomonProjectManager::~gnomonProjectManager(void)
{
    d->project->close();
    delete d->project;
    delete d;
}

gnomonProject *gnomonProjectManager::project(void)
{
    if(!d->project)
        qWarning()<<"no project created yet.";
    return d->project;
}

gnomonProject *gnomonProjectManager::openProject(const QString &path, bool restore_session, bool load_pipeline) {
    closeProject();
    d->project = new gnomonProject(path);
    emit projectCreated();
    if(!load_pipeline){
        if(!restore_session || !GNOMON_SESSION->load()) {
            GNOMON_SESSION->newSession(d->project->projectInfo().default_source);
        }
    }
    QQmlEngine::setObjectOwnership(d->project, QQmlEngine::CppOwnership);
    emit projectLoaded();
    return d->project;
}

gnomonProject *gnomonProjectManager::createProject(const QString &path, const QString &name,
                                                   const QString &description, const QString &source) {
    closeProject();
    d->project = gnomonProject::newProject(path, name, description, source);
    emit projectCreated();
    // if there is a session definition load otherwise newSession
    if(!GNOMON_SESSION->load()) {
        GNOMON_SESSION->newSession(source);
    };
    QQmlEngine::setObjectOwnership(d->project, QQmlEngine::CppOwnership);
    return d->project;
}

bool gnomonProjectManager::isExistingProject(const QString &path) const {
    return gnomonProject::isDirAProject(QDir(QUrl(path).toLocalFile()));
}

void gnomonProjectManager::closeProject() {
    if(d->project) {
        d->project->close();
        delete d->project;
        d->project = nullptr;
    }
}

bool gnomonProjectManager::cleanProject(const QString &path) const {
    auto info_dir_path = QUrl(path).toLocalFile() + "/.gnomon";
    QDir info_dir(info_dir_path);
    return info_dir.removeRecursively();
}

gnomonProjectManager *gnomonProjectManager::s_instance = nullptr;

QVariantMap gnomonProjectManager::readProjectInfo(const QString &path) {
    return gnomonProject::readProjectInfoFromPath(path);
}

std::mutex gnomonProjectManager::s_mutex;

//
// gnomonProjectManager.cpp ends here
