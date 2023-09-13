#include <QQmlEngine>
#include "gnomonProjectManager.h"
#include "gnomonProject.h"
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


gnomonProject *gnomonProjectManager::createProject(const QString &path, const QString &name="") {
    closeProject();
    d->project = gnomonProject::newProject(path, name);
    QQmlEngine::setObjectOwnership(d->project, QQmlEngine::CppOwnership);
    return d->project;
}

void gnomonProjectManager::closeProject() {
    if(d->project) {
        d->project->close();
        delete d->project;
        d->project = nullptr;
    }
}

gnomonProjectManager *gnomonProjectManager::s_instance = nullptr;
std::mutex gnomonProjectManager::s_mutex;

//
// gnomonProjectManager.cpp ends here
