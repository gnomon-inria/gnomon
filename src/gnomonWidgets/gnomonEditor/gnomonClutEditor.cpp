// Version: $Id: b3634632a3e3eac8bcc4a3b15a61a881bb42aa64 $
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#include "gnomonClutEditor.h"

#include <dtkThemes>

#include <QtXml>

#include <vtkColorTransferFunction.h>
#include <vtkLookupTable.h>
#include <vtkMapper.h>
#include <vtkPiecewiseFunction.h>
#include <vtkScalarsToColors.h>
#include <vtkGPUVolumeRayCastMapper.h>
#include <vtkVolume.h>
#include <vtkVolumeProperty.h>

// /////////////////////////////////////////////////////////////////
// gnomonClutEditor - Helper functions
// /////////////////////////////////////////////////////////////////

QPointF gnomonClutEditorMap   (QPointF vertex, qreal min, qreal max, int width, int height, bool logScale);
QPointF gnomonClutEditorMapInv(QPointF vertex, qreal min, qreal max, int width, int height, bool logScale);

// ///////////////////////////////////////////////////////////////////
// gnomonClutEditorInterpolator
// ///////////////////////////////////////////////////////////////////

class gnomonClutEditorInterpolator
{
public:
    virtual ~gnomonClutEditorInterpolator(void) {}

public:
    void setStart(gnomonClutEditorVertex *vertex);
    void  setStop(gnomonClutEditorVertex *vertex);

    virtual QColor interpolate(gnomonClutEditorVertex *vertex) const = 0;

protected:
    gnomonClutEditorVertex *start;
    gnomonClutEditorVertex *stop;
};

void gnomonClutEditorInterpolator::setStart(gnomonClutEditorVertex *vertex)
{
    this->start = vertex;
}

void gnomonClutEditorInterpolator::setStop(gnomonClutEditorVertex *vertex)
{
    this->stop = vertex;
}

// /////////////////////////////////////////////////////////////////
// gnomonClutEditorInterpolatorHSV
// /////////////////////////////////////////////////////////////////

class gnomonClutEditorInterpolatorHSV : public gnomonClutEditorInterpolator
{
public:
    QColor interpolate(gnomonClutEditorVertex *vertex) const;

private:
    static qreal computeHue(double x, double x0, qreal h0, double x1, qreal h1);
};

qreal gnomonClutEditorInterpolatorHSV::computeHue(double x, double x0, qreal h0, double x1, qreal h1)
{
    if (x1 == x0)
        return (h0 + h1)/2.0;

    double alpha = (x-x0)/(x1-x0);

    return h1*alpha + h0*(1.0-alpha);
}

QColor gnomonClutEditorInterpolatorHSV::interpolate(gnomonClutEditorVertex *vertex) const
{
    if(!this->start) {
        qDebug() << "No start value set for interpolation";
        return vertex->color();
    }

    if(!this->stop) {
        qDebug() << "No stop value set for interpolation";
        return vertex->color();
    }

    qreal x    = vertex->scenePos().x();
    qreal xmin = start->scenePos().x();
    qreal xmax = stop->scenePos().x();

    qreal hmin = start->color().hueF();
    qreal hmax = stop->color().hueF();

    qreal h = this->computeHue(x, xmin, hmin, xmax, hmax);
    qreal s = vertex->color().saturationF();
    qreal v = vertex->color().valueF();

    return QColor::fromHsvF(h, s, v);
}

// /////////////////////////////////////////////////////////////////
// gnomonClutEditorInterpolatorRGB
// /////////////////////////////////////////////////////////////////

class gnomonClutEditorInterpolatorRGB : public gnomonClutEditorInterpolator
{
public:
    QColor interpolate(gnomonClutEditorVertex *vertex) const;
};

QColor gnomonClutEditorInterpolatorRGB::interpolate(gnomonClutEditorVertex *vertex) const
{
    if(!this->start) {
        qDebug() << Q_FUNC_INFO << "No start value set for interpolation";
        return vertex->color();
    }

    if(!this->stop) {
        qDebug() << Q_FUNC_INFO << "No stop value set for interpolation";
        return vertex->color();
    }

    QPropertyAnimation interpolator;
    interpolator.setEasingCurve(QEasingCurve::Linear);
    interpolator.setDuration(100);
    interpolator.setKeyValueAt(0.0, this->start->color());
    interpolator.setKeyValueAt(1.0, this->stop->color());
    interpolator.setCurrentTime(50);

    return interpolator.currentValue().value<QColor>();
}

// ///////////////////////////////////////////////////////////////////
// gnomonClutEditorHistogram
// ///////////////////////////////////////////////////////////////////

