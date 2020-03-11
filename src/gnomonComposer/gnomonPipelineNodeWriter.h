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
     gnomonPipelineNodeWriter(const QString& algorithm_class, const QString& algorithm, const QString& path, QList<QString> inputs);
    ~gnomonPipelineNodeWriter(void);

public:
    virtual QString toToml(const QString& node_name) override;

    virtual QString toLuigiClass(void) override;

public:
    QString algorithm_class;
    QString algorithm;
    QString path;

    QMap<QString, dtkComposerScenePort *> input_ports;

};

//
// gnomonPipelineNodeWriter.h ends here