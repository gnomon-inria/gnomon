#pragma once

#include <gnomonProjectExport.h>

#include <QtCore>

class gnomonAbstractSessionManager;
struct gnomonProjectInfo {
    QString name;
    QString description;
    QString path;
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
    static gnomonProject *newProject(const QString &path, const QString &name, const QString &description);
    void close();

    Q_INVOKABLE bool loadSessionFromPipeline(const QString &path, QObject *window);
    gnomonAbstractSessionManager *currentSession(void);
public:
    QString projectDir(void);
    QString currentDir(void);

public:
    void setCurrentDir(const QString& url);

public:
    static QVariantMap readProjectInfoFromPath(const QString &path);

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

