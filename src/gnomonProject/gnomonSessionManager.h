#pragma once

#include <gnomonProjectExport.h>

#include <QtCore>

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

private:
    static gnomonSessionManager *s_instance;
    static std::mutex s_mutex;

};

//
// gnomonSessionManager.h ends here

