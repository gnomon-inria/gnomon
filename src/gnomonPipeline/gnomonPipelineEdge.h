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
class gnomonPipelinePort;

// /////////////////////////////////////////////////////////////////
// gnomonPipelineEdge
// /////////////////////////////////////////////////////////////////

class gnomonPipelineEdge
{
public:
    gnomonPipelineEdge(void);
    ~gnomonPipelineEdge(void);

public:
    gnomonPipelinePort *source(void);
    gnomonPipelinePort *target(void);

public:
    void setSource(gnomonPipelinePort *port);
    void setTarget(gnomonPipelinePort *port);

public:
    bool   link(void);
    bool unlink(void);

public:
    gnomonPipelineNode *parent(void);

public:
    void setParent(gnomonPipelineNode *parent);

private:
    class gnomonPipelineEdgePrivate *d;
};



//
// gnomonPipelineEdge.h ends here

