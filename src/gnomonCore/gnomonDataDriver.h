#pragma once

#include <QtCore>

#include <dtkCore>

#include <gnomonCoreExport>
#include <gnomonCore.h>

class gnomonAbstractDataDriver;

class GNOMONCORE_EXPORT gnomonDataDriver
{

public:
    static gnomonDataDriver *instance(const QString& name = "mongo");

protected:
    static gnomonDataDriver *s_instance;

public:
    const QString& name(void);
    bool insert(const QString& doc);
    bool remove(const QString& key);
    bool protect(const QString& key);
    const QString& find(const QString& query);

private:
    gnomonDataDriver(const QString& name);
    ~gnomonDataDriver(void);

private:
    gnomonAbstractDataDriver *d = nullptr;

};

