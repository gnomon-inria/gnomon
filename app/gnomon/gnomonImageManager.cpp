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

#include "gnomonImageManager.h"

// ///////////////////////////////////////////////////////////////////
// gnomonImageManagerItem
// ///////////////////////////////////////////////////////////////////

class gnomonImageManagerItem : public QLabel
{
    Q_OBJECT

public:
     gnomonImageManagerItem(QWidget *parent = nullptr);
    ~gnomonImageManagerItem(void);
};

gnomonImageManagerItem::gnomonImageManagerItem(QWidget *parent) : QLabel(parent)
{

}

gnomonImageManagerItem::~gnomonImageManagerItem(void)
{

}

// ///////////////////////////////////////////////////////////////////
// gnomonImageManagerPrivate
// ///////////////////////////////////////////////////////////////////

class gnomonImageManagerPrivate : public QScrollArea
{
public:
     gnomonImageManagerPrivate(QWidget *parent = nullptr);
    ~gnomonImageManagerPrivate(void);
};

gnomonImageManagerPrivate::gnomonImageManagerPrivate(QWidget *parent) : QScrollArea(parent)
{

}

gnomonImageManagerPrivate::~gnomonImageManagerPrivate(void)
{

}

// ///////////////////////////////////////////////////////////////////
// gnomonImageManager
// ///////////////////////////////////////////////////////////////////

gnomonImageManager::gnomonImageManager(QWidget *parent) : QFrame(parent)
{
    d = new gnomonImageManagerPrivate;

    QHBoxLayout *layout = new QHBoxLayout(this);
}

gnomonImageManager::~gnomonImageManager(void)
{
    delete d;
}

// ///////////////////////////////////////////////////////////////////

#include "gnomonImageManager.moc"

//
// gnomonImageManager.cpp ends here