class gnomonClutEditorHistogram: public QGraphicsItem
{
public:
     gnomonClutEditorHistogram(QGraphicsItem *parent = Q_NULLPTR);
    ~gnomonClutEditorHistogram();

public:
    void setup(const gnomonClutEditor::Histogram& histogram);

public:
    QRectF boundingRect(void) const ;

public:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR);

public:
    QList<double> normalizedHistogram;
};

gnomonClutEditorHistogram::gnomonClutEditorHistogram(QGraphicsItem *parent) : QGraphicsItem(parent)
{

}

gnomonClutEditorHistogram::~gnomonClutEditorHistogram()
{

}

void gnomonClutEditorHistogram::setup(const gnomonClutEditor::Histogram &histogram)
{
    static const double epsilon = 1e-6;

    if (this->normalizedHistogram.count() != 0)
        this->normalizedHistogram.clear();

    double max = 0;

    for(int i = 0; i < histogram.count(); ++i) {
        if (histogram.at(i) < 0) {
            qDebug() << Q_FUNC_INFO << "bad value at:" << i;
            this->normalizedHistogram.append(0.0);
        }
        else {
            max = qMax(max, (double)histogram.at(i));
            this->normalizedHistogram.append((double)histogram.at(i));
        }
    }

    if (max < epsilon) {
        qDebug() << Q_FUNC_INFO << "histogram is null" ;
        return;
    }

    for(int i = 0; i < this->normalizedHistogram.count() ; ++i) {
        this->normalizedHistogram.replace(i, this->normalizedHistogram.at(i) / max);
    }
}

QRectF gnomonClutEditorHistogram::boundingRect(void) const
{
    return this->parentItem()->boundingRect();
}

void gnomonClutEditorHistogram::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if (this->normalizedHistogram.isEmpty())
        return;

    if (this->boundingRect().isEmpty())
        return;

    painter->setPen(Qt::darkGray);
    painter->setBrush(Qt::lightGray);

    qreal p_w = this->boundingRect().width();
    qreal p_h = this->boundingRect().height();
    qreal b_w = p_w / this->normalizedHistogram.count();

    for(int i = 0 ; i < this->normalizedHistogram.count() ; ++i) {
        qreal v = this->normalizedHistogram.at(i) * 0.95 ;
        qreal x = i * b_w;
        qreal y = ( 1.0 - v ) * p_h;
        qreal w = b_w;
        qreal h = v * p_h ;

        painter->drawRect(x, y, w, h);
    }
}

// /////////////////////////////////////////////////////////////////
// gnomonClutEditorVertex
// /////////////////////////////////////////////////////////////////

gnomonClutEditorVertex::gnomonClutEditorVertex(int x, int y, QColor color, QGraphicsItem *parent) : QGraphicsItem(parent)
{
    this->fgColor = color;
    this->bgColor = QColor(0xc0, 0xc0, 0xc0);

    this->setPos(x, -y);
    this->setZValue(1);

    this->setFlag(QGraphicsItem::ItemIsMovable, true);
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
}

gnomonClutEditorVertex::gnomonClutEditorVertex(QPointF point, QColor color, QGraphicsItem *parent) : QGraphicsItem(parent)
{
    this->fgColor = color;
    this->bgColor = QColor(0xc0, 0xc0, 0xc0);

    this->setPos(point.x(), -point.y());
    this->setZValue(2);

    this->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
}

gnomonClutEditorVertex::~gnomonClutEditorVertex(void)
{

}

void gnomonClutEditorVertex::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if(this->isSelected())
        painter->setPen(Qt::magenta);
    else
        painter->setPen(Qt::black);

    painter->setBrush(bgColor); painter->drawEllipse(-10, -10, 20, 20);
    painter->setBrush(fgColor); painter->drawEllipse(-5, -5, 10, 10);
}

QRectF gnomonClutEditorVertex::boundingRect(void) const
{
    return QRectF(-10, -10, 20, 20);
}

QPoint gnomonClutEditorVertex::position(void) const
{
    return QPoint(this->x(), this->y());
}

QColor gnomonClutEditorVertex::color(void) const
{
    return fgColor;
}

void gnomonClutEditorVertex::setColor(const QColor& color)
{
    fgColor = color;

    this->update();
}

void gnomonClutEditorVertex::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseMoveEvent(event);

    emit moved(event->scenePos());
}

// /////////////////////////////////////////////////////////////////
// gnomonClutEditorTable - helper functions
// /////////////////////////////////////////////////////////////////

static bool gnomonClutEditorVertexLessThan(const gnomonClutEditorVertex *v1, const gnomonClutEditorVertex *v2) {
    return (v1->position().x() < v2->position().x());
}

// /////////////////////////////////////////////////////////////////
// gnomonClutEditorTable
// /////////////////////////////////////////////////////////////////

class gnomonClutEditorTable : public QGraphicsItem
{
public:
     gnomonClutEditorTable(QGraphicsItem *parent = 0);
    ~gnomonClutEditorTable(void);

