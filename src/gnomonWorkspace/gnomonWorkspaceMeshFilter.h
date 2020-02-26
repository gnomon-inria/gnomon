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

class GNOMONWORKSPACE_EXPORT gnomonWorkspaceMeshFilter : public dtkWidgetsWorkspace
{
    Q_OBJECT

public:
     gnomonWorkspaceMeshFilter(QWidget *parent = nullptr);
    ~gnomonWorkspaceMeshFilter(void);

public:
    void enter(void) override;
    void leave(void) override;

public slots:
    void apply(void) override;
    void configure(const QString& text);

public:
    static const QColor color;

public:
    static bool isEmpty(void);

private:
    class gnomonWorkspaceMeshFilterPrivate *d;
};

//
// gnomonWorkspaceMeshFilter.h ends here
