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
     gnomonPipelineNodeReader(const QString& algorithm_class, const QString& algorithm, const QString& path, QList<QString> outputs);
    ~gnomonPipelineNodeReader(void);

public:
    const QMap<QString, dtkComposerScenePort *>& outputPorts(void);

public:
    virtual QString toToml(const QString& node_name) override;
    virtual const QJsonObject toJson(const QString& node_name) override;
    virtual QString toLuigiClass(void) override;

protected:
    class gnomonPipelineNodeReaderPrivate *dd;
};

//
// gnomonPipelineNodeReader.h ends here