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
class gnomonPipelinePort;

// /////////////////////////////////////////////////////////////////
// gnomonPipelineEdge
// /////////////////////////////////////////////////////////////////

class GNOMONPIPELINE_EXPORT gnomonPipelineEdge : public QObject
{
    Q_OBJECT

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

