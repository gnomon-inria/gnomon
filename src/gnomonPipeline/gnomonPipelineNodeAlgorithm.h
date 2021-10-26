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

#include <gnomonComposerExport.h>

#include "gnomonPipelineNode.h"

class gnomonPipelinePort;

class GNOMONCOMPOSER_EXPORT gnomonPipelineNodeAlgorithm : public gnomonPipelineNode
{
public:
     gnomonPipelineNodeAlgorithm(const QString& algorithm_class, const QString& algorithm, QVariantMap parameters, QList<QString> inputs, QList<QString> outputs);
    ~gnomonPipelineNodeAlgorithm(void);

public:
    const QMap<QString, gnomonPipelinePort *>& inputPorts(void);
    const QMap<QString, gnomonPipelinePort *>& outputPorts(void);

public:
    virtual QString toToml(const QString& node_name) override;
    virtual QString toLuigiClass(void) override;
    virtual const QJsonObject toJson(const QString& node_name) override;

protected:
    class gnomonPipelineNodeAlgorithmPrivate *dd;
};

//
// gnomonPipelineNodeAlgorithm.h ends here