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

class gnomonImageManagerPrivate;
class gnomonItemButton;

class gnomonImageManagerItem : public QLabel
{
    Q_OBJECT

public:
     gnomonImageManagerItem(const QColor&, const QPixmap& thumbnail, gnomonImageManagerPrivate *parent);
    ~gnomonImageManagerItem(void);

signals:
    void clicked(void);
    void destroy(void);
    void save(void);

protected:
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    void mousePressEvent(QMouseEvent *);

public:
    int id;

public:
    gnomonImageManagerPrivate *parent;

public:
    gnomonItemButton *button_destroy;
    gnomonItemButton *button_save;

public:
    QPixmap thumbnail;
    QPixmap transparent_thumbnail;
};

//
// gnomonImageManagerItem.h ends here
