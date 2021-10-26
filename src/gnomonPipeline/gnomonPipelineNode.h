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

// #include <dtkComposerExport.h>
// #include <dtkComposer>
#include <gnomonPipelineExport.h>

#include <QtCore>

class gnomonPipelineEdge;
class gnomonPipelinePort;

class GNOMONPIPELINE_EXPORT gnomonPipelineNode : public QObject
{
    Q_OBJECT

public:
    gnomonPipelineNode(void);
    ~gnomonPipelineNode(void);

public:
    const QString& algorithmClass(void);

/*public:
    void layout(void);

public:
    void resize(qreal width, qreal height);

public:
    QRectF boundingRect(void) const;

public:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);*/
    
public:
    void  addInputEdge(gnomonPipelineEdge *edge);
    void addOutputEdge(gnomonPipelineEdge *edge);

    void  removeInputEdge(gnomonPipelineEdge *edge);
    void removeOutputEdge(gnomonPipelineEdge *edge);

public:
    int  addInputPort(gnomonPipelinePort *port);
    void addOutputPort(gnomonPipelinePort *port);

    void  removeInputPort(gnomonPipelinePort *port);
    void removeOutputPort(gnomonPipelinePort *port);

public:
    virtual QString toToml(const QString& node_name);
    virtual QString toLuigiClass(void);
    virtual const QJsonObject toJson(const QString& node_name);

protected:
    class gnomonPipelineNodePrivate *d;
};
