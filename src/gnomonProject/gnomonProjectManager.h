#pragma once

#include <gnomonProjectExport.h>

#include <QtCore>

#define GNOMON_PROJECT gnomonProjectManager::instance()->project()

class gnomonProject;

class GNOMONPROJECT_EXPORT gnomonProjectManager : public QObject
{
    Q_OBJECT
    
public:
    static gnomonProjectManager *instance(void);

public:
    gnomonProjectManager(gnomonProjectManager &) = delete;
    void operator=(const gnomonProjectManager &) = delete;
public:
    Q_PROPERTY(gnomonProject *project READ project CONSTANT);
    
public:
    Q_INVOKABLE gnomonProject *openProject(const QString &path);
    Q_INVOKABLE gnomonProject *newProject(const QString &path, const QString & name);

    gnomonProject *project(void);

protected:
    gnomonProjectManager(QObject *parent = nullptr);
    ~gnomonProjectManager(void);

private:
    class gnomonProjectManagerPrivate *d;
    
private: 
    static gnomonProjectManager *s_instance;
    static std::mutex s_mutex;
};


//
// gnomonProjectManager.h ends here
