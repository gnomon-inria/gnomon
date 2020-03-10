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

class GNOMONCOMPOSER_EXPORT gnomonPipelineNodeReader : public gnomonPipelineNode
{
public:
     gnomonPipelineNodeReader(const QString& algorithm_class, const QString& algorithm, const QString& path);
    ~gnomonPipelineNodeReader(void);

public:
    virtual QString toToml(const QString& node_name) override;

public:
    QString algorithm_class;
    QString algorithm;
    QString path;

    dtkComposerScenePort *output_port;
};

//
// gnomonPipelineNodeReader.h ends here