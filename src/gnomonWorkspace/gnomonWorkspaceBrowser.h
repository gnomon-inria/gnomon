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

#include <dtkWidgets>

class GNOMONWORKSPACE_EXPORT gnomonWorkspaceBrowser : public dtkWidgetsWorkspace
{
    Q_OBJECT

public:
     gnomonWorkspaceBrowser(QWidget *parent = nullptr);
    ~gnomonWorkspaceBrowser(void);

public:
    void enter(void) override;
    void leave(void) override;

public:
    void apply(void) override;

public:
    static const QColor color;

private:
    class gnomonWorkspaceBrowserPrivate *d;
};

//
// gnomonWorkspaceBrowser.h ends here