    void addVertex(gnomonClutEditorVertex *vertex);
    void removeVertex(gnomonClutEditorVertex *vertex);

    void sort(void);

public:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

public:
    QRectF boundingRect(void) const;

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

public:
    QList<gnomonClutEditorVertex *> vertices;
};

gnomonClutEditorTable::gnomonClutEditorTable(QGraphicsItem *parent) : QGraphicsItem(parent)
{
    this->setFlag(QGraphicsItem::ItemIsMovable, true);
    this->setZValue(1);
}

gnomonClutEditorTable::~gnomonClutEditorTable(void)
{

}

void gnomonClutEditorTable::addVertex(gnomonClutEditorVertex *vertex)
{
    vertices << vertex;

    vertex->setParentItem(this);

    this->sort();
}

void gnomonClutEditorTable::removeVertex(gnomonClutEditorVertex *vertex)
{
    vertices.removeAll(vertex);

    this->sort();
}

void gnomonClutEditorTable::sort(void)
{
    qSort(vertices.begin(), vertices.end(), gnomonClutEditorVertexLessThan);
}

void gnomonClutEditorTable::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    int n_points = vertices.size()+4;

    QPoint *points = new QPoint[n_points];
    points[0] = this->mapFromScene(QPoint(0, 0)).toPoint();
    points[1] = this->mapFromScene(QPoint(0, vertices.first()->position().y())).toPoint();
    for(int i = 0 ; i < vertices.count() ; i++)
        points[i+2] = vertices.at(i)->position();
    points[vertices.count()+2] = this->mapFromScene(QPoint(this->scene()->sceneRect().width(), vertices.last()->position().y())).toPoint();
    points[vertices.count()+3] = this->mapFromScene(QPoint(this->scene()->sceneRect().width(), 0)).toPoint();

    QPen pen(Qt::black, 1);

    int xmin = vertices.first()->pos().x();
    int xmax = vertices.last()->pos().x();

    QLinearGradient linearGradient(xmin, 0, xmax, 0);
    {
        QColor color = vertices.first()->color(); color.setAlpha(128);
        linearGradient.setColorAt(0.0, color);
    }

    for(gnomonClutEditorVertex *vertex : vertices) {
        qreal position;
        position = (vertex->pos().x()-xmin)/(xmax-xmin);
        position = qMax(0.0, position);
        position = qMin(position, 1.0);

        QColor color = vertex->color(); color.setAlpha(128);

        linearGradient.setColorAt(position, color);
    }

    {
        QColor color = vertices.last()->color(); color.setAlpha(128);
        linearGradient.setColorAt(1.0, color);
    }

    painter->setPen(pen);
    painter->setBrush(linearGradient);
    painter->setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter->drawPolygon(points, n_points);

    delete[] points;
}

QRectF gnomonClutEditorTable::boundingRect(void) const
{
    int xmin, xmax, ymin, ymax;
    xmin = ymin = INT_MAX;
    xmax = ymax = INT_MIN;

    for(gnomonClutEditorVertex *vertex : vertices) {
        if(vertex->x() < xmin) xmin = vertex->x();
        if(vertex->x() > xmax) xmax = vertex->x();
        if(vertex->y() < ymin) ymin = vertex->y();
        if(vertex->y() > ymax) ymax = vertex->y();
    }

    return QRectF(xmin, ymin, xmax-xmin, qAbs(ymin));
}

void gnomonClutEditorTable::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QVector<QPointF> points;

    points << QPointF(vertices.first()->position().x(), 0);
    for(int i = 0 ; i < vertices.count() ; i++)
        points << QPointF(vertices.at(i)->position());
    points << QPointF(vertices.last()->position().x(), 0);

    QPolygonF polygon = QPolygonF(points);

    if(!polygon.containsPoint(event->pos(), Qt::OddEvenFill)) {
        event->ignore();
        return;
    }

    QPointF delta = event->pos() - event->lastPos();

    if(mapRectToScene(this->boundingRect()).left() + delta.x() < 0) {
        event->ignore();
        return;
    }

    if(mapRectToScene(this->boundingRect()).right() + delta.x() > this->scene()->views().first()->width()) {
        event->ignore();
        return;
    }

    this->setPos(QPointF(this->pos().x() + delta.x(), this->pos().y()));

    event->accept();
}

void gnomonClutEditorTable::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);

    this->scene()->clearSelection();
}

// /////////////////////////////////////////////////////////////////
// gnomonClutEditorScene
// /////////////////////////////////////////////////////////////////

class gnomonClutEditorScenePrivate
{
public:
    gnomonClutEditorTable *table;
};

gnomonClutEditorScene::gnomonClutEditorScene(QObject *parent) : QGraphicsScene(parent), d(new gnomonClutEditorScenePrivate)
{
}

