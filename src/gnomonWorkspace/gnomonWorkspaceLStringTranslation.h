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

class GNOMONWORKSPACE_EXPORT gnomonWorkspaceLStringTranslation : public dtkWidgetsWorkspace
{
    Q_OBJECT

public:
     gnomonWorkspaceLStringTranslation(QWidget *parent = nullptr);
    ~gnomonWorkspaceLStringTranslation(void);

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
    class gnomonWorkspaceLStringTranslationPrivate *d;
};

//
// gnomonWorkspaceLStringTranslation.h ends here
