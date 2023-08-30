#include "gnomonProjectManager.h"

// /////////////////////////////////////////////////////////////////
// gnomonProjectManagerPrivate
// /////////////////////////////////////////////////////////////////
class gnomonProjectManagerPrivate
{
public:
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
    delete d;
}

gnomonProjectManager *gnomonProjectManager::s_instance = nullptr;
std::mutex gnomonProjectManager::s_mutex;

//
// gnomonProjectManager.cpp ends here
