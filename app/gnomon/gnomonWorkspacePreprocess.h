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

#include <QtWidgets>

class gnomonWorkspacePreprocess : public QWidget
{
    Q_OBJECT

public:
     gnomonWorkspacePreprocess(QWidget *parent = nullptr);
    ~gnomonWorkspacePreprocess(void);

public slots:
    void apply(void);
    void configure(const QString& text);

private:
    class gnomonWorkspacePreprocessPrivate *d;
};

//
// gnomonWorkspacePreprocess.h ends here
