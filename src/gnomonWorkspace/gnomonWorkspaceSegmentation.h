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

class GNOMONWORKSPACE_EXPORT gnomonWorkspaceSegmentation : public dtkWidgetsWorkspace
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

private:
    class gnomonWorkspaceSegmentationPrivate *d;
};

//
// gnomonWorkspaceSegmentation.h ends here
