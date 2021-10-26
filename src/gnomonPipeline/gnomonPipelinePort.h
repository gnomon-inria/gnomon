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


class gnomonPipelineNode;

class gnomonPipelinePort
{
public:
    enum Type {
        Input,
        Output
    };

public:
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