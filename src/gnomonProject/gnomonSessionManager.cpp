#include "gnomonSessionManager.h"

// /////////////////////////////////////////////////////////////////
// gnomonSessionManagerPrivate
// /////////////////////////////////////////////////////////////////
class gnomonSessionManagerPrivate
{
public:
    //gnomonSession * session;
    // Map to store sessions
    
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

bool gnomonSessionManager::save(const string &id, const string &path)
{
    /* save a session into a map,
    the session is retrivable wit it's Id
    */
   // d->session_process->save_session(d->session);
}
void gnomonSessionManager::load(const string &id, const string &path)
{
    /* Should return a session */
    // d->session_process->load_session(id, path)
}


gnomonSessionManager *gnomonSessionManager::s_instance = nullptr;
std::mutex gnomonSessionManager::s_mutex;


//
// gnomonSessionManager.cpp ends here
