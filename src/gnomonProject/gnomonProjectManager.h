#pragma once

#include <gnomonProjectExport.h>

#include <QtCore>

class gnomonProject;

class GNOMONPROJECT_EXPORT gnomonProjectManager : public QObject
{
    Q_OBJECT
    
public:
    gnomonProjectManager(QObject *parent = nullptr);
    ~gnomonProjectManager(void);
private:
    class gnomonProjectManagerPrivate *d;
private: 
    static gnomonProjectManager *s_instance;
};


//
// gnomonProjectManager.h ends here
