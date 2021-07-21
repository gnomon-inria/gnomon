#pragma once

#include <gnomonWorkspaceExport>

#include <dtkWidgets>

class GNOMONWORKSPACE_EXPORT gnomonWorkspaceDatabaseBrowser : public dtkWidgetsWorkspace
{
    Q_OBJECT

public:
     gnomonWorkspaceDatabaseBrowser(QWidget *parent = nullptr);
    ~gnomonWorkspaceDatabaseBrowser(void);

public:
    void enter(void) override;
    void leave(void) override;

public:
    void apply(void) override;

public:
    static const QColor color;

public:
    static bool isEmpty(void);

private:
    class gnomonWorkspaceDatabaseBrowserPrivate *d;
};

//
// gnomonWorkspaceDatabaseBrowser.h ends here
