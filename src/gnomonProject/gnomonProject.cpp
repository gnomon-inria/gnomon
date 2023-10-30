#include "gnomonProject.h"
#include "gnomonProject"


#define PROJECT_INFO_FOLDER ".gnomon"
#define PROJECT_INFO_FILE ".gnomon/project.json"
#define PROJECT_BACKUP_FOLDER ".gnomon/backup"
#define PROJECT_BACKUP_MANIFEST ".gnomon/backup/manifest.json"
#define PROJECT_MANIFEST_FILE ".gnomon/manifest.json"

// /////////////////////////////////////////////////////////////////
// gnomonProjectPrivate
// /////////////////////////////////////////////////////////////////

class gnomonProjectPrivate {

public:
    explicit gnomonProjectPrivate(const QString &path);
    ~gnomonProjectPrivate();

public:
    void initFile(const QString& path);
    QJsonObject readFromJson(const QString& url);

public:

    gnomonProjectInfo projectInfo;

    QDir projectDir;
    QDir currentDir;

    QString manifest_url;
    QHash<QString, int> workspace_obj_count;
};

gnomonProjectPrivate::gnomonProjectPrivate(const QString &path): 
    projectDir(path), 
    currentDir(path)
{
    QDir::setCurrent(path);
    projectInfo.path = path;
}

gnomonProjectPrivate::~gnomonProjectPrivate()
{

}

void gnomonProjectPrivate::initFile(const QString& path)
{
    QFile file(path);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << Q_FUNC_INFO << "can't open file " << path;
        return;
    }
    file.close();
}

QJsonObject gnomonProjectPrivate::readFromJson(const QString& url)
{
    QByteArray storage;
    QFile f(url);
    if(f.open(QIODevice::ReadOnly| QIODevice::Text)) {
        storage = f.readAll();
        f.close();
    }
    return QJsonDocument::fromJson(storage).object();;
}
// /////////////////////////////////////////////////////////////////
// gnomonProject
// /////////////////////////////////////////////////////////////////



gnomonProject::gnomonProject(const QString &path): QObject(nullptr) {
    QString path_copy = sanitizeUrlToPath(path);
    d = new gnomonProjectPrivate(path_copy);

    if(!d->projectDir.exists()) {
        QDir::current().mkpath(path_copy);
    }

    bool isProject = isDirAProject(d->projectDir);
    if(isProject) {
        readProjectInfo();
        d->manifest_url = d->projectDir.filePath(PROJECT_BACKUP_MANIFEST);
        wasSaved = QFile::exists(d->projectDir.filePath(PROJECT_MANIFEST_FILE));
    } else {
        auto pName = d->projectDir.dirName();
        populateNewProject();
        d->projectInfo.name = pName;
        d->projectInfo.lastModified = QDateTime::currentDateTime();
    };
}

gnomonProject::~gnomonProject(void)
{
    delete d;
}

QVariantMap gnomonProject::readProjectInfoFromPath(const QString &path) {
    QVariantMap pInfo;
    QString path2 = sanitizeUrlToPath(path);
    QDir projectDir(path2);
    QFile projectInfoFile(projectDir.absoluteFilePath(PROJECT_INFO_FILE));
    if(projectInfoFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&projectInfoFile);
        auto content = in.readAll().toUtf8();
        QJsonDocument doc = QJsonDocument::fromJson(content);
        QJsonObject storage = doc.object();
        pInfo = storage.toVariantMap();

        projectInfoFile.close();
    }
    return pInfo;
}

void gnomonProject::readProjectInfo() {
    auto storage = readProjectInfoFromPath(projectDir());
    auto &pInfo = d->projectInfo;
    pInfo.name = storage["name"].toString();
    pInfo.description = storage["description"].toString();
    pInfo.path = storage["path"].toString();
    pInfo.default_source = storage["default_source"].toString();
    pInfo.lastModified = QDateTime::fromString(storage["lastModified"].toString(), Qt::ISODate);
}

void gnomonProject::saveProjectInfo() {
    QFile projectInfoFile(d->projectDir.absoluteFilePath(PROJECT_INFO_FILE));
    if(projectInfoFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        auto &pInfo = d->projectInfo;
        QJsonObject storage;
        storage["name"] = pInfo.name;
        storage["description"] = pInfo.description;
        storage["path"] = pInfo.path;
        storage["default_source"] = pInfo.default_source;
        pInfo.lastModified.setSecsSinceEpoch(QDateTime::currentSecsSinceEpoch());
        storage["lastModified"] = pInfo.lastModified.toString("yyyy-MM-ddTHH:mm:ss");
        QJsonDocument doc(storage);
        QTextStream out(&projectInfoFile);
        out << doc.toJson();
        out.flush();
        projectInfoFile.close();
    }
}

void gnomonProject::populateNewProject() {
    d->projectDir.mkdir(PROJECT_INFO_FOLDER);
    saveProjectInfo();
    d->projectDir.mkdir(PROJECT_BACKUP_FOLDER);

    d->manifest_url = d->projectDir.filePath(PROJECT_BACKUP_MANIFEST);
    d->initFile(d->manifest_url);
}

bool gnomonProject::isDirAProject(const QDir &dir) {
    return dir.exists(PROJECT_INFO_FOLDER) && dir.exists(PROJECT_BACKUP_FOLDER);
}

QString gnomonProject::projectDir(void)
{
    return d->currentDir.path();
}

