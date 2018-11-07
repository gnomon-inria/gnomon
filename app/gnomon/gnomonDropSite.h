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

class gnomonDropSite : public QFrame
{
    Q_OBJECT

public:
     gnomonDropSite(QWidget *parent = nullptr);
    ~gnomonDropSite(void);

public:
    QSize sizeHint(void) const;

public:
    int index(void);

private:
    class gnomonDropSitePrivate *d;
};

//
// gnomonDropSite.h ends here
