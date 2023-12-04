#include "gnomonAbstractSessionManager.h"
#include "gnomonProject"

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

bool gnomonAbstractSessionManager::addForm(std::shared_ptr<gnomonAbstractDynamicForm> form) 
{
    if(!s_forms.contains(form->uuid())) {
        auto project_dir = QDir(GNOMON_PROJECT->projectDir());
        QString path = QString(PROJECT_FORMS_DIRECTORY) + "/" + form->uuid();
        project_dir.mkpath(path);
        form->setFormStorageDir(project_dir.filePath(path));
        s_forms[form->uuid()] = form;
        sync();
        return true;
    } else {
        return false;
    }

}

std::shared_ptr<gnomonAbstractDynamicForm> gnomonAbstractSessionManager::getForm(const QString& uuid)
{
    if (s_forms.contains(uuid)) {
        return s_forms[uuid];
    } else {
        dtkWarn()<<Q_FUNC_INFO<<"No existing Form with UUID"<<uuid<<"!";
        return nullptr;
    }
}