#pragma once

#include <gnomonProjectExport.h>

#include <QtCore>

class GNOMONPROJECT_EXPORT gnomonProject : public QObject
{
    Q_OBJECT

public:
    gnomonProject(QObject *parent = nullptr) = delete;
    explicit gnomonProject(const QString &path);
    ~gnomonProject(void) override;

public:
    Q_PROPERTY(QString projectDir READ projectDir WRITE setProjectDir NOTIFY projectDirChanged);

public:
    static gnomonProject *newProject(const QString &path, const QString &name);

public:
    const QString& projectDir(void);

public:
    void setProjectDir(const QString& url);

signals:
    void projectDirChanged(void);
    
private:
    bool isDirAProject(const QDir &dir);
    void populateNewProject();
    void readProjectInfo();

private:
   class gnomonProjectPrivate *d;
};

//
// gnomonProject.h ends here

