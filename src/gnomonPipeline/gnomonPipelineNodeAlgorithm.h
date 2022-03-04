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

class GNOMONPIPELINE_EXPORT gnomonPipelineNodeAlgorithm : public gnomonPipelineNode
{
public:
     gnomonPipelineNodeAlgorithm(const QString& algorithm_class, const QString& algorithm, QJsonObject parameters, QList<QString> inputs, QList<QString> outputs);
    ~gnomonPipelineNodeAlgorithm(void);

public:
    virtual const QJsonObject& parameters(void) override;

public:
    virtual QString toToml(void) override;
    virtual QString toLuigiClass(void) override;
    virtual const QJsonObject toJson(void) override;

protected:
    class gnomonPipelineNodeAlgorithmPrivate *dd;
};

//
// gnomonPipelineNodeAlgorithm.h ends here