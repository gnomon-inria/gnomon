#include "gnomonDynamicFormMetadata.h"

gnomonDynamicFormMetadata::gnomonDynamicFormMetadata(const QJsonObject &json) {
    deserialize(json);
}

gnomonDynamicFormMetadata::gnomonDynamicFormMetadata(const gnomonDynamicFormMetadata& other) {
    name = other.name;
    source = other.source;
    description = other.description;
}

QJsonObject gnomonDynamicFormMetadata::serialize() const {
    QJsonObject json;
    json["name"] = name;
    json["source"] = source;
    json["description"] = description;
    return json;
}

void gnomonDynamicFormMetadata::deserialize(const QJsonObject &json) {
    setName(json["name"].toString());
    setSource(json["source"].toString());
    setDescription(json["description"].toString());
}

const QString &gnomonDynamicFormMetadata::getName() const {
    return name;
}

void gnomonDynamicFormMetadata::setName(const QString &name) {
    gnomonDynamicFormMetadata::name = name;
    emit nameChanged();
}

const QString &gnomonDynamicFormMetadata::getSource() const {
    return source;
}

void gnomonDynamicFormMetadata::setSource(const QString &source) {
    gnomonDynamicFormMetadata::source = source;
    emit sourceChanged();
}

const QString &gnomonDynamicFormMetadata::getDescription() const {
    return description;
}

void gnomonDynamicFormMetadata::setDescription(const QString &description) {
    gnomonDynamicFormMetadata::description = description;
    emit descriptionChanged();
}

