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

#include <tissueVisualizationExport.h>

// /////////////////////////////////////////////////////////////////
// tissueClutEditorVertex
// /////////////////////////////////////////////////////////////////

class TISSUEVISUALIZATION_EXPORT tissueClutEditorVertex : public QObject, public QGraphicsItem
{
    Q_OBJECT

public:
     tissueClutEditorVertex(int x, int y, QColor color = Qt::yellow, QGraphicsItem *parent = 0);
     tissueClutEditorVertex(QPointF point, QColor color = Qt::yellow, QGraphicsItem *parent = 0);
    ~tissueClutEditorVertex(void);

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
// tissueClutEditorScene
// /////////////////////////////////////////////////////////////////

class tissueClutEditorScenePrivate;

class TISSUEVISUALIZATION_EXPORT tissueClutEditorScene : public QGraphicsScene
{
    Q_OBJECT

public:
     tissueClutEditorScene(QObject *parent = 0);
    ~tissueClutEditorScene(void);

    void addItem(QGraphicsItem *item);

signals:
    void   moved(const QPointF& position);
    void removed(void);

protected:
    void keyPressEvent(QKeyEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

private:
    tissueClutEditorScenePrivate *d;
};

// /////////////////////////////////////////////////////////////////
// tissueClutEditorView
// /////////////////////////////////////////////////////////////////

class tissueClutEditorViewPrivate;

class TISSUEVISUALIZATION_EXPORT tissueClutEditorView : public QGraphicsView
{
public:
     tissueClutEditorView(QWidget *parent = 0);
    ~tissueClutEditorView(void);

private:
    tissueClutEditorViewPrivate *d;
};

// /////////////////////////////////////////////////////////////////
// tissueClutEditor
// /////////////////////////////////////////////////////////////////

class TISSUEVISUALIZATION_EXPORT tissueClutEditor : public QWidget
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
     tissueClutEditor(QWidget *parent = 0);
    ~tissueClutEditor(void);

    void   *colorTransferFunction(void);
    void *opacityTransferFunction(void);

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
    class tissueClutEditorPrivate *d;
};

//
// tissueClutEditor.h ends here
