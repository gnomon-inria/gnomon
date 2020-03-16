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

class GNOMONCOMPOSER_EXPORT gnomonPipelineNodeConstructor : public gnomonPipelineNode
{
public:
     gnomonPipelineNodeConstructor(const QString& algorithm_class, const QString& algorithm, QMap<QString, QVariant> parameters, QList<QString> outputs);
    ~gnomonPipelineNodeConstructor(void);

public:
    virtual QString toToml(const QString& node_name) override;

    virtual QString toLuigiClass(void) override;

public:
    QString algorithm_class;
    QString algorithm;
    QMap<QString, QVariant> parameters;

    QMap<QString, dtkComposerScenePort *> output_ports;
};

//
// gnomonPipelineNodeConstructor.h ends here