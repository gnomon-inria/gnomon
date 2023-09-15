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

double gnomonAbstractSessionManager::progress()
{
    return this->m_progress;
}

void gnomonAbstractSessionManager::setProgress(double progress)
{
    this->m_progress = progress;
    qInfo() << "Session Loading at "<< int(100*this->m_progress) << "%";
    emit progressChanged();
}