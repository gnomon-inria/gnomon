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

gnomonSessionManager::gnomonSessionManager(QObject *parent) : gnomonAbstractSessionManager(parent)
{
    d = new gnomonSessionManagerPrivate;
}

gnomonSessionManager::~gnomonSessionManager(void)
{
    delete d;
}

bool gnomonSessionManager::save(const QString &id, const QString &path)
{
    /* save a session into a map,
    the session is retrivable wit it's Id
    */
   // d->session_process->save_session(d->session);
}
void gnomonSessionManager::load(const QString &id, const QString &path)
{
    /* Should return a session */
    // d->session_process->load_session(id, path)
}

void gnomonSessionManager::initialize() {
    gnomonAbstractSessionManager::registerInstance(new gnomonSessionManager(nullptr));
}


//
// gnomonSessionManager.cpp ends here
