#pragma once

#include <QtCore>

#include <gnomonCoreExport>

struct GNOMONCORE_EXPORT gnomonProject {
    QString name;
    QByteArray icon;
    QString context;
    QStringList tags;
    QJsonObject pipeline;
};

class GNOMONCORE_EXPORT gnomonProjectManager
{

public:
    static gnomonProjectManager *instance();

protected:
    static gnomonProjectManager *s_instance;

public:
    gnomonProject* currentProject(void) const; // return current project

    QList<gnomonProject*> projects(void) const; //list of available projects
    gnomonProject *load(const QString& name); // load a new project
    gnomonProject *load(const QUrl& url); // load a new project

    bool create(QString name, QByteArray icon, QStringList tags, QString context); // create a new project
    bool save(QJsonObject pipeline);

public:
    void setContext(const QString& context);
    void setIcon(QByteArray icon);
    void setName(const QString& name);
    void setTags(QStringList tags);


private:
    gnomonProjectManager();
    ~gnomonProjectManager(void);

private:
    int m_currentProjectIndex = -1;
    QList<gnomonProject *> m_projects; // list of available projects

};
