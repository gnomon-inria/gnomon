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

class GNOMONWIDGETS_EXPORT gnomonWorkspaceCellImageQuantification : public gnomonWorkspace
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
