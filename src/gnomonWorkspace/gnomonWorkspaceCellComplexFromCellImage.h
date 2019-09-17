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

class GNOMONWORKSPACE_EXPORT gnomonWorkspaceCellComplexFromCellImage : public dtkWidgetsWorkspace
{
    Q_OBJECT

public:
     gnomonWorkspaceCellComplexFromCellImage(QWidget *parent = nullptr);
    ~gnomonWorkspaceCellComplexFromCellImage(void);

public:
    void enter(void) override {}
    void leave(void) override {}

public slots:
    void apply(void) override;
    void configure(const QString& text);

private:
    class gnomonWorkspaceCellComplexFromCellImagePrivate *d;
};

//
// gnomonWorkspaceCellComplexFromCellImage.h ends here
