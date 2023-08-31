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
    static gnomonProject *newProject(const QString &path, const QString &name);


private:
    bool isDirAProject(const QDir &dir);
    void populateNewProject();
    void readProjectInfo();

private:
   class gnomonProjectPrivate *d;
};

//
// gnomonProject.h ends here

