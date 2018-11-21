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

#include <gnomonWidgetsExport>

#include "gnomonWorkspace.h"

class GNOMONWIDGETS_EXPORT gnomonWorkspacePreprocess : public gnomonWorkspace
{
    Q_OBJECT

public:
     gnomonWorkspacePreprocess(QWidget *parent = nullptr);
    ~gnomonWorkspacePreprocess(void);

public:
    void enter(void) override {}
    void leave(void) override {}

public slots:
    void apply(void);
    void configure(const QString& text);

private:
    class gnomonWorkspacePreprocessPrivate *d;
};

//
// gnomonWorkspacePreprocess.h ends here
