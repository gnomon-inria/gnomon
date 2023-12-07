#pragma once

#include <gnomonProjectExport.h>

#include <QtCore>

class gnomonAbstractSessionManager;
struct gnomonProjectInfo {
    QString name;
    QString description;
    QString path;
    QString default_source;
    QDateTime lastModified;
    QStringList data_path;
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
    Q_PROPERTY(QStringList dataPath READ dataPath NOTIFY dataPathChanged);

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
    QStringList dataPath(void);

public:
    void setCurrentDir(const QString& url);
    bool wasSaved = false;
    Q_INVOKABLE void addDirectoryToDataPath(const QString& path);
    Q_INVOKABLE bool isAccessible(const QString& path) const;
    Q_INVOKABLE QString relativePath(const QString& path) const;

public:
    static QVariantMap readProjectInfoFromPath(const QString &path);
    Q_INVOKABLE QString findFile(const QString& filename) const;

    void addToManifest(const QJsonObject& workspace_info);
    bool backupFile(const QString &fname, const QString &content);

    // TODO: we actually want the project to restore the file content
    // const QString& restoreFile(const QString &fname);

    QStringList editorFileInfo(const QStringList& extensions);
    QList< QPair<QString, QString> > browserFormInfo(void);

    static bool isDirAProject(const QDir &dir);

signals:
    void currentDirChanged(void);
    void dataPathChanged(void);
    
private:
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