gnomonClutEditorScene::~gnomonClutEditorScene(void)
{
    delete d;

    d = NULL;
}

void gnomonClutEditorScene::addItem(QGraphicsItem *item)
{
    if(gnomonClutEditorTable *table = dynamic_cast<gnomonClutEditorTable *>(item))
        d->table = table;

    QGraphicsScene::addItem(item);
}

void gnomonClutEditorScene::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Backspace || event->key() == Qt::Key_Delete) {
        for(QGraphicsItem *item : this->selectedItems()) {
            if(gnomonClutEditorVertex *vertex = dynamic_cast<gnomonClutEditorVertex *>(item)) {
                d->table->removeVertex(vertex);
                d->table->update();
                this->removeItem(vertex);
                delete vertex;

                emit removed();
            }
        }
    }
}

void gnomonClutEditorScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if(!this->sceneRect().adjusted(0.0, 50.0, 0.0, 0.0).contains(event->scenePos()))
        return;

    if(QGraphicsItem *item = this->itemAt(event->scenePos(),QTransform()))
        if(dynamic_cast<gnomonClutEditorVertex *>(item))
            return;

    if (d->table) {

        gnomonClutEditorVertex *vertex = new gnomonClutEditorVertex(d->table->mapFromScene(QPointF(event->scenePos().x(), -event->scenePos().y())));

        d->table->addVertex(vertex);
        d->table->update();

        vertex->setSelected(true);

        connect(vertex, SIGNAL(moved(const QPointF&)), this, SIGNAL(moved(const QPointF&)));
    }
}

void gnomonClutEditorScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(!this->sceneRect().adjusted(0.0, 50.0, 0.0, 0.0).contains(event->scenePos()))
        return;

    QPointF delta = event->scenePos() - event->lastScenePos();

    for(QGraphicsItem *item : this->selectedItems()) {

        if(gnomonClutEditorVertex *vertex = dynamic_cast<gnomonClutEditorVertex *>(item)) {

            if((vertex->scenePos().x() + delta.x() < 0) && (delta.x() <= 0)) {
                event->ignore();
                return;
            }

            if((vertex->scenePos().x() + delta.x() > this->views().first()->width()) && (delta.x() >= 0)) {
                event->ignore();
                return;
            }

            if((vertex->scenePos().y() + delta.y() <= -100) && (delta.y() <= 0)) {
                event->ignore();
                return;
            }

            if((vertex->scenePos().y() + delta.y() >= 0) && (delta.y() >= 0)) {
                event->ignore();
                return;
            }
        }
    }

    d->table->sort();
    d->table->update();

    QGraphicsScene::mouseMoveEvent(event);
}

// /////////////////////////////////////////////////////////////////
// gnomonClutEditorView
// /////////////////////////////////////////////////////////////////

class gnomonClutEditorViewPrivate
{
public:
};

gnomonClutEditorView::gnomonClutEditorView(QWidget *parent) : QGraphicsView(parent), d(new gnomonClutEditorViewPrivate)
{
    this->setAttribute(Qt::WA_MacShowFocusRect, false);
    this->setDragMode(QGraphicsView::RubberBandDrag);
    this->setFrameShape(QFrame::NoFrame);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);
    this->setViewportUpdateMode(FullViewportUpdate);
}

gnomonClutEditorView::~gnomonClutEditorView(void)
{
    delete d;

    d = NULL;
}

// /////////////////////////////////////////////////////////////////
// gnomonClutEditor
// /////////////////////////////////////////////////////////////////

class gnomonClutEditorPrivate
{
public:
    gnomonClutEditorScene *scene;
    gnomonClutEditorView *view;

public:
    double min;
    double max;

public:
    gnomonClutEditorTable *table;

public:
    gnomonClutEditorHistogram *histogram;

public:
    QGraphicsWidget *header;

public:
    QGraphicsRectItem *bg;

public:
    vtkColorTransferFunction *colorTransferFunction;
    vtkPiecewiseFunction *opacityTransferFunction;

public:
    gnomonClutEditor::ColorSpace color_space;

public:
    QCheckBox *button_log;
    QCheckBox *button_apply;

    QLineEdit *label_alpha;
    QLineEdit *label_value;

    QPushButton *button_auto;
    QPushButton *button_color;
    QPushButton *button_export;
    QPushButton *button_import;
};

