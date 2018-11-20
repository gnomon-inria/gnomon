// Version: $Id: 80be09c7cc19c2fc912fa461752bfbc2a211d0d6 $
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
#include <QtGui>
#include <QtWidgets>

#include <gnomonVisualizationExport.h>

// /////////////////////////////////////////////////////////////////
// gnomonClutEditorVertex
// /////////////////////////////////////////////////////////////////

class GNOMONVISUALIZATION_EXPORT gnomonClutEditorVertex : public QObject, public QGraphicsItem
{
    Q_OBJECT

public:
     gnomonClutEditorVertex(int x, int y, QColor color = Qt::yellow, QGraphicsItem *parent = 0);
     gnomonClutEditorVertex(QPointF point, QColor color = Qt::yellow, QGraphicsItem *parent = 0);
    ~gnomonClutEditorVertex(void);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

    QRectF boundingRect(void) const;
    QPoint position(void) const;
    QColor color(void) const;

    void setColor(const QColor& color);

signals:
    void moved(const QPointF& position);

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

private:
    QColor fgColor;
    QColor bgColor;
};

// /////////////////////////////////////////////////////////////////
// gnomonClutEditorScene
// /////////////////////////////////////////////////////////////////

class gnomonClutEditorScenePrivate;

class GNOMONVISUALIZATION_EXPORT gnomonClutEditorScene : public QGraphicsScene
{
    Q_OBJECT

public:
     gnomonClutEditorScene(QObject *parent = 0);
    ~gnomonClutEditorScene(void);

    void addItem(QGraphicsItem *item);

signals:
    void   moved(const QPointF& position);
    void removed(void);

protected:
    void keyPressEvent(QKeyEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

private:
    gnomonClutEditorScenePrivate *d;
};

// /////////////////////////////////////////////////////////////////
// gnomonClutEditorView
// /////////////////////////////////////////////////////////////////

class gnomonClutEditorViewPrivate;

class GNOMONVISUALIZATION_EXPORT gnomonClutEditorView : public QGraphicsView
{
public:
     gnomonClutEditorView(QWidget *parent = 0);
    ~gnomonClutEditorView(void);

private:
    gnomonClutEditorViewPrivate *d;
};

// /////////////////////////////////////////////////////////////////
// gnomonClutEditor
// /////////////////////////////////////////////////////////////////

class vtkColorTransferFunction;
class vtkPiecewiseFunction;

class GNOMONVISUALIZATION_EXPORT gnomonClutEditor : public QWidget
{
    Q_OBJECT

public:
    enum ColorSpace {
        HSV,
        RGB
    };

public:
    typedef QList<int> Histogram;

public:
     gnomonClutEditor(QWidget *parent = 0);
    ~gnomonClutEditor(void);

    void   *colorTransferFunction(void);
    void *opacityTransferFunction(void);

    void setColorTransferFunction(vtkColorTransferFunction *color_transfer_function);
    void setOpacityTransferFunction(vtkPiecewiseFunction *opacity_transfer_function);

    void setRange(double min, double max);

    void setHistogram(const Histogram& histogram);

signals:
    void updated(void);

public slots:
    void importClut(void);
    void exportClut(void);

    void toggle(bool);

public:
    QSize sizeHint(void) const;

protected slots:
    void onApply(void);
    void onColorAuto(void);
    void onColorChoose(void);
    void onSelectionChanged(void);
    void onVertexMoved(const QPointF& point);

protected:
    void resizeEvent(QResizeEvent *event);

 private:
    void updateTable(void);

private:
    class gnomonClutEditorPrivate *d;
};

//
// gnomonClutEditor.h ends here
