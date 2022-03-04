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

#include "gnomonPipelineNode.h"

class gnomonPipelinePort;

class GNOMONPIPELINE_EXPORT gnomonPipelineNodeWriter : public gnomonPipelineNode
{
public:
     gnomonPipelineNodeWriter(const QString& algorithm_class, const QString& algorithm, const QString& path, QList<QString> inputs);
    ~gnomonPipelineNodeWriter(void);

public:
    virtual QString path(void) override;

public:
    virtual QString toToml(void) override;
    virtual QString toLuigiClass(void) override;
    virtual const QJsonObject toJson(void) override;

protected:
    class gnomonPipelineNodeWriterPrivate *dd;
};

//
// gnomonPipelineNodeWriter.h ends here