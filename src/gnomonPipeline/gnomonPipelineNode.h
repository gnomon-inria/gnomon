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
#include <QtGui>

class gnomonPipelineEdge;
class gnomonPipelinePort;

class GNOMONPIPELINE_EXPORT gnomonPipelineNode : public QObject
{
    Q_OBJECT

public:
    gnomonPipelineNode(void);
    ~gnomonPipelineNode(void);

public:
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged);
    Q_PROPERTY(QString algorithmClass READ algorithmClass CONSTANT);
    Q_PROPERTY(QString algorithmPlugin READ algorithmPlugin CONSTANT);
    Q_PROPERTY(QColor color READ color);
    Q_PROPERTY(QPointF position READ position WRITE setPosition NOTIFY positionChanged);

    Q_PROPERTY(QStringList inputPortsNames READ inputPortsNames NOTIFY inputPortsChanged);
    Q_PROPERTY(QStringList outputPortsNames READ outputPortsNames NOTIFY outputPortsChanged);

    Q_PROPERTY(int inputEdgeCount READ inputEdgeCount);
    Q_PROPERTY(int outputEdgeCount READ outputEdgeCount);

public:
    const QString& name(void);

    const QString& algorithmClass(void);
    const QString& algorithmPlugin(void);
    const QColor& color(void);

    const QPointF& position(void);

public:
    void setName(const QString& node_name);
    void setPosition(const QPointF& pos);

signals:
    void nameChanged(void);

    void positionChanged(void);

    void inputPortsChanged(void);
    void outputPortsChanged(void);

/*public:
    void layout(void);

public:
    void resize(qreal width, qreal height);

public:
    QRectF boundingRect(void) const;

public:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);*/
    
public:
    int inputEdgeCount(void);
    int outputEdgeCount(void);

    Q_INVOKABLE gnomonPipelineEdge *inputEdgeAt(int index);
    Q_INVOKABLE gnomonPipelineEdge *outputEdgeAt(int index);

    void  addInputEdge(gnomonPipelineEdge *edge);
    void addOutputEdge(gnomonPipelineEdge *edge);

    void  removeInputEdge(gnomonPipelineEdge *edge);
    void removeOutputEdge(gnomonPipelineEdge *edge);

public:
    const QMap<QString, gnomonPipelinePort *>& inputPorts(void);
    const QMap<QString, gnomonPipelinePort *>& outputPorts(void);

    QStringList inputPortsNames(void);
    QStringList outputPortsNames(void);

    Q_INVOKABLE gnomonPipelinePort *inputPort(const QString& name);
    Q_INVOKABLE gnomonPipelinePort *outputPort(const QString& name);

    int  addInputPort(const QString& name, gnomonPipelinePort *port);
    void addOutputPort(const QString& name, gnomonPipelinePort *port);

    void  removeInputPort(gnomonPipelinePort *port);
    void removeOutputPort(gnomonPipelinePort *port);

public:
    virtual QString toToml(void);
    virtual QString toLuigiClass(void);
    virtual const QJsonObject toJson(void);

protected:
    class gnomonPipelineNodePrivate *d;
};

//
// gnomonPipelineNode.h ends here