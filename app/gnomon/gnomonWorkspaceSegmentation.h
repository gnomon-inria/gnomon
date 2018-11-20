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

#include "gnomonWorkspace.h"

#include <QtWidgets>

class gnomonWorkspaceSegmentation : public gnomonWorkspace
{
    Q_OBJECT

public:
     gnomonWorkspaceSegmentation(QWidget *parent = nullptr);
    ~gnomonWorkspaceSegmentation(void);

public:
    void enter(void) override {}
    void leave(void) override {}

public slots:
    void apply(void);
    void configure(const QString& text);

private:
    class gnomonWorkspaceSegmentationPrivate *d;
};

//
// gnomonWorkspaceSegmentation.h ends here
