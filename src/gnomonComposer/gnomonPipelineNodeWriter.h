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

class GNOMONCOMPOSER_EXPORT gnomonPipelineNodeWriter : public gnomonPipelineNode
{
public:
     gnomonPipelineNodeWriter(const QString& algorithm_class, const QString& algorithm, const QString& path);
    ~gnomonPipelineNodeWriter(void);

public:
    virtual QString toToml(const QString& node_name) override;

public:
    QString algorithm_class;
    QString algorithm;
    QString path;

    dtkComposerScenePort *input_port;

};

//
// gnomonPipelineNodeWriter.h ends here