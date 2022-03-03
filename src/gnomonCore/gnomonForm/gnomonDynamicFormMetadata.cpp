#include "gnomonDynamicFormMetadata.h"

class gnomonDynamicFormMetadataPrivate {
    friend gnomonDynamicFormMetadata;
private:
    QMap<QString, QString> data = {};
};

gnomonDynamicFormMetadata::gnomonDynamicFormMetadata() {
    d = new gnomonDynamicFormMetadataPrivate();
}

gnomonDynamicFormMetadata::gnomonDynamicFormMetadata(const QJsonObject &json) {
    d = new gnomonDynamicFormMetadataPrivate();
    deserialize(json);
}

gnomonDynamicFormMetadata::gnomonDynamicFormMetadata(const gnomonDynamicFormMetadata& other) {
    d = new gnomonDynamicFormMetadataPrivate();
    for(const auto& key: other.d->data.keys()) {
        d->data[key] = other.d->data[key];
    }
}

QJsonObject gnomonDynamicFormMetadata::serialize() const {
    QJsonObject json;
    for(const auto& key: d->data.keys()) {
        json[key] = d->data[key];
    }
    return json;
}

void gnomonDynamicFormMetadata::deserialize(const QJsonObject &json) {
    for(const auto & key: json.keys()) {
        d->data[key] = json[key].toString();
    }
}

QStringList gnomonDynamicFormMetadata::keys() {
    return d->data.keys();
}

QString gnomonDynamicFormMetadata::get(const QString &key) {
    if (!d->data.contains(key)) {
        d->data[key] = "default";  // default
    }
    return d->data[key];
}

void gnomonDynamicFormMetadata::set(const QString &key, const QString &value) {
    d->data[key] = value;
}
