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

class GNOMONWORKSPACE_EXPORT gnomonWorkspacePointCloudFromImage : public dtkWidgetsWorkspace
{
    Q_OBJECT

public:
     gnomonWorkspacePointCloudFromImage(QWidget *parent = nullptr);
    ~gnomonWorkspacePointCloudFromImage(void);

public:
    void enter(void) override {}
    void leave(void) override {}

public slots:
    void apply(void) override;
    void configure(const QString& text);

private:
    class gnomonWorkspacePointCloudFromImagePrivate *d;
};

//
// gnomonWorkspacePointCloudFromImage.h ends here
