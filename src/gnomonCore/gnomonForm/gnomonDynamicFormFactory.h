#pragma once

#include <QtCore>
#include "gnomonAbstractDynamicForm.h"

namespace gnomonForm {
    void registerForms(void);
    std::shared_ptr<gnomonAbstractDynamicForm> createDynamicForm(const QString &type);
    std::shared_ptr<gnomonAbstractDynamicForm> createDynamicForm(const QJsonObject &serialization);
}