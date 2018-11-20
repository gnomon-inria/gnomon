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

#include "gnomonWorkspace.h"

#include <QtWidgets>

class gnomonWorkspaceBrowser : public gnomonWorkspace
{
    Q_OBJECT

public:
     gnomonWorkspaceBrowser(QWidget *parent = nullptr);
    ~gnomonWorkspaceBrowser(void);

public:
    void enter(void) override {};
    void leave(void) override {};

public slots:
    void replaceChannels(QStringList);
    void replaceChannel(QListWidgetItem *current_item, QListWidgetItem *previous_item);

private:
    class gnomonWorkspaceBrowserPrivate *d;
};

//
// gnomonWorkspaceBrowser.h ends here
