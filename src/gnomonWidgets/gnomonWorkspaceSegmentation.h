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

class GNOMONWIDGETS_EXPORT gnomonWorkspaceSegmentation : public gnomonWorkspace
{
    Q_OBJECT

public:
     gnomonWorkspaceSegmentation(QWidget *parent = nullptr);
    ~gnomonWorkspaceSegmentation(void);

public:
    void enter(void) override {}
    void leave(void) override {}

public slots:
    void apply(void) override;

    void configure(const QString& text);
    void computeCells(void);

private:
    class gnomonWorkspaceSegmentationPrivate *d;
};

//
// gnomonWorkspaceSegmentation.h ends here
