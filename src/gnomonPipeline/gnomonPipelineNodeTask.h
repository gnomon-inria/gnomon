//
// Created by Arthur Luciani on 09/05/2022.
//

#pragma once

#include <gnomonPipelineExport.h>

#include "gnomonPipelineNode.h"
#include "gnomonForm/gnomonAbstractDynamicForm"


class GNOMONPIPELINE_EXPORT gnomonPipelineNodeTask : public gnomonPipelineNode {

public:
    gnomonPipelineNodeTask(const QString& task, QList<QString> inputs, QList<QString> outputs);

    QMap<QString, QString>
    runTask(QMap<QString, QString> inputs);

    static gnomonPipelineNodeTask *_dynamic_cast(gnomonPipelineNode *o);
};

