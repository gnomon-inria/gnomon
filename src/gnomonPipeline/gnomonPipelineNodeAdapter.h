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

class GNOMONCOMPOSER_EXPORT gnomonPipelineNodeAdapter : public gnomonPipelineNode
{
public:
     gnomonPipelineNodeAdapter(const QString& algorithm_class, const QString& algorithm, QList<QString> inputs, QList<QString> outputs);
    ~gnomonPipelineNodeAdapter(void);

public:
    const QMap<QString, gnomonPipelinePort *>& inputPorts(void);
    const QMap<QString, gnomonPipelinePort *>& outputPorts(void);

public:
    virtual QString toToml(const QString& node_name) override;
    virtual QString toLuigiClass(void) override;
    virtual const QJsonObject toJson(const QString& node_name) override;

protected:
    class gnomonPipelineNodeAdapterPrivate *dd;
};

//
// gnomonPipelineNodeAdapter.h ends here