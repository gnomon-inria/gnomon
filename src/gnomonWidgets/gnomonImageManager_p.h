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

#include <dtkImagingCore>

class gnomonImagesSerie;
using gnomonImagesSeriePtr = QSharedPointer<gnomonImagesSerie>;

class gnomonAbstractImagesSerieWriter;

class gnomonImageManager;
class gnomonImageManagerData;
class gnomonImageManagerItem;
class gnomonImageManagerFocus;

class gnomonImageManagerPrivate : public QScrollArea
{
public:
    enum State {
        Collapsed,
        Expanded
    };

public:
     gnomonImageManagerPrivate(QWidget *parent = nullptr);
    ~gnomonImageManagerPrivate(void);

public:
    QSize sizeHint(void) const;

public:
    gnomonImageManagerItem *create(gnomonImagesSeriePtr, const QColor&);

public:
    QHash<gnomonImageManagerItem *, gnomonImagesSeriePtr> images_series;
    QHash<gnomonImageManagerItem *, gnomonImageManagerData *> data;

public:
    gnomonAbstractImagesSerieWriter *writer;

public:
    gnomonImageManager *q;

public:
    gnomonImageManagerFocus *focus_item = nullptr;

public:
    QScrollArea *focus_area = nullptr;

public:
    static int item_counter;

public:
    bool inside = false;

public:
    QWidget *contents;

public:
    State state = Collapsed;
};

//
// gnomonImageManager_p.h ends here
