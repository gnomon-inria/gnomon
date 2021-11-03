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
    virtual QString toToml(const QString& node_name) override;
    virtual QString toLuigiClass(void) override;
    virtual const QJsonObject toJson(const QString& node_name) override;
};

//
// gnomonPipelineNodeAdapter.h ends here