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

#include <dtkWidgets>

class GNOMONWORKSPACE_EXPORT gnomonWorkspacePlantScan3D : public dtkWidgetsWorkspace
{
    Q_OBJECT

public:
     gnomonWorkspacePlantScan3D(QWidget *parent = nullptr);
    ~gnomonWorkspacePlantScan3D(void);

public:
    void enter(void) override;
    void leave(void) override;

public:
    void apply(void) override;

public slots:
    void step(void);
    void reset(void);

public:
    void fill(QWidget *);

public:
    static const QColor color;

private:
    class gnomonWorkspacePlantScan3DPrivate *d;
};

//
// gnomonWorkspacePlantScan3D.h ends here
