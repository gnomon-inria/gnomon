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

bool gnomonAbstractSessionManager::addForm(const std::shared_ptr<gnomonAbstractDynamicForm>& form)
{
    this->trackForm(form);
    if(!this->m_owned_forms.contains(form->uuid())) {
        this->m_owned_forms[form->uuid()] = form;
        return true;
    } else {
        return false;
    }

}

std::shared_ptr<gnomonAbstractDynamicForm> gnomonAbstractSessionManager::getForm(const QString& uuid)
{
    if (this->m_owned_forms.contains(uuid)) {
        return this->m_owned_forms[uuid];
    } else if(this->m_tracked_forms.contains(uuid)) {
        return this->m_tracked_forms[uuid].lock();
    } else {
        dtkWarn()<<Q_FUNC_INFO<<"No existing Form with UUID"<<uuid<<"!";
        return nullptr;
    }
}

bool gnomonAbstractSessionManager::trackForm(const std::shared_ptr<gnomonAbstractDynamicForm>& form) {
    // first, cleaning up expired pointers
    this->cleanExpiredForms();

    if(!this->m_tracked_forms.contains(form->uuid())) {
        auto project_dir = QDir(GNOMON_PROJECT->projectDir());
        QString path = QString(PROJECT_FORMS_DIRECTORY) + "/" + form->uuid();
        project_dir.mkpath(path);
        form->setFormStorageDir(project_dir.filePath(path));
        this->m_tracked_forms[form->uuid()] = form;
        this->sync();
        return true;
    } else {
        return false;
    }
}

void gnomonAbstractSessionManager::cleanExpiredForms() {
    auto keys = this->m_tracked_forms.keys();
    for(const auto &uuid: keys) {
        qDebug() << "$$ form: " << uuid << " >> n ref: " << m_tracked_forms[uuid].use_count();
        //TODO: some references left because commands are not cleaned (especially outputs)
        if(this->m_tracked_forms[uuid].expired()) {
            this->m_tracked_forms.remove(uuid);
        }
    }
    QDir dir(GNOMON_PROJECT->projectDir());
    QDir form_dir(dir.filePath(PROJECT_FORMS_DIRECTORY));
    for(const auto &file_info : form_dir.entryInfoList()) {
        if(file_info.isDir() && !QUuid::fromString(file_info.fileName()).isNull()
           && !this->m_tracked_forms.contains(file_info.fileName())) {
            gnomonProject::recursiveRemoveDir(file_info.filePath());
        }
    }
}
