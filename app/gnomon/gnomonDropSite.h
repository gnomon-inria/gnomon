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

protected:
    void dragEnterEvent(QDragEnterEvent *);
    void dragLeaveEvent(QDragLeaveEvent *);
    void dragMoveEvent(QDragMoveEvent *);
    void dropEvent(QDropEvent *);

private:
    class gnomonDropSitePrivate *d;
};

//
// gnomonDropSite.h ends here
