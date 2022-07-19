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

#include "gnomonPipelineNode.h"
#include "gnomonPipelinePort.h"

// /////////////////////////////////////////////////////////////////
// gnomonPipelineEdge
// /////////////////////////////////////////////////////////////////

class GNOMONPIPELINE_EXPORT gnomonPipelineEdge : public QObject
{
    Q_OBJECT

    Q_PROPERTY(gnomonPipelinePort *source READ source WRITE setSource NOTIFY sourceChanged);
    Q_PROPERTY(gnomonPipelinePort *target READ target WRITE setTarget NOTIFY targetChanged);
    Q_PROPERTY(gnomonPipelineNode *parent READ parent WRITE setParent NOTIFY parentChanged);

    Q_PROPERTY(int formIndex READ formIndex WRITE setFormIndex NOTIFY formIndexChanged)

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

public:
    int formIndex(void);
    void setFormIndex(int index);

signals:
    void sourceChanged();
    void targetChanged();
    void parentChanged();
    void formIndexChanged(int);

private:
    class gnomonPipelineEdgePrivate *d;
};

//
// gnomonPipelineEdge.h ends here

