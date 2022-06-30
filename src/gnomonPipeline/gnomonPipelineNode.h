#pragma once

#include <gnomonPipelineExport.h>

#include <QtCore>

class gnomonPipelineEdge;
class gnomonPipelinePort;
class dtkCoreParameter;

class GNOMONPIPELINE_EXPORT gnomonPipelineNode : public QObject
{
    Q_OBJECT

public:
    enum Type {
        NODE_DEFAULT = -1,
        NODE_ALGORITHM = 0,
        NODE_READER = 1,
        NODE_WRITER = 2,
        NODE_CONSTRUCTOR = 3,
        NODE_FILTER = 4,
        NODE_CONVERTER = 5,
        NODE_ADAPTER = 6,
        NODE_TASK = 7,
        NODE_MORPHONET = 8
    };

public:
    gnomonPipelineNode(void);
#ifndef SWIG
    gnomonPipelineNode(Type t);
#endif
    ~gnomonPipelineNode(void);


public:
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged);
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged);
    Q_PROPERTY(QString version READ version WRITE setVersion NOTIFY versionChanged);
    Q_PROPERTY(QString algorithmClass READ algorithmClass CONSTANT);
    Q_PROPERTY(QString algorithmPlugin READ algorithmPlugin CONSTANT);
    Q_PROPERTY(QString path READ path CONSTANT);
    Q_PROPERTY(QJsonObject parameters READ parameters CONSTANT)

    Q_PROPERTY(Type type READ type);
    Q_PROPERTY(QPointF position READ position WRITE setPosition NOTIFY positionChanged);

    Q_PROPERTY(QStringList inputPortsNames READ inputPortsNames NOTIFY inputPortsChanged);
    Q_PROPERTY(QStringList outputPortsNames READ outputPortsNames NOTIFY outputPortsChanged);

    Q_PROPERTY(int inputEdgeCount READ inputEdgeCount);
    Q_PROPERTY(int outputEdgeCount READ outputEdgeCount);

    Q_ENUM(Type);

public:
    const QString& name(void);
    const QString& description(void);
    const QString& version(void);

    const QString& algorithmClass(void);
    const QString& algorithmPlugin(void);

    virtual inline QString path(void) { return ""; };
    virtual QJsonObject parameters(void);
    virtual QJsonObject morphonet_data(void);
    virtual QStringList parametersName(void);
    virtual void configureParameter(const QString &name, dtkCoreParameter *param);

    Type type(void);
    const QPointF& position(void);

public:
    void setName(const QString& node_name);
    void setDescription(const QString& desc);
    void setVersion(const QString& version);

    void setPosition(const QPointF& pos);

signals:
    void nameChanged(void);
    void descriptionChanged(void);
    void versionChanged(void);

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
    QList<gnomonPipelineEdge *> inputEdges(void);
    QList<gnomonPipelineEdge *> outputEdges(void);

    gnomonPipelineEdge* inputEdgeFromPort(const QString& portName);
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