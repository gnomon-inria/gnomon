/* gnomonPipelineNode.h ---
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


class DTKCOMPOSER_EXPORT gnomonPipelineNode : public dtkComposerSceneNode
{
public:
    gnomonPipelineNode(void);
    ~gnomonPipelineNode(void);

public:
    const QString& algorithmClass(void);

public:
    void layout(void);

public:
    void resize(qreal width, qreal height);

public:
    QRectF boundingRect(void) const;

public:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

public:
    virtual QString toToml(const QString& node_name);
    virtual QString toLuigiClass(void);
    virtual const QJsonObject toJson(const QString& node_name);

protected:
    class gnomonPipelineNodePrivate *d;
};
