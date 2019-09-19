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

#include "gnomonWorkspace.h"

#include <dtkWidgets>

class GNOMONWORKSPACE_EXPORT gnomonWorkspacePreprocess : public dtkWidgetsWorkspace
{
    Q_OBJECT

public:
     gnomonWorkspacePreprocess(QWidget *parent = nullptr);
    ~gnomonWorkspacePreprocess(void);

public:
    void enter(void) override;
    void leave(void) override;

public slots:
    void apply(void) override;
    void configure(const QString& text);

private:
    class gnomonWorkspacePreprocessPrivate *d;
};

//
// gnomonWorkspacePreprocess.h ends here
