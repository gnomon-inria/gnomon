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

class GNOMONWIDGETS_EXPORT gnomonWorkspaceMeshFromImage : public gnomonWorkspace
{
    Q_OBJECT

public:
     gnomonWorkspaceMeshFromImage(QWidget *parent = nullptr);
    ~gnomonWorkspaceMeshFromImage(void);

public:
    void enter(void) override {}
    void leave(void) override {}

public slots:
    void apply(void) override;
    void configure(const QString& text);

private:
    class gnomonWorkspaceMeshFromImagePrivate *d;
};

//
// gnomonWorkspaceMeshFromImage.h ends here
