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

class gnomonWorkspaceSegmentation : public QWidget
{
    Q_OBJECT

public:
     gnomonWorkspaceSegmentation(QWidget *parent = nullptr);
    ~gnomonWorkspaceSegmentation(void);

public slots:
    void apply(void);

private:
    class gnomonWorkspaceSegmentationPrivate *d;
};

//
// gnomonWorkspaceSegmentation.h ends here