gnomonClutEditor::gnomonClutEditor(QWidget *parent) : QWidget(parent), d(new gnomonClutEditorPrivate)
{
    this->setMinimumSize(QSize(800, 145));

    d->min = 100.0;
    d->max = 200.0;

    d->color_space = gnomonClutEditor::RGB;

    d->colorTransferFunction = Q_NULLPTR;
    d->opacityTransferFunction = Q_NULLPTR;

    d->histogram = Q_NULLPTR;

    d->bg = new QGraphicsRectItem;
    d->bg->setZValue(0);
    d->bg->setPen(QColor("#3b3b3b"));
    d->bg->setBrush(QColor(0, 0, 0, 128));

    d->table = new gnomonClutEditorTable;

    d->scene = new gnomonClutEditorScene(this);
    d->scene->addItem(d->bg);
    d->scene->addItem(d->table);

    d->view = new gnomonClutEditorView(this);
    d->view->setScene(d->scene);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(d->view);

    this->setFixedHeight(145);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    // --

    QPalette palette ;
    palette.setBrush(QPalette::Background, Qt::transparent);

    d->button_log = new QCheckBox("Logarithmic scale");
    d->button_log->setPalette(palette);
    d->button_log->setAutoFillBackground(false);

    d->button_apply = new QCheckBox("Applied");
    d->button_apply->setPalette(palette);
    d->button_apply->setAutoFillBackground(false);

    d->label_value = new QLineEdit("v:");
    d->label_alpha = new QLineEdit("α:");

    d->button_auto = new QPushButton("Interpolate");
    d->button_auto->setPalette(palette);
    d->button_auto->setAutoFillBackground(false);

    d->button_color = new QPushButton("Color");
    d->button_color->setPalette(palette);
    d->button_color->setAutoFillBackground(false);

    d->button_export = new QPushButton("Export");
    d->button_export->setPalette(palette);
    d->button_export->setAutoFillBackground(false);

    d->button_import = new QPushButton("Import");
    d->button_import->setPalette(palette);
    d->button_import->setAutoFillBackground(false);

    // --

    d->header = new QGraphicsWidget;

    QGraphicsProxyWidget *button_log_proxy = new QGraphicsProxyWidget;
    button_log_proxy->setWidget(d->button_log);

    QGraphicsProxyWidget *button_apply_proxy = new QGraphicsProxyWidget;
    button_apply_proxy->setWidget(d->button_apply);

    QGraphicsProxyWidget *label_value_proxy = new QGraphicsProxyWidget;
    label_value_proxy->setWidget(d->label_value);

    QGraphicsProxyWidget *label_alpha_proxy = new QGraphicsProxyWidget;
    label_alpha_proxy->setWidget(d->label_alpha);

    QGraphicsProxyWidget *button_auto_proxy = new QGraphicsProxyWidget;
    button_auto_proxy->setWidget(d->button_auto);

    QGraphicsProxyWidget *button_color_proxy = new QGraphicsProxyWidget;
    button_color_proxy->setWidget(d->button_color);

    QGraphicsProxyWidget *button_export_proxy = new QGraphicsProxyWidget;
    button_export_proxy->setWidget(d->button_export);

    QGraphicsProxyWidget *button_import_proxy = new QGraphicsProxyWidget;
    button_import_proxy->setWidget(d->button_import);

    // --

    QGraphicsLinearLayout *h_layout = new QGraphicsLinearLayout;
    h_layout->addItem(button_log_proxy);
    h_layout->addStretch(1);
    h_layout->addItem(button_auto_proxy);
    h_layout->addItem(button_color_proxy);
    h_layout->addItem(button_import_proxy);
    h_layout->addItem(button_export_proxy);
    h_layout->addItem(label_value_proxy);
    h_layout->addItem(label_alpha_proxy);
    h_layout->addItem(button_apply_proxy);

    d->header->setLayout(h_layout);

    d->scene->addItem(d->header);

    // --

    connect(d->scene, SIGNAL(moved(const QPointF&)), this, SLOT(onVertexMoved(const QPointF&)));
    connect(d->scene, SIGNAL(removed()), this, SLOT(onApply()));
    connect(d->scene, SIGNAL(selectionChanged()), this, SLOT(onSelectionChanged()));

    connect(d->button_auto, SIGNAL(clicked()), this, SLOT(onColorAuto()));
    connect(d->button_color, SIGNAL(clicked()), this, SLOT(onColorChoose()));
    connect(d->button_import, SIGNAL(clicked()), this, SLOT(importClut()));
    connect(d->button_export, SIGNAL(clicked()), this, SLOT(exportClut()));
    connect(d->button_apply, SIGNAL(clicked()), this, SLOT(onApply()));
    connect(d->button_apply, SIGNAL(toggled(bool)), this, SLOT(onApply()));
    connect(d->button_log, SIGNAL(toggled(bool)), this, SLOT(onApply()));
}

gnomonClutEditor::~gnomonClutEditor(void)
{
    delete d;

    d = NULL;
}

void *gnomonClutEditor::colorTransferFunction(void)
{
    return d->colorTransferFunction;
}

void gnomonClutEditor::setColorTransferFunction(vtkColorTransferFunction *color_transfer_function)
{
    d->colorTransferFunction = color_transfer_function;
    if(!d->opacityTransferFunction)
        return;

    this->updateTable();
}

void *gnomonClutEditor::opacityTransferFunction(void)
{
    return d->opacityTransferFunction;
}

void gnomonClutEditor::setOpacityTransferFunction(vtkPiecewiseFunction *opacity_transfer_function)
{
    d->opacityTransferFunction = opacity_transfer_function;
    this->updateTable();
}

void gnomonClutEditor::updateTable(void) {
    if(!d->colorTransferFunction || !d->opacityTransferFunction)
        return;
    for(gnomonClutEditorVertex *vertex : d->table->vertices) {
        d->table->removeVertex(vertex);
        d->scene->removeItem(vertex);
        delete vertex;
    }
    int size = d->colorTransferFunction->GetSize();
    double *vtk_vertex = new double[2];
    double *vtk_color = new double[3];
    double alpha = 0;
    for(int i = 0; i < size; ++i) {
        d->colorTransferFunction->GetNodeValue(i, &vtk_vertex[0]);
        d->colorTransferFunction->GetColor(vtk_vertex[0], &vtk_color[0]);
        alpha = d->opacityTransferFunction->GetValue(vtk_vertex[0]);
        gnomonClutEditorVertex *new_vertex = new gnomonClutEditorVertex(gnomonClutEditorMap(QPointF(vtk_vertex[0], alpha), d->min, d->max, this->width(), 100, d->button_log->isChecked()), QColor(vtk_color[0] *255, vtk_color[1] * 255, vtk_color[2] * 255));
        d->table->addVertex(new_vertex);
        connect(new_vertex, SIGNAL(moved(const QPointF&)), d->scene, SIGNAL(moved(const QPointF&)));
    }
}
void gnomonClutEditor::setRange(double min, double max)
{
    d->min = min;
    d->max = max;
}

void gnomonClutEditor::setHistogram(const Histogram& histogram)
{
    if(!d->histogram)
        d->histogram = new gnomonClutEditorHistogram(d->bg);

    d->histogram->setup(histogram);
}

void gnomonClutEditor::importClut(void)
{
    QString fileName = QFileDialog::getOpenFileName(this, "Import color lookup table");

    QDomDocument doc("clut");

    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly))
        return;

    if (!doc.setContent(&file)) {
        file.close();
        return;
    }

    file.close();

    for(gnomonClutEditorVertex *vertex : d->table->vertices) {
        d->table->removeVertex(vertex);
        d->scene->removeItem(vertex);
        delete vertex;
    }

    d->table->update();

    QDomElement root = doc.documentElement();
    double min = root.attribute("min").toDouble();
    double max = root.attribute("max").toDouble();
    int    log = root.attribute("log").toInt();
    this->setRange(min, max);

    QDomNode n = root.firstChild();
    while(!n.isNull()) {
        QDomElement e = n.toElement();
        if(!e.isNull()) {

            qreal v = e.attribute("v").toDouble();
            qreal a = e.attribute("a").toDouble();
            int r = e.attribute("r").toInt();
            int g = e.attribute("g").toInt();
            int b = e.attribute("b").toInt();

            gnomonClutEditorVertex *vertex = new gnomonClutEditorVertex(gnomonClutEditorMap(QPointF(v, a), d->min, d->max, this->width(), 100, log), QColor(r, g, b));

            d->table->addVertex(vertex);

            connect(vertex, SIGNAL(moved(const QPointF&)), d->scene, SIGNAL(moved(const QPointF&)));
        }
        n = n.nextSibling();
    }

    d->table->update();

    d->button_log->setChecked(log);

    this->onApply();
}

void gnomonClutEditor::exportClut(void)
{
    QDomDocument document("clut");

    QDomElement root = document.createElement("clut");
    root.setAttribute("min", d->min);
    root.setAttribute("max", d->max);
    root.setAttribute("log", d->button_log->isChecked() ? "1" : "0");
    document.appendChild(root);

    for(gnomonClutEditorVertex *vertex : d->table->vertices) {
        QPointF mapped = gnomonClutEditorMapInv(vertex->scenePos(), d->min, d->max, this->width(), 100, d->button_log->isChecked());

        QDomElement element = document.createElement("vertex");
        element.setAttribute("v", mapped.x());
        element.setAttribute("a", mapped.y());
        element.setAttribute("r", vertex->color().red());
        element.setAttribute("g", vertex->color().green());
        element.setAttribute("b", vertex->color().blue());
        root.appendChild(element);
    }

    QString fileName = QFileDialog::getSaveFileName(this, "Export color lookup table");

    QFile file(fileName);

    if (!file.open(QIODevice::WriteOnly))
        return;

    QTextStream out(&file); out << document.toString();

    file.close();
}

