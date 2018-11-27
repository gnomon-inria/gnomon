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

class GNOMONWIDGETS_EXPORT gnomonWorkspaceFusion : public gnomonWorkspace
{
    Q_OBJECT

public:
     gnomonWorkspaceFusion(QWidget *parent = nullptr);
    ~gnomonWorkspaceFusion(void);

public:
    void enter(void) override {}
    void leave(void) override {}

public slots:
    void apply(void) override;
    void configure(const QString& text);

private:
    class gnomonWorkspaceFusionPrivate *d;
};

//
// gnomonWorkspaceFusion.h ends here
