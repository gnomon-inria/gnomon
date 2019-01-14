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


class gnomonAbstractForm;
class gnomonAbstractCommand;
class gnomonAbstractVisualization;

class gnomonFormManager;
class gnomonFormManagerItem;
class gnomonFormManagerFocus;

class gnomonFormManagerPrivate : public QScrollArea
{
public:
    enum State {
        Collapsed,
        Expanded
    };

public:
     gnomonFormManagerPrivate(QWidget *parent = nullptr);
    ~gnomonFormManagerPrivate(void);

public:
    QSize sizeHint(void) const;

public:
    gnomonFormManagerItem *create(gnomonAbstractForm *, const QColor&, const QImage& image);

public:
    QHash<gnomonFormManagerItem *, gnomonAbstractForm *> forms;
    QHash<gnomonFormManagerItem *, gnomonAbstractVisualization *> formVisualizations;
    QHash<gnomonFormManagerItem *, gnomonAbstractCommand *> formWriterCommand;

public:
    gnomonFormManager *q;

public:
    gnomonFormManagerFocus *focus_item = nullptr;

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
// gnomonFormManager_p.h ends here