void gnomonClutEditor::toggle(bool toggle)
{
    QPropertyAnimation *animation = new QPropertyAnimation(this, "height");
    animation->setDuration(500);
    animation->setStartValue(0);
    animation->setEndValue(150);
    animation->setDirection(toggle ? QAbstractAnimation::Forward : QAbstractAnimation::Backward);
    animation->setEasingCurve(toggle ? QEasingCurve::OutBounce : QEasingCurve::InQuart);
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

QSize gnomonClutEditor::sizeHint(void) const
{
    return QSize(300, this->height());
}

void gnomonClutEditor::onApply(void)
{
    if(!d->table->vertices.count())
        return;

    if(!d->colorTransferFunction)
        d->colorTransferFunction = vtkColorTransferFunction::New();

    d->colorTransferFunction->RemoveAllPoints();

    d->colorTransferFunction->AddRGBPoint(
            gnomonClutEditorMapInv(QPointF(d->min, 0.0), d->min, d->max, this->width(), 100, d->button_log->isChecked()).x(),
            d->table->vertices.first()->color().redF(),
            d->table->vertices.first()->color().greenF(),
            d->table->vertices.first()->color().blueF());

    for(int i = 0; i < d->table->vertices.count(); i++) {

        gnomonClutEditorVertex *vertex = d->table->vertices.at(i);

        double v = gnomonClutEditorMapInv(vertex->scenePos(), d->min, d->max, this->width(), 100, d->button_log->isChecked()).x();
        double r = vertex->color().redF();
        double g = vertex->color().greenF();
        double b = vertex->color().blueF();

        d->colorTransferFunction->AddRGBPoint(v, r, g, b);
    }

    d->colorTransferFunction->AddRGBPoint(
            gnomonClutEditorMapInv(QPointF(d->max, 0.0), d->min, d->max, this->width(), 100, d->button_log->isChecked()).x(),
            d->table->vertices.first()->color().redF(),
            d->table->vertices.first()->color().greenF(),
            d->table->vertices.first()->color().blueF());

    if(!d->opacityTransferFunction)
        d->opacityTransferFunction = vtkPiecewiseFunction::New();

    d->opacityTransferFunction->RemoveAllPoints();

    d->opacityTransferFunction->AddPoint(
            d->min,
            gnomonClutEditorMapInv(d->table->vertices.first()->scenePos(), d->min, d->max, this->width(), 100, d->button_log->isChecked()).y());

    for(gnomonClutEditorVertex *vertex : d->table->vertices) {
        d->opacityTransferFunction->AddPoint(
             gnomonClutEditorMapInv(vertex->scenePos(), d->min, d->max, this->width(), 100, d->button_log->isChecked()).x(),
             gnomonClutEditorMapInv(vertex->scenePos(), d->min, d->max, this->width(), 100, d->button_log->isChecked()).y());

        // ///////////////////////////////////////////////////////////////////
        // Oversampling to tend to logarithmic
        // ///////////////////////////////////////////////////////////////////

        // if(!d->button_log->isChecked())
        //     continue;

        // if(!d->table->vertices.indexOf(vertex))
        //     continue;

        // gnomonClutEditorVertex *start = d->table->vertices.at(d->table->vertices.indexOf(vertex) - 1);
        // gnomonClutEditorVertex *stop = vertex;

        // qreal sampling = 100;

        // for(int i = 0; i < (int)sampling; i++) {

        //     qreal a = (stop->pos().y() - start->pos().y()) / (stop->pos().x() - start->pos().x());
        //     qreal b = start->pos().y() - a * start->pos().x();
        //     qreal v = start->pos().x() + (((stop->pos().x() - start->pos().x()) / sampling) * i);
        //     qreal l = a*v + b;

        //     d->opacityTransferFunction->AddPoint(
        //         gnomonClutEditorMapInv(QPointF(v, l), d->min, d->max, this->width(), 100, d->button_log->isChecked()).x(),
        //         gnomonClutEditorMapInv(QPointF(v, l), d->min, d->max, this->width(), 100, d->button_log->isChecked()).y());
        // }

        // ///////////////////////////////////////////////////////////////////
    }

    d->opacityTransferFunction->AddPoint(
         d->max,
         gnomonClutEditorMapInv(d->table->vertices.last()->scenePos(), d->min, d->max, this->width(), 100, d->button_log->isChecked()).y());

    emit updated();
}

void gnomonClutEditor::onColorAuto(void)
{
    QList<QGraphicsItem *> selection = d->scene->selectedItems();

    if(selection.count() != 1) {
        qDebug() << Q_FUNC_INFO << "Choose only one vertex for automatic color determination";
        return;
    }

    gnomonClutEditorVertex *vertex = dynamic_cast<gnomonClutEditorVertex *>(selection.first());

    if(!vertex) {
        qDebug() << Q_FUNC_INFO << "Choose only one vertex for automatic color determination";
        return;
    }

    QList<gnomonClutEditorVertex *> vertices = d->table->vertices;

    if(vertices.indexOf(vertex) == 0 || vertices.indexOf(vertex) == vertices.count()-1) {
        qDebug() << Q_FUNC_INFO << "Selected vertex must be surrounded by two other vertices";
        return;
    }

    gnomonClutEditorVertex *prev = vertices.at(vertices.indexOf(vertex)-1);
    gnomonClutEditorVertex *next = vertices.at(vertices.indexOf(vertex)+1);

    gnomonClutEditorInterpolator *interpolator = Q_NULLPTR;

    if(d->color_space == gnomonClutEditor::HSV)
        interpolator = new gnomonClutEditorInterpolatorHSV;

    if(d->color_space == gnomonClutEditor::RGB)
        interpolator = new gnomonClutEditorInterpolatorRGB;

    if(!interpolator)
        return;

    interpolator->setStart(prev);
    interpolator->setStop(next);

    vertex->setColor(interpolator->interpolate(vertex));

    if(d->button_apply->isChecked())
        this->onApply();

    delete interpolator;
}

void gnomonClutEditor::onColorChoose(void)
{
    QList<gnomonClutEditorVertex *> selection;

    for(QGraphicsItem *item : d->scene->selectedItems())
        if(gnomonClutEditorVertex *vertex = dynamic_cast<gnomonClutEditorVertex *>(item))
            selection << vertex;

    if(selection.count() == 0)
        return;

    QColorDialog dialog(this);
    dialog.setCurrentColor(selection.first()->color());

    if(dialog.exec())
        for(gnomonClutEditorVertex * vertex: selection)
            vertex->setColor(dialog.selectedColor());

    if(d->button_apply->isChecked())
        this->onApply();
}

void gnomonClutEditor::onSelectionChanged(void)
{
    QList<QGraphicsItem *> selection = d->scene->selectedItems();

    if(selection.count() != 1) {
        d->label_value->setText("--");
        d->label_alpha->setText("--");
    } else {
        if(gnomonClutEditorVertex *vertex = dynamic_cast<gnomonClutEditorVertex *>(selection.first()))
            this->onVertexMoved(vertex->scenePos());
    }
}

void gnomonClutEditor::onVertexMoved(const QPointF& point)
{
    QPointF mapped = gnomonClutEditorMapInv(point, d->min, d->max, this->width(), 100, d->button_log->isChecked());

    d->label_value->setText(QString::number(mapped.x(), 'g', 4));
    d->label_alpha->setText(QString::number(mapped.y(), 'g', 4));

    if(d->button_apply->isChecked())
        this->onApply();
}

void gnomonClutEditor::resizeEvent(QResizeEvent *event)
{
    d->scene->setSceneRect(0, -event->size().height(), event->size().width(), event->size().height());

    d->header->setPos(0, -event->size().height());
    d->header->setGeometry(0, -event->size().height(), event->size().width(), 45);

    d->bg->setPos(0, -100);
    d->bg->setRect(0, 0, event->size().width(), 100);

    if(event->size().height() < 150)
        return;

    for(gnomonClutEditorVertex *vertex : d->table->vertices)
        vertex->setPos(
            gnomonClutEditorMap(
                gnomonClutEditorMapInv(
                    vertex->scenePos(),
                    d->min,
                    d->max,
                    event->oldSize().width(),
                    event->oldSize().height(), d->button_log->isChecked()),
                d->min,
                d->max,
                event->size().width(),
                event->size().height(), d->button_log->isChecked()));
}

// /////////////////////////////////////////////////////////////////
// gnomonClutEditor - helper functions
// /////////////////////////////////////////////////////////////////

#include <cmath>

const double opacityCoefficient = 3;

QPointF gnomonClutEditorMap(QPointF vertex, qreal min, qreal max, int width, int height, bool logScale)
{
    qreal x = vertex.x();
    qreal y = vertex.y();

    if (logScale)
        y = std::pow(y, 1.0/opacityCoefficient);

    x = (x - min)/(max - min) * width;
    y =  y * height;

    y = qMax(0.0, y);
    y = qMin(y, (double)height);

    return QPointF(x, y);
}

QPointF gnomonClutEditorMapInv(QPointF vertex, qreal min, qreal max, int width, int height, bool logScale)
{
    qreal x = vertex.x()/double(width) * (max - min) + min;
    qreal y = vertex.y()/double(height) * -1.0 ;

    if (logScale)
        y = std::pow(y, opacityCoefficient);

    y = qMax(0.0, y);
    y = qMin(y, 1.0);

    return QPointF(x, y);
}

//
// gnomonClutEditor.cpp ends here
