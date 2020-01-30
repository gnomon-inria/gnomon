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

class GNOMONWORKSPACE_EXPORT gnomonWorkspaceCellImageConstructor : public dtkWidgetsWorkspace
{
    Q_OBJECT

public:
     gnomonWorkspaceCellImageConstructor(QWidget *parent = nullptr);
    ~gnomonWorkspaceCellImageConstructor(void);

public:
    void enter(void) override;
    void leave(void) override;

public slots:
    void apply(void) override;
    void configure(const QString& text);

public:
    static const QColor color;

private:
    class gnomonWorkspaceCellImageConstructorPrivate *d;
};

//
// gnomonWorkspaceCellImageConstructor.h ends here
