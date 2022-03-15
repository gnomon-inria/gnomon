#pragma once

#include <QtCore>

#include <gnomonCoreExport>

struct GNOMONCORE_EXPORT gnomonProject {
    Q_GADGET
    Q_PROPERTY(QString name MEMBER m_name)
    Q_PROPERTY(QByteArray icon MEMBER m_icon) //qml ArrayBuffer
    Q_PROPERTY(QStringList tagss MEMBER m_tags)
    Q_PROPERTY(QString context MEMBER m_context)
    Q_PROPERTY(QJsonObject pipeline MEMBER m_pipeline) // pass it as string if pb with qml

public:
    QString m_name;
    QByteArray m_icon;
    QString m_context;
    QStringList m_tags;
    QJsonObject m_pipeline;

};

Q_DECLARE_METATYPE(gnomonProject);

class GNOMONCORE_EXPORT gnomonProjectManager : public QObject
{
    Q_OBJECT

public:
    static gnomonProjectManager *instance();

protected:
    static gnomonProjectManager *s_instance;

public:
    gnomonProject currentProject(void) const; // return current project

    QList<gnomonProject> projects(void) const; //list of available projects
    gnomonProject load(const QString& name); // load a new project
    gnomonProject load(const QUrl& url); // load a new project

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
    QList<gnomonProject> m_projects; // list of available projects

};
