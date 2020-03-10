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

#include "gnomonComposerSceneNode.h"

class GNOMONCOMPOSER_EXPORT gnomonPipelineNodeAlgorithm : public gnomonComposerSceneNode
{
public:
     gnomonPipelineNodeAlgorithm(const QString& algorithm_class, const QString& algorithm, QMap<QString, QVariant> parameters, QList<QString> inputs, QList<QString> outputs);
    ~gnomonPipelineNodeAlgorithm(void);

public:
    QString algorithm_class;
    QString algorithm;
    QMap<QString, QVariant> parameters;

    QMap<QString, dtkComposerScenePort *> input_ports;
    QMap<QString, dtkComposerScenePort *> output_ports;
};

//
// gnomonPipelineNodeAlgorithm.h ends here