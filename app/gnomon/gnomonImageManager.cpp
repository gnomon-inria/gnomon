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

public:
    gnomonImageManagerItem *create(gnomonImageManager::Image);

public:
    QHash<gnomonImageManagerItem *, gnomonImageManager::Image> images;
};

gnomonImageManagerPrivate::gnomonImageManagerPrivate(QWidget *parent) : QScrollArea(parent)
{

}

gnomonImageManagerPrivate::~gnomonImageManagerPrivate(void)
{

}

gnomonImageManagerItem *gnomonImageManagerPrivate::create(gnomonImageManager::Image image)
{
    Q_UNUSED(image);

    return new gnomonImageManagerItem(this);
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

void gnomonImageManager::addImage(gnomonImageManager::Image image)
{
    d->images.insert(d->create(image), image);
}

// ///////////////////////////////////////////////////////////////////

#include "gnomonImageManager.moc"

//
// gnomonImageManager.cpp ends here
