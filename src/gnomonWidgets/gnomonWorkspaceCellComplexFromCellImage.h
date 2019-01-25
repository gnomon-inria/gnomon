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

class GNOMONWIDGETS_EXPORT gnomonWorkspaceCellComplexFromCellImage : public gnomonWorkspace
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
