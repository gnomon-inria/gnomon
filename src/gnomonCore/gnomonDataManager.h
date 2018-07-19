
#pragma once

#include <QtCore>

#include <gnomonCoreExport>

class dtkImage;

class GNOMONCORE_EXPORT gnomonDataManager: public QObject
{
    Q_OBJECT

public:
    static gnomonDataManager *instance(void);

protected:
    static gnomonDataManager *s_instance;

    //public:
    //void insert();
    
public:
    dtkImage *get(const QString& name);

private:
    gnomonDataManager(void);
    ~gnomonDataManager(void);

private:
    class gnomonDataManagerPrivate *d;
};
