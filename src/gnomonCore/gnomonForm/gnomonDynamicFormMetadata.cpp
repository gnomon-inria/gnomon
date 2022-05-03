#include "gnomonDynamicFormMetadata.h"

gnomonDynamicFormMetadata::gnomonDynamicFormMetadata(const QJsonObject &json) {
    deserialize(json);
}

QJsonObject gnomonDynamicFormMetadata::serialize() const {
    return this->data();
}

void gnomonDynamicFormMetadata::deserialize(const QJsonObject &json) {
    for(const auto & key: json.keys()) {
        m_data[key] = json[key].toString();
    }
}

void gnomonDynamicFormMetadata::clear(void) {
    m_data.clear();
    emit keysChanged();
}

QStringList gnomonDynamicFormMetadata::keys() const {
    return m_data.keys();
}

QJsonObject gnomonDynamicFormMetadata::data(void) const {
    QJsonObject json;
    for(const auto& key: m_data.keys()) {
        json[key] = m_data[key];
    }
    return json;
}

QString gnomonDynamicFormMetadata::get(const QString &key) const {
    if (m_data.contains(key)) {
        return m_data[key];
    } 

    qWarning() << Q_FUNC_INFO << "metadata doesnt contains " << key << " keys are " << this->keys();
    return "default";
    
}

void gnomonDynamicFormMetadata::set(const QString &key, const QString &value) {
    if (!m_data.contains(key) || (value != m_data[key])) {
        m_data[key] = value;
        emit dataChanged();
    }
}