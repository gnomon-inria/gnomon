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
    d->project = new gnomonProject("");
}

gnomonProjectManager::~gnomonProjectManager(void)
{
    delete d->project;
    delete d;
}

gnomonProject *gnomonProjectManager::project(void)
{
    return d->project;
}

gnomonProjectManager *gnomonProjectManager::s_instance = nullptr;
std::mutex gnomonProjectManager::s_mutex;

//
// gnomonProjectManager.cpp ends here
