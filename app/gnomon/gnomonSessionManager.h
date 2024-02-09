#pragma once

#include <gnomonProject/gnomonAbstractSessionManager.h>

#include <QtCore>

class gnomonSessionManagerPrivate;

class GNOMONPROJECT_EXPORT gnomonSessionManager : public gnomonAbstractSessionManager
{

public:
    gnomonSessionManager(gnomonSessionManager &) = delete;
    void operator=(const gnomonSessionManager &) = delete;

protected:
    gnomonSessionManager(QObject *parent);
    ~gnomonSessionManager(void);

public:
    bool save(const QString &id, const QString &path) override;
    void load(const QString &id, const QString &path) override;

    bool load() override;

    bool loadFromPipeline(const QString &path) override;

public:
    void setEngine(QQmlApplicationEngine *engine) override;

    void setWindow(QObject* window) override;

public:
    static void initialize();

    int loadWorkspace(const QString &source, const QString &uuid) override;

    int newWorkspace(const QString &source) override;

    void setActiveWorkspace(int id) override;

    QJsonObject *getStorageForWorkspace(const QString &uuid) override;

    void sync() override;

    bool newSession(const QString &source) override;

protected:
    void timerEvent(QTimerEvent *event) override;

private:
    gnomonSessionManagerPrivate *d = nullptr;

};

//
// gnomonSessionManager.h ends here

