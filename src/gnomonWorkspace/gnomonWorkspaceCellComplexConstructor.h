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

class GNOMONWORKSPACE_EXPORT gnomonWorkspaceCellComplexConstructor : public dtkWidgetsWorkspace
{
    Q_OBJECT

public:
     gnomonWorkspaceCellComplexConstructor(QWidget *parent = nullptr);
    ~gnomonWorkspaceCellComplexConstructor(void);

public:
    void enter(void) override;
    void leave(void) override;

public slots:
    void apply(void) override;
    void configure(const QString& text);

public:
    static const QColor color;

private:
    class gnomonWorkspaceCellComplexConstructorPrivate *d;
};

//
// gnomonWorkspaceCellComplexConstructor.h ends here
