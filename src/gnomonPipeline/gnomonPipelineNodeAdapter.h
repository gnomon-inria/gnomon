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

class GNOMONPIPELINE_EXPORT gnomonPipelineNodeAdapter : public gnomonPipelineNode
{
public:
     gnomonPipelineNodeAdapter(const QString& algorithm_class, const QString& algorithm, QList<QString> inputs, QList<QString> outputs);
    ~gnomonPipelineNodeAdapter(void);

public:
    virtual QString toToml(void) override;
    virtual QString toLuigiClass(void) override;
    virtual const QJsonObject toJson(void) override;
};

//
// gnomonPipelineNodeAdapter.h ends here