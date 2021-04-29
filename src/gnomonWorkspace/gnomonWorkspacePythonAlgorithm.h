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

class GNOMONWORKSPACE_EXPORT gnomonWorkspacePythonAlgorithm : public dtkWidgetsWorkspace
{
    Q_OBJECT

public:
     gnomonWorkspacePythonAlgorithm(QWidget *parent = nullptr);
    ~gnomonWorkspacePythonAlgorithm(void);

public:
    void enter(void) override;
    void leave(void) override;
    void apply(void) override;

public:
    void run(void);

public:
    static const QColor color;

protected:
    void resizeEvent(QResizeEvent *event) override;

public:
    static bool isEmpty(void);

private:
    class gnomonWorkspacePythonAlgorithmPrivate *d;
};

//
// gnomonWorkspacePythonAlgorithm.h ends here
