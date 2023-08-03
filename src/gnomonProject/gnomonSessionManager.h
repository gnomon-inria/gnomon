#pragma once

#include <gnomonProjectExport.h>

#include <QtCore>

class gnomonSessionProcess

class GNOMONPROJECT_EXPORT gnomonSessionManager : public QObject
{
    Q_OBJECT

public:
    static gnomonSessionManager *instance(void);

public:
    gnomonSessionManager(gnomonSessionManager &) = delete;
    void operator=(const gnomonSessionManager &) = delete;

protected:
    gnomonSessionManager(QObject *parent);
    ~gnomonSessionManager(void);

public:
    bool save(const string &id, const string &path);
    void load(const string &id, const string &path);

private:
    static gnomonSessionManager *s_instance;
    static std::mutex s_mutex;

};

//
// gnomonSessionManager.h ends here

