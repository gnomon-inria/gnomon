#pragma once

#include <QtCore>
#include <gnomonProjectExport.h>
#include <gnomonCore/gnomonForm/gnomonAbstractDynamicForm>

#define GNOMON_SESSION gnomonAbstractSessionManager::instance()

class QQmlApplicationEngine;

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

    virtual bool loadFromPipeline(const QString &path) = 0;

public:
    virtual void setEngine(QQmlApplicationEngine *engine) = 0;

    Q_INVOKABLE virtual void setWindow(QObject *window) = 0;

    Q_INVOKABLE virtual int loadWorkspace(const QString &source, const QString &uuid) = 0;

    Q_INVOKABLE virtual int newWorkspace(const QString &source) = 0;

    virtual QJsonObject* getStorageForWorkspace(const QString &uuid) = 0;

public slots:
    virtual void sync() = 0;

public:
    static gnomonAbstractSessionManager *instance();

public:
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm>> forms;

signals:
    void progressChanged(void);
    void finished(void);
    void failed(QString);

protected:
    double m_progress = 0.; // from 0 to 1
    static void registerInstance(gnomonAbstractSessionManager *o);

private:
    static gnomonAbstractSessionManager *s_instance;
    static std::mutex s_mutex;
};