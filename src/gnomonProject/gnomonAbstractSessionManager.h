#pragma once

#include <QtCore>
#include <gnomonProjectExport.h>

class GNOMONPROJECT_EXPORT gnomonAbstractSessionManager: public QObject {
    Q_OBJECT
public:
    gnomonAbstractSessionManager(QObject *parent= nullptr);

public:
    virtual bool save(const QString &id, const QString &path) = 0;

    virtual void load(const QString &id, const QString &path) = 0;

public:
    static gnomonAbstractSessionManager *instance();

protected:
    static void registerInstance(gnomonAbstractSessionManager *o);

private:
    static gnomonAbstractSessionManager *s_instance;
    static std::mutex s_mutex;
};