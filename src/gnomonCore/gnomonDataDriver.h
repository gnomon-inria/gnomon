#pragma once

#include <QtCore>

#include <dtkCore>

#include <gnomonCoreExport>
#include <gnomonCore.h>

class gnomonAbstractDataDriver;

class GNOMONCORE_EXPORT gnomonDataDriver
{

public:
    static gnomonDataDriver *instance(const QString& name = "gnomonDataDriverMongo");

protected:
    static gnomonDataDriver *s_instance;

public:
    QString name(void);

    // return ths inserted _id
    QString insert(const QString& doc);
    bool delete_one(const QString& key);
    bool protect(const QString& key);
    QStringList find(const QString& query);

private:
    gnomonDataDriver(const QString& name);
    ~gnomonDataDriver(void);

private:
    gnomonAbstractDataDriver *d = nullptr;

};

