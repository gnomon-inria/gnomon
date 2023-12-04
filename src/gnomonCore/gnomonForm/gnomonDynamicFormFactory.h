#pragma once

#include <QtCore>
#include "gnomonAbstractDynamicForm.h"

std::shared_ptr<gnomonAbstractDynamicForm> createDynamicForm(const QString &type);

std::shared_ptr<gnomonAbstractDynamicForm> createDynamicForm(const QJsonObject &serialization);
