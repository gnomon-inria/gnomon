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

class GNOMONWORKSPACE_EXPORT gnomonWorkspaceLStringFromTree : public dtkWidgetsWorkspace
{
    Q_OBJECT

public:
     gnomonWorkspaceLStringFromTree(QWidget *parent = nullptr);
    ~gnomonWorkspaceLStringFromTree(void);

public:
    void enter(void) override;
    void leave(void) override;

public slots:
    void apply(void) override;
    void configure(const QString& text);

private:
    class gnomonWorkspaceLStringFromTreePrivate *d;
};

//
// gnomonWorkspaceLStringFromTree.h ends here
