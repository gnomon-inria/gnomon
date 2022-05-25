//
// Created by Arthur Luciani on 09/05/2022.
//

#include "gnomonPipelineNodeTask.h"

#include <dtkCore>
#include <QtCore>

#include "gnomonPipelineNode_p.h"
#include "gnomonPipelinePort.h"


gnomonPipelineNodeTask::gnomonPipelineNodeTask(const QString &task, QList<QString> inputs, QList<QString> outputs) {

    d->color = QColor(147, 112, 219);
    d->algorithm_class = "task";
    d->algorithm = task;

    for (const auto& input : inputs) {
        this->addInputPort(input, new gnomonPipelinePort(gnomonPipelinePort::Input, input, this));
    }
    for (const auto& output : outputs) {
        this->addOutputPort(output, new gnomonPipelinePort(gnomonPipelinePort::Output, output, this));
    }
}

QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm>>
gnomonPipelineNodeTask::runTask(QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm>> inputs) {
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm>> outputs;

    if(d->algorithm == "compose") {
        auto first  = inputs["first"]->clone();
        first->compose(inputs["second"]);
        outputs["output"] = first;
    } else {
        qWarning() << Q_FUNC_INFO << "Unknown task: " << d->algorithm;
    }

    return outputs;
}

gnomonPipelineNodeTask *gnomonPipelineNodeTask::_dynamic_cast(gnomonPipelineNode *o) {
    return dynamic_cast<gnomonPipelineNodeTask*>(o);
}





