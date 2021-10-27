#include "gnomonProjectManager.h"
#include "gnomon"

gnomonProjectManager *gnomonProjectManager::s_instance = nullptr;


gnomonProject *loadFromFile(const QUrl& url) {
    QFile file(url.path());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return nullptr;

    QByteArray data = file.readAll();
    file.close();

    QJsonParseError parse_error;
    QJsonDocument project_doc = QJsonDocument::fromJson(data, &parse_error);
    if (project_doc.isNull()) {
        qWarning() << "Parsing of " << url.path() << "failed";
        return nullptr;
    }

    QJsonObject rootObj = project_doc.object();

    gnomonProject *p = new gnomonProject;
    p->name = rootObj.value("name").toString();
    p->context = rootObj.value("context").toString();
    p->icon = rootObj.value("icon").toString().toUtf8(); //TODO test

    QJsonArray tags_array = rootObj.value("tags").toArray();
    for(auto && val : tags_array) {
        p->tags.append(val.toString());
    }

    //TODO check version
    //project_json.insert("gnomonVersion", GNOMON_VERSION);
    //project_json.insert("fileFormatVersion", "0.0.1");

    //TODO pipeline
    //project_json.insert("pipeline", "TODO");

    return p;
}

gnomonProjectManager *gnomonProjectManager::instance()
{
    if(!s_instance)
        s_instance = new gnomonProjectManager();

    return s_instance;
};

gnomonProjectManager::gnomonProjectManager(void)
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "inria", "gnomon");
    QString default_project_dir = QDir::homePath() + QDir::separator() + "gnomonProjects";
    QString projects_path = settings.value("general/projects_path", default_project_dir).toString();

    //ensure the directory exist and go there
    QDir project_dir;
    project_dir.mkpath(projects_path);
    project_dir.setPath(projects_path);

    //load existing projects
    //each file is a project
    for(QString f : project_dir.entryList(QDir::Files, QDir::Name)) {
        gnomonProject *p = loadFromFile( QUrl(projects_path + QDir::separator() + f));
        if(p)
            m_projects.append(p);
    }

    m_currentProjectIndex = 0;
    if(m_projects.size() < 1) {
        gnomonProject *currentProject = new gnomonProject;
        currentProject->name = "NoNameProject";
        m_projects.append(currentProject);
    }
}

gnomonProjectManager::~gnomonProjectManager(void)
{
    for(auto *p : m_projects) {
        delete p;
    }
    m_projects.clear();
    m_currentProjectIndex = -1;
}


gnomonProject* gnomonProjectManager::currentProject(void) const
{
    return m_projects.at(m_currentProjectIndex);
}

QList<gnomonProject*> gnomonProjectManager::projects(void) const
{
    return m_projects;
};
gnomonProject* gnomonProjectManager::load(const QString& name)
{
    int idx = 0;
    for(auto&& p : m_projects) {
        if(p->name == name) {
            m_currentProjectIndex = idx;
            return currentProject();
        }
    }

    qWarning() << "Cannot found project: " << name << " no project loaded";
    return currentProject();
}

gnomonProject* gnomonProjectManager::load(const QUrl& url)
{
    gnomonProject *p = loadFromFile(url);
    if(!p) {
            qWarning() << "Cannot found project at: " << url << " no project loaded";
    } else {
        //check if a project with same name already exist
        for(auto&& pr : m_projects) {
            if(pr->name == p->name) {
                qWarning() << "A project with name: " << pr->name << " already exist. I can't load project from " << url;
                return currentProject();
            }
        }

        m_projects.append(p);
        m_currentProjectIndex = m_projects.size();
    }

    return currentProject();
}

bool gnomonProjectManager::create(QString name, QByteArray icon, QStringList tags, QString context)
{
    gnomonProject *p = new gnomonProject;
    p->name = name;
    p->icon = icon;
    p->tags = tags;
    p->context = context;

    //check if a project with same name already exist
    for(auto&& pr : m_projects) {
        if(pr->name == p->name) {
            qWarning() << "A project with name: " << pr->name << " already exist. I can't create project!";
            return false;
        }
    }

    m_projects.append(p);
    m_currentProjectIndex = m_projects.size();
    return true;
}

bool gnomonProjectManager::save(QJsonObject pipeline)
{
    m_projects[m_currentProjectIndex]->pipeline = pipeline;

    //TODO
    //save current project
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "inria", "gnomon");
    QString default_project_dir = QDir::homePath() + QDir::separator() + "gnomonProjects";
    QString projects_path = settings.value("general/projects_path", default_project_dir).toString();
    QString path = projects_path + QDir::separator() + m_projects[m_currentProjectIndex]->name;
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    QJsonObject project_json;
    project_json.insert("type", "project");
    project_json.insert("gnomonVersion", GNOMON_VERSION);
    project_json.insert("fileFormatVersion", "0.0.1");
    project_json.insert("name", m_projects[m_currentProjectIndex]->name);
    project_json.insert("icon", m_projects[m_currentProjectIndex]->icon.data());
    QJsonArray tags_json = QJsonArray::fromStringList(m_projects[m_currentProjectIndex]->tags);
    project_json.insert("tags", tags_json);
    project_json.insert("context", m_projects[m_currentProjectIndex]->context);
    project_json.insert("pipeline", "TODO");

    QJsonDocument project_doc(project_json);
    file.write(project_doc.toJson());
    file.close();

    return true;
}

void gnomonProjectManager::setContext(const QString& context)
{
    m_projects[m_currentProjectIndex]->context = context;
}
void gnomonProjectManager::setIcon(QByteArray icon)
{
    m_projects[m_currentProjectIndex]->icon = icon;
}
void gnomonProjectManager::setName(const QString& name)
{
    m_projects[m_currentProjectIndex]->name = name;
}
void gnomonProjectManager::setTags(QStringList tags)
{
    m_projects[m_currentProjectIndex]->tags = tags;
}