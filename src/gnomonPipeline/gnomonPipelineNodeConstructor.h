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

class GNOMONPIPELINE_EXPORT gnomonPipelineNodeConstructor : public gnomonPipelineNode
{
public:
     gnomonPipelineNodeConstructor(const QString& algorithm_class, const QString& algorithm, QVariantMap parameters, QList<QString> outputs);
    ~gnomonPipelineNodeConstructor(void);

public:
    const QMap<QString, gnomonPipelinePort *>& outputPorts(void);

public:
    virtual QString toToml(const QString& node_name) override;
    virtual QString toLuigiClass(void) override;
    virtual const QJsonObject toJson(const QString& node_name) override;


protected:
    class gnomonPipelineNodeConstructorPrivate *dd;
};

//
// gnomonPipelineNodeConstructor.h ends here