// Version: $Id$
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#pragma once

#include <gnomonWorkspaceExport>

#include <QtCore>

class GNOMONWORKSPACE_EXPORT gnomonWorkspaceBrowser : public QObject
{
    Q_OBJECT

public:
     gnomonWorkspaceBrowser(QObject *parent = nullptr);
    ~gnomonWorkspaceBrowser(void);

private:
    class gnomonWorkspaceBrowserPrivate *d;
};

//
// gnomonWorkspaceBrowser.h ends here
