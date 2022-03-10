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
     gnomonPipelineNodeConstructor(const QString& algorithm_class, const QString& algorithm, QJsonObject parameters, QList<QString> outputs);
    ~gnomonPipelineNodeConstructor(void);

public:
    virtual QJsonObject parameters(void) override;
    virtual QStringList parametersName(void) override;
    virtual dtkCoreParameter *parameter(const QString &name) override;

public:
    virtual QString toToml(void) override;
    virtual QString toLuigiClass(void) override;
    virtual const QJsonObject toJson(void) override;


protected:
    class gnomonPipelineNodeConstructorPrivate *dd;
};

//
// gnomonPipelineNodeConstructor.h ends here