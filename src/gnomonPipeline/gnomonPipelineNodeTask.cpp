//
// Created by Arthur Luciani on 09/05/2022.
//

#include "gnomonPipelineNodeTask.h"

#include <dtkCore>
#include <QtCore>
#include <gnomonProject>

#include "gnomonPipelineNode_p.h"
#include "gnomonPipelinePort.h"


gnomonPipelineNodeTask::gnomonPipelineNodeTask(const QString &task, QList<QString> inputs, QList<QString> outputs) {

    d->type = gnomonPipelineNode::NODE_TASK;
    d->algorithm_class = "task";
    d->algorithm = task;

    for (const auto& input : inputs) {
        this->addInputPort(input, new gnomonPipelinePort(gnomonPipelinePort::Input, input, this));
    }
    for (const auto& output : outputs) {
        this->addOutputPort(output, new gnomonPipelinePort(gnomonPipelinePort::Output, output, this));
    }
}

QMap<QString, QString>
gnomonPipelineNodeTask::runTask(QMap<QString, QString> inputs) {
    QMap<QString, QString> outputs;

    if(d->algorithm == "compose") {
        auto first = GNOMON_SESSION->getForm(inputs["first"])->clone();
        first->compose(GNOMON_SESSION->getForm(inputs["second"]));
        outputs["output"] = first->uuid();
    } else {
        qWarning() << Q_FUNC_INFO << "Unknown task: " << d->algorithm;
    }

    return outputs;
}

gnomonPipelineNodeTask *gnomonPipelineNodeTask::_dynamic_cast(gnomonPipelineNode *o) {
    return dynamic_cast<gnomonPipelineNodeTask*>(o);
}





