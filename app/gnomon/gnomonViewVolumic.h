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

class dtkImage;

class gnomonViewVolumic : public QFrame
{
    Q_OBJECT

public:
     gnomonViewVolumic(QWidget *parent = nullptr);
    ~gnomonViewVolumic(void);

public:
    void setImage(dtkImage *);

public:
    dtkImage *image(void);

public slots:
    void onSliceChanged(int);

protected:
    void dragEnterEvent(QDragEnterEvent *);
    void dragLeaveEvent(QDragLeaveEvent *);
    void dragMoveEvent(QDragMoveEvent *);
    void dropEvent(QDropEvent *);

private:
    class gnomonViewVolumicPrivate *d;
};

//
// gnomonViewVolumic.h ends here
