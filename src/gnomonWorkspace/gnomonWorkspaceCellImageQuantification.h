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

class GNOMONWORKSPACE_EXPORT gnomonWorkspaceCellImageQuantification : public gnomonWorkspace
{
    Q_OBJECT

public:
     gnomonWorkspaceCellImageQuantification(QWidget *parent = nullptr);
    ~gnomonWorkspaceCellImageQuantification(void);

public:
    void enter(void) override {}
    void leave(void) override {}

public slots:
    void apply(void) override;
    void configure(const QString& text);

private:
    class gnomonWorkspaceCellImageQuantificationPrivate *d;
};


//
// gnomonWorkspaceCellImageQuantification.h ends here
