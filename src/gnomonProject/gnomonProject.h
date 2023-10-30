#pragma once

#include <gnomonProjectExport.h>

#include <QtCore>

class gnomonAbstractSessionManager;
struct gnomonProjectInfo {
    QString name;
    QString description;
    QString path;
    QString default_source;
    QString launcher_workspace;
    QDateTime lastModified;
};

class GNOMONPROJECT_EXPORT gnomonProject : public QObject
{
    Q_OBJECT

public:
    gnomonProject(QObject *parent = nullptr) = delete;
    explicit gnomonProject(const QString &path);
    ~gnomonProject(void) override;

public:
    Q_PROPERTY(QString projectDir READ projectDir);
    Q_PROPERTY(QString currentDir READ currentDir WRITE setCurrentDir NOTIFY currentDirChanged);
    Q_PROPERTY(gnomonAbstractSessionManager *currentSession READ currentSession CONSTANT);

public:
    static gnomonProject *newProject(const QString &path, const QString &name, const QString &description,
                                     const QString &source);
    void close();

    Q_INVOKABLE void save(void);
    Q_INVOKABLE bool loadSessionFromPipeline(const QString &path, QObject *window);
    gnomonAbstractSessionManager *currentSession(void);
public:
    QString projectDir(void);
    QString currentDir(void);
    const gnomonProjectInfo &projectInfo();

public:
    void setCurrentDir(const QString& url);
    bool wasSaved = false;

public:
    static QVariantMap readProjectInfoFromPath(const QString &path);
    void addToManifest(const QJsonObject& workspace_info);
    bool backupFile(const QString &fname, const QString &content);

    // TODO: we actually want the project to restore the file content
    // const QString& restoreFile(const QString &fname);

    QStringList editorFileInfo(const QStringList& extensions);
    QList< QPair<QString, QString> > browserFormInfo(void);
    QJsonObject workspaceAlgoInfo(const QString& workspace_name);



signals:
    void currentDirChanged(void);
    
private:
    bool isDirAProject(const QDir &dir);
    void populateNewProject();
    void readProjectInfo();
    static inline QString sanitizeUrlToPath(const QString &url);

private slots:
    void saveProjectInfo();

private:
   class gnomonProjectPrivate *d;
};

//
// gnomonProject.h ends here

