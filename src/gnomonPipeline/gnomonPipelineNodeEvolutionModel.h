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

class GNOMONPIPELINE_EXPORT gnomonPipelineNodeEvolutionModel : public gnomonPipelineNode
{
public:
     gnomonPipelineNodeEvolutionModel(const QString& algorithm_class, const QString& algorithm, QJsonObject parameters, QList<QString> outputs);
    ~gnomonPipelineNodeEvolutionModel(void);

public:
    virtual QJsonObject parameters(void) override;
    virtual QStringList parametersName(void) override;
    virtual void configureParameter(const QString &name, dtkCoreParameter *param) override;

    virtual QString getParameterAsString(const QString &name) override;
    virtual double getParameterAsDouble(const QString &name) override;

public:
    virtual QString toToml(void) override;
    virtual QString toLuigiClass(void) override;
    virtual const QJsonObject toJson(void) override;


protected:
    class gnomonPipelineNodeEvolutionModelPrivate *dd;
};

//
// gnomonPipelineNodeEvolutionModel.h ends here