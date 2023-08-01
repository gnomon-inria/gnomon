#pragma once

#include <gnomonProjectExport.h>

#include <QtCore>

class GNOMONPROJECT_EXPORT gnomonProject : public QObject
{
    Q_OBJECT

public:
    gnomonProject(QObject *parent = nullptr);
    ~gnomonProject(void);
private:
   class gnomonProjectPrivate *d;
};

//
// gnomonProject.h ends here

