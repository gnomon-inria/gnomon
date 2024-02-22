#pragma once

#include <QtCore>
#include <gnomonProjectExport.h>
#include <gnomonCore/gnomonForm/gnomonAbstractDynamicForm>

#define GNOMON_SESSION gnomonAbstractSessionManager::instance()

#define PROJECT_SESSION_DIRECTORY ".gnomon/session"
#define PROJECT_FORMS_DIRECTORY ".gnomon/session/forms"
#define PROJECT_SESSION_FILE ".gnomon/session/session.ini"
#define PROJECT_PIPELINE_FILE ".gnomon/session/pipeline.json"

class QQmlApplicationEngine;
class gnomonPipeline;

class GNOMONPROJECT_EXPORT gnomonAbstractSessionManager: public QObject {
    Q_OBJECT
public:
    gnomonAbstractSessionManager(QObject *parent= nullptr);

public:
    Q_PROPERTY(double progress READ progress NOTIFY progressChanged);

    double progress();
    void setProgress(double);

public:
    Q_INVOKABLE virtual bool save(const QString &id, const QString &path) = 0;

    Q_INVOKABLE virtual void load(const QString &id, const QString &path) = 0;

    Q_INVOKABLE virtual bool load() = 0;

    Q_INVOKABLE virtual bool newSession(const QString &source = "qrc:/gnomonQuick/Workspaces/WorkspaceBrowsing.qml") = 0;

    virtual bool loadFromPipelineFile(const QString &path) = 0;
    virtual bool loadFromPipeline(gnomonPipeline *pipeline) = 0;

public:
    virtual void setEngine(QQmlApplicationEngine *engine) = 0;

    Q_INVOKABLE virtual void setWindow(QObject *window) = 0;

    Q_INVOKABLE virtual int loadWorkspace(const QString &source, const QString &uuid) = 0;

    Q_INVOKABLE virtual int newWorkspace(const QString &source) = 0;
    
    Q_INVOKABLE virtual void setActiveWorkspace(int id) = 0;

    virtual QJsonObject* getStorageForWorkspace(const QString &uuid) = 0;

public slots:
    virtual void sync() = 0;

public:
    static gnomonAbstractSessionManager *instance();

public:
    bool addForm(const std::shared_ptr<gnomonAbstractDynamicForm>& form);
    bool trackForm(const std::shared_ptr<gnomonAbstractDynamicForm>& form);
    std::shared_ptr<gnomonAbstractDynamicForm> getForm(const QString& uuid);

signals:
    void progressChanged(void);
    void finished(void);
    void failed(QString);

protected:
    void cleanExpiredForms();

    double m_progress = 0.; // from 0 to 1
    static void registerInstance(gnomonAbstractSessionManager *o);

    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm>> m_owned_forms;
    QMap<QString, std::weak_ptr<gnomonAbstractDynamicForm>> m_tracked_forms;

private:
    static gnomonAbstractSessionManager *s_instance;
    static std::mutex s_mutex;
};