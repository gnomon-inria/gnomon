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
    Q_ASSERT_X(d->project, "project", "no project created yet");
    return d->project;
}

gnomonProjectManager *gnomonProjectManager::s_instance = nullptr;

gnomonProject *gnomonProjectManager::openProject(const QString &path) {
    if(d->project) {
        d->project->close();
        delete d->project;
    }
    d->project = new gnomonProject(path);
    return d->project;
}

gnomonProject *gnomonProjectManager::newProject(const QString &path, const QString &name) {
    if(d->project) {
        d->project->close();
        delete d->project;
    }
    d->project = gnomonProject::newProject(path, name);
    return d->project;
}

std::mutex gnomonProjectManager::s_mutex;

//
// gnomonProjectManager.cpp ends here
