#include "gnomonAbstractSessionManager.h"

gnomonAbstractSessionManager *gnomonAbstractSessionManager::s_instance = nullptr;

gnomonAbstractSessionManager::gnomonAbstractSessionManager(QObject *parent) : QObject(parent) {

}

gnomonAbstractSessionManager *gnomonAbstractSessionManager::instance() {
    Q_ASSERT(s_instance);
    return s_instance;
}

void gnomonAbstractSessionManager::registerInstance(gnomonAbstractSessionManager *o) {
    s_instance = o;
}
