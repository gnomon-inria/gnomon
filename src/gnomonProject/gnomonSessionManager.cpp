#include "gnomonSessionManager.h"

// /////////////////////////////////////////////////////////////////
// gnomonSessionManagerPrivate
// /////////////////////////////////////////////////////////////////
class gnomonSessionManagerPrivate
{
public:
};

// /////////////////////////////////////////////////////////////////
// gnomonSessionManager
// /////////////////////////////////////////////////////////////////

gnomonSessionManager *gnomonSessionManager::instance() {
    std::lock_guard<std::mutex> lock(s_mutex);
    if(!s_instance)
        s_instance = new gnomonSessionManager;
    return s_instance;
}
gnomonSessionManager::gnomonSessionManager(QObject *parent) : QObject(parent)
{
    d = new gnomonSessionManagerPrivate;
}

gnomonSessionManager::~gnomonSessionManager(void)
{
    delete d;
}

gnomonSessionManager *gnomonSessionManager::s_instance = nullptr;
std::mutex gnomonSessionManager::s_mutex;


//
// gnomonSessionManager.cpp ends here
