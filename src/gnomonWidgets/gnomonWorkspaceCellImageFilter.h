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

class GNOMONWIDGETS_EXPORT gnomonWorkspaceCellImageFilter : public gnomonWorkspace
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
