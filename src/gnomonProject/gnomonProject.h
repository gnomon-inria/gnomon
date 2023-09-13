#pragma once

#include <gnomonProjectExport.h>

#include <QtCore>


struct gnomonProjectInfo {
    QString name;
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

public:
    static gnomonProject *newProject(const QString &path, const QString &name);
    void loadSessionFromPipeline(const QString &path);
    void close();

public:
    QString projectDir(void);
    QString currentDir(void);

public:
    void setCurrentDir(const QString& url);

signals:
    void currentDirChanged(void);
    
private:
    bool isDirAProject(const QDir &dir);
    void populateNewProject();
    void readProjectInfo();

private slots:
    void saveProjectInfo();

private:
   class gnomonProjectPrivate *d;
};

//
// gnomonProject.h ends here

