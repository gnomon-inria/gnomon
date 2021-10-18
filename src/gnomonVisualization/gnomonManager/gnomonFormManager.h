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

#include <gnomonVisualizationExport>

#include <QtCore>
#include <QtGui>

class gnomonAbstractForm;
class gnomonAbstractDynamicForm;
class gnomonAbstractCommand;
class gnomonAbstractVisualization;
class gnomonAbstractMatplotlibVisualization;

class gnomonFormManagerItem;

class vtkCamera;

class GNOMONVISUALIZATION_EXPORT gnomonFormManager : public QObject
{
    Q_OBJECT

public:
    static gnomonFormManager *instance(void);

signals:
    void added(int id);
//     void shrink(void);
//     void expand(void);

// public:
//     QSize sizeHint(void) const;

public slots:
    void addForm(gnomonAbstractDynamicForm *, const QColor&, gnomonAbstractVisualization* visualization, vtkCamera *cam=0);
    void addForm(gnomonAbstractDynamicForm *, const QColor&, gnomonAbstractMatplotlibVisualization* visualization);
    void addForm(gnomonAbstractDynamicForm *, const QColor&, const QImage& image);

// public slots:
//     void present(gnomonFormManagerItem *, bool = false);

public:
    gnomonAbstractDynamicForm *get(int index);
    gnomonAbstractVisualization *getVisualization(int index);
    vtkCamera *getCamera(int index);

public:
    Q_INVOKABLE QPixmap thumbnail(int index);

protected:
     gnomonFormManager(QObject *parent = nullptr);
    ~gnomonFormManager(void);

// protected:
//     void enterEvent(QEvent *);
//     void leaveEvent(QEvent *);

// protected:
//     void mousePressEvent(QMouseEvent *);

// protected:
//     void paintEvent(QPaintEvent *);

private:
    class gnomonFormManagerPrivate *d;

private:
    static gnomonFormManager *s_instance;
};

//
// gnomonFormManager.h ends here