void gnomonProject::setCurrentDir(const QString& url)
{
    auto path = sanitizeUrlToPath(url);
    if(path != d->currentDir.path()) {
        d->currentDir.setPath(path);
        emit currentDirChanged();
    }
}

bool gnomonProject::loadSessionFromPipeline(const QString &path, QObject *window)
{
    GNOMON_SESSION->loadFromPipeline(path);
}

gnomonProject *gnomonProject::newProject(const QString &path, const QString &name, const QString &description,
                                         const QString &source) {
    auto project = new gnomonProject(path);
    project->d->projectInfo.name = name;
    project->d->projectInfo.description = description;
    project->d->projectInfo.default_source = source;
    project->saveProjectInfo();
    return project;
}

void gnomonProject::close() {
    qWarning()<<Q_FUNC_INFO<<"Not implemented.";
}

QString gnomonProject::currentDir(void) {
    return d->currentDir.path();
}

gnomonAbstractSessionManager* gnomonProject::currentSession(void)
{
    return GNOMON_SESSION;
}

QString gnomonProject::sanitizeUrlToPath(const QString &url) {
    QUrl _url(url);
    return QString(_url.isValid() && _url.isLocalFile() ? _url.toLocalFile() : url);
}

const gnomonProjectInfo &gnomonProject::projectInfo() {
    return d->projectInfo;
}

void gnomonProject::addToManifest(const QJsonObject& workspace_info)
{
    QFile file(d->manifest_url);
    QJsonObject old_obj;
    QJsonObject new_obj;
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QByteArray content = file.readAll();
        old_obj = QJsonDocument::fromJson(content).object();
        file.close();
    }
    auto object_exist = [&](){
        for(const QString& key : old_obj.keys()) {
            for(auto&& item: old_obj[key].toArray()) {
                if (item == workspace_info[key]) {
                    return true;
                }
            }
        }
        return false;
    };
    if(!object_exist()) {
        if(old_obj.contains(workspace_info.keys()[0])) {
            QJsonValueRef data_ref = old_obj[workspace_info.keys()[0]];
            QJsonArray old_data = data_ref.toArray();
            old_data.append(workspace_info.value(workspace_info.keys()[0]));
            old_obj[workspace_info.keys()[0]] = old_data;
        } else {
            QJsonArray data_info;
            data_info.append(workspace_info.value(workspace_info.keys()[0]));
            new_obj.insert(workspace_info.keys()[0], data_info);
        }
    }
    if(!old_obj.empty()) {
        for(const QString& key: old_obj.keys()){
            new_obj.insert(key, old_obj.value(key));
        }
    }

    QJsonDocument session_doc(new_obj);
    if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        file.write(session_doc.toJson());
        file.close();
    }
}

bool gnomonProject::backupFile(const QString &fname, const QString &content)
{
    auto file_path = d->projectDir.filePath(PROJECT_BACKUP_FOLDER + QString("/") + fname);
    QFile f(file_path);
    if(f.open(QIODevice::WriteOnly| QIODevice::Text)) {
        QTextStream out(&f);
        out<<content;
        f.close();
    } else {
        return false;
    }
    return true;
}

void gnomonProject::save(void)
{
    QFile old_manifest(d->projectDir.filePath(PROJECT_MANIFEST_FILE));
    if(old_manifest.exists())
        old_manifest.remove();
    QFile::copy(d->projectDir.filePath(PROJECT_BACKUP_MANIFEST),
                d->projectDir.filePath(PROJECT_MANIFEST_FILE));
}

QStringList gnomonProject::editorFileInfo(const QStringList& extensions)
{
    QDir backup_dir(d->projectDir.filePath(PROJECT_BACKUP_FOLDER));

    QStringList editor_files;
    for(const QFileInfo& file_info: backup_dir.entryInfoList())
    {
        if (file_info.isFile()) {
            for (const auto& ext : extensions) {
                if (file_info.suffix() == ext) {
                    editor_files.append(file_info.filePath());
                    break;
                }
            }
        }
    }

    return editor_files;
}

QList< QPair<QString, QString> > gnomonProject::browserFormInfo(void)
{
    QList< QPair<QString, QString> > restore_info;
    QJsonObject doc_obj = d->readFromJson(d->projectDir.filePath(PROJECT_MANIFEST_FILE));
    QJsonArray data_info = doc_obj.value(doc_obj.keys().last()).toArray();

    restore_info.append({
        data_info.first().toObject().value("path").toString(),
        data_info.first().toObject().value("plugin_name").toString()
    });

    return restore_info;
}

QJsonObject gnomonProject::workspaceAlgoInfo(const QString& workspace_name)
{
    QJsonObject workspace_info;
    QJsonArray workspace_array;
    QJsonObject doc_obj = d->readFromJson(d->projectDir.filePath(PROJECT_MANIFEST_FILE));
    QJsonObject::const_iterator it = doc_obj.constBegin();
    while(it != doc_obj.constEnd()) {
        auto it_val = it.value().toArray().first().toObject().value("workspace_name").toString();
        if(it_val == workspace_name) {
            QJsonObject temp_obj;
            temp_obj.insert(it.key(), it.value().toArray());
            workspace_array.push_back(temp_obj);
        }
        ++it;
    }
    if(workspace_array.count() > d->workspace_obj_count[workspace_name]) {
        workspace_info = workspace_array[d->workspace_obj_count[workspace_name]].toObject();
    }
    d->workspace_obj_count[workspace_name]++;
    return workspace_info;
}
//
// gnomonProject.cpp ends here
