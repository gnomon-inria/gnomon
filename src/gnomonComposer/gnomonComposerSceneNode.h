/* gnomonComposerSceneNode.h ---
 *
 * Author: Julien Wintz
 * Created: Fri Feb  3 12:32:09 2012 (+0100)
 */

/* Commentary:
 *
 */

/* Change log:
 *
 */

#pragma once

#include <dtkComposerExport.h>
#include <dtkComposer>


class DTKCOMPOSER_EXPORT gnomonComposerSceneNode : public dtkComposerSceneNode
{
public:
    gnomonComposerSceneNode(void);
    ~gnomonComposerSceneNode(void);

public:
    void layout(void);

public:
    void resize(const QSizeF& size);
    void resize(qreal width, qreal height);

public:
    QRectF boundingRect(void) const;

public:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

public:
    void resetPos(const QPointF& pos, const QRectF& rect);

protected:
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
    void dragLeaveEvent(QGraphicsSceneDragDropEvent *event);
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event);
    void dropEvent(QGraphicsSceneDragDropEvent *event);

protected:
    class gnomonComposerSceneNodePrivate *d;
};
