#include "gnomonAlgorithm/gnomonAbstractAlgorithm.h"
#include "gnomonCommand/gnomonAbstractAdapterCommand.h"
#include "gnomonCore.h"

void gnomonAbstractAlgorithm::connectParameter(const QString& parameter_name)
{
    dtkCoreParameters params = this->parameters();
    dtkCoreParameter *parameter = params.value(parameter_name, nullptr);
    if (parameter) {
        parameter->connect([=]() {
            this->updateParameters(parameter_name);
        });
    }
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
