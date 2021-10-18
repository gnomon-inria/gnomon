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

class gnomonFormManagerPrivate;
class gnomonItemButton;

class gnomonFormManagerItem : public QObject
{
    Q_OBJECT

public:
     gnomonFormManagerItem(const QColor&, const QPixmap& thumbnail, int n_times, gnomonFormManagerPrivate *parent);
    ~gnomonFormManagerItem(void);

// signals:
//     void clicked(void);
//     void destroy(void);
//     void save(void);

// protected:
//     void enterEvent(QEvent *);
//     void leaveEvent(QEvent *);
//     void mousePressEvent(QMouseEvent *);

public:
    int id;

public:
    gnomonFormManagerPrivate *parent;

// public:
//     gnomonItemButton *button_destroy;
//     gnomonItemButton *button_save;

public:
    QPixmap image;
    QPixmap thumbnail;
    QPixmap transparent_thumbnail;
};

//
// gnomonFormManagerItem.h ends here
