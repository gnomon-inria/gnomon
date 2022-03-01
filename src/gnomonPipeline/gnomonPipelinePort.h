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

#include <gnomonPipelineExport.h>

#include <QtCore>

class gnomonPipelineNode;

class GNOMONPIPELINE_EXPORT gnomonPipelinePort : public QObject
{
    Q_OBJECT

    Q_PROPERTY(gnomonPipelineNode *node READ node);
    Q_PROPERTY(QString name READ name CONSTANT);
    Q_PROPERTY(QString label READ label WRITE setLabel NOTIFY labelChanged);

    Q_PROPERTY(int formIndex READ formIndex WRITE setFormIndex NOTIFY formIndexChanged)

public:
    enum Type {
        Input,
        Output
    };

public:
    gnomonPipelinePort(void) = default;
    gnomonPipelinePort(Type type, gnomonPipelineNode *parent);
    gnomonPipelinePort(Type type, const QString& name, gnomonPipelineNode *parent);
    ~gnomonPipelinePort(void);

public:
    gnomonPipelinePort::Type type(void);

public:
    gnomonPipelineNode *node(void);

public:
    QString name(void);

public:
    QString label(void);
    void setLabel(const QString& label);

public:
    int formIndex(void);
    void setFormIndex(int index);

signals:
    void formIndexChanged(int);
    void labelChanged(void);

private:
    class gnomonPipelinePortPrivate *d;
};

//
// gnomonPipelinePort.h ends here