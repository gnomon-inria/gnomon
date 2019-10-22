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

class GNOMONWORKSPACE_EXPORT gnomonWorkspaceTreeFromLString : public dtkWidgetsWorkspace
{
    Q_OBJECT

public:
     gnomonWorkspaceTreeFromLString(QWidget *parent = nullptr);
    ~gnomonWorkspaceTreeFromLString(void);

public:
    void enter(void) override;
    void leave(void) override;

public slots:
    void apply(void) override;
    void configure(const QString& text);

private:
    class gnomonWorkspaceTreeFromLStringPrivate *d;
};

//
// gnomonWorkspaceTreeFromLString.h ends here
