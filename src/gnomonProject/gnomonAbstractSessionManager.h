#pragma once

#include <QtCore>
#include <gnomonProjectExport.h>

#define GNOMON_SESSION gnomonAbstractSessionManager::instance()

class GNOMONPROJECT_EXPORT gnomonAbstractSessionManager: public QObject {
    Q_OBJECT
public:
    gnomonAbstractSessionManager(QObject *parent= nullptr);

public:
    Q_PROPERTY(double progress READ progress NOTIFY progressChanged);

    double progress();
    void setProgress(double);

public:
    virtual bool save(const QString &id, const QString &path) = 0;

    virtual void load(const QString &id, const QString &path) = 0;

    virtual bool loadFromPipeline(const QString &path, QObject *window) = 0;

public:
    static gnomonAbstractSessionManager *instance();

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