#include "gnomonAlgorithm/gnomonAbstractAlgorithm.h"
#include "gnomonCommand/gnomonAbstractAdapterCommand.h"
#include "gnomonCore.h"

void gnomonAbstractAlgorithm::connectParameter(dtkCoreParameter *parameter) {
    parameter->connect([this] (QVariant v) {
        qDebug()<<Q_FUNC_INFO;
        dtkCoreParameterInList<QString> propertyParameter = v.value<dtkCoreParameterInList<QString>>();
        QString sender_label = propertyParameter.label();
        this->updateParameters(sender_label);
    });
}

void gnomonAbstractAlgorithm::refreshParameters(void) {
    qDebug()<<Q_FUNC_INFO<<"Not implemented";
}

void gnomonAbstractAlgorithm::updateParameters(const QString& sender_name) {
    qDebug()<<Q_FUNC_INFO<<"Not implemented";
}

namespace gnomonCore {
    GNOMON_DEFINE_CONCEPT(gnomonAbstractAlgorithm, algorithm, gnomonCore);
}

//
// gnomonAbstractAlgorithm.cpp ends here
