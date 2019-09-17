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

class GNOMONWORKSPACE_EXPORT gnomonWorkspaceCellImageFilter : public dtkWidgetsWorkspace
{
    Q_OBJECT

public:
     gnomonWorkspaceCellImageFilter(QWidget *parent = nullptr);
    ~gnomonWorkspaceCellImageFilter(void);

public:
    void enter(void) override {}
    void leave(void) override {}

public slots:
    void apply(void) override;
    void configure(const QString& text);

private:
    class gnomonWorkspaceCellImageFilterPrivate *d;
};

//
// gnomonWorkspaceCellImageFilter.h ends here
