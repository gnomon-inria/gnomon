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

#include <QtCore>
#include <QtWidgets>

class gnomonImageManager : public QFrame
{
    Q_OBJECT

public:
     gnomonImageManager(QWidget *parent = nullptr);
    ~gnomonImageManager(void);

private:
    class gnomonImageManagerPrivate *d;
};

//
// gnomonImageManager.h ends here
