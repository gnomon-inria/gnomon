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

class GNOMONWORKSPACE_EXPORT gnomonWorkspaceImageConstructor : public dtkWidgetsWorkspace
{
    Q_OBJECT

public:
     gnomonWorkspaceImageConstructor(QWidget *parent = nullptr);
    ~gnomonWorkspaceImageConstructor(void);

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
    class gnomonWorkspaceImageConstructorPrivate *d;
};

//
// gnomonWorkspaceImageConstructor.h ends here
