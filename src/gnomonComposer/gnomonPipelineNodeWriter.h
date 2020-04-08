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
    const QMap<QString, dtkComposerScenePort *>& inputPorts(void);

public:
    virtual QString toToml(const QString& node_name) override;
    virtual QString toLuigiClass(void) override;

protected:
    class gnomonPipelineNodeWriterPrivate *dd;
};

//
// gnomonPipelineNodeWriter.h ends here