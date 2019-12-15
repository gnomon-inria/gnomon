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

class gnomonAbstractDynamicForm;
class gnomonAbstractCommand;
class gnomonAbstractVisualization;
class gnomonAbstractMatplotlibVisualization;

class gnomonFormManager;
class gnomonFormManagerItem;
class gnomonFormManagerFocus;
class gnomonFormManagerData;

class vtkCamera;

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
    gnomonFormManagerItem *create(gnomonAbstractDynamicForm *, const QColor&, const QImage& image);
    gnomonFormManagerItem *create(const QString&, const QColor&, const QImage& image);

public:
    QHash<gnomonFormManagerItem *, gnomonAbstractDynamicForm *> forms;
    QHash<gnomonFormManagerItem *, gnomonAbstractVisualization *> formVisualizations;
    QHash<gnomonFormManagerItem *, gnomonAbstractMatplotlibVisualization *> formMatplotlibVisualizations;
    QHash<gnomonFormManagerItem *, gnomonAbstractCommand *> formWriterCommand;
    QHash<gnomonFormManagerItem *, gnomonFormManagerData *> formData;
    QHash<gnomonFormManagerItem *, vtkCamera *> formCameras;

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
