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

#include <QtWidgets>

class gnomonWorkspaceBrowser : public QSplitter
{
    Q_OBJECT

public:
     gnomonWorkspaceBrowser(QWidget *parent = nullptr);
    ~gnomonWorkspaceBrowser(void);

private:
    class gnomonWorkspaceBrowserPrivate *d;
};

//
// gnomonWorkspaceBrowser.h ends here
