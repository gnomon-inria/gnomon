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
    registerForm(form);
    if(!s_owned_forms.contains(form->uuid())) {
        s_owned_forms[form->uuid()] = form;
        return true;
    } else {
        return false;
    }

}

std::shared_ptr<gnomonAbstractDynamicForm> gnomonAbstractSessionManager::getForm(const QString& uuid)
{
    if (s_owned_forms.contains(uuid)) {
        return s_owned_forms[uuid];
    } else if(s_followed_forms.contains(uuid)) {
        return s_followed_forms[uuid].lock();
    } else {
        dtkWarn()<<Q_FUNC_INFO<<"No existing Form with UUID"<<uuid<<"!";
        return nullptr;
    }
}

bool gnomonAbstractSessionManager::registerForm(const std::shared_ptr<gnomonAbstractDynamicForm>& form) {
    // first, cleaning up expired pointers
    cleanExpiredForms();

    if(!s_followed_forms.contains(form->uuid())) {
        auto project_dir = QDir(GNOMON_PROJECT->projectDir());
        QString path = QString(PROJECT_FORMS_DIRECTORY) + "/" + form->uuid();
        project_dir.mkpath(path);
        form->setFormStorageDir(project_dir.filePath(path));
        s_followed_forms[form->uuid()] = form;
        sync();
        return true;
    } else {
        return false;
    }
}

void gnomonAbstractSessionManager::cleanExpiredForms() {
    auto keys = s_followed_forms.keys();
    for(const auto &uuid: keys) {
        qDebug() << "$$ form: " << uuid << " >> n ref: " << s_followed_forms[uuid].use_count();
        //TODO: some references left because commands are not cleaned (especially outputs)
        if(s_followed_forms[uuid].expired()) {
            s_followed_forms.remove(uuid);
        }
    }
    QDir dir(GNOMON_PROJECT->projectDir());
    QDir form_dir(dir.filePath(PROJECT_FORMS_DIRECTORY));
    for(const auto &file_info : form_dir.entryInfoList()) {
        if(file_info.isDir() && !QUuid::fromString(file_info.fileName()).isNull()
           && !s_followed_forms.contains(file_info.fileName())) {
            gnomonProject::recursiveRemoveDir(file_info.filePath());
        }
    }
}
