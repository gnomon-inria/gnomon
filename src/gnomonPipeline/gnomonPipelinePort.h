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

public:
    enum Type {
        Input,
        Output
    };

public:
    gnomonPipelinePort(void) = default;
    gnomonPipelinePort(Type type, gnomonPipelineNode *parent);
    gnomonPipelinePort(Type type, const QString& label, gnomonPipelineNode *parent);
    ~gnomonPipelinePort(void);

public:
    gnomonPipelinePort::Type type(void);

public:
    gnomonPipelineNode *node(void);

public:
    QString label(void);

    void setLabel(const QString& label);

private:
    class gnomonPipelinePortPrivate *d;
};

//
// gnomonPipelinePort.h ends here