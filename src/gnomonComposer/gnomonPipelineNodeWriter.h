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

#include <dtkComposer>

class GNOMONCOMPOSER_EXPORT gnomonPipelineNodeWriter : public dtkComposerSceneNodeComposite
{
public:
     gnomonPipelineNodeWriter(const QString& algorithm_class, const QString& algorithm, const QString& path);
    ~gnomonPipelineNodeWriter(void);

public:
    QString algorithm_class;
    QString algorithm;
    QString path;

    dtkComposerScenePort *input_port;

};

//
// gnomonPipelineNodeWriter.h ends here