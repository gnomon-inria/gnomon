//
// Created by Arthur Luciani on 22/02/2022.
//

#include "gnomonDynamicFormMetadata.h"

gnomonDynamicFormMetadata::gnomonDynamicFormMetadata(const QJsonObject &json):
                                    QObject(), name(), source(), description() {
    deserialize(json);
}

QJsonObject gnomonDynamicFormMetadata::serialize() const {
    QJsonObject json;
    json["name"] = name;
    json["source"] = source;
    json["description"] = description;
    return json;
}

void gnomonDynamicFormMetadata::deserialize(const QJsonObject &json) {
    name = json["name"].toString();
    source = json["source"].toString();
    description = json["description"].toString();
}
