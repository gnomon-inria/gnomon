#pragma once

#include <QJsonObject>

class gnomonDynamicFormMetadataPrivate;

class gnomonDynamicFormMetadata: public QObject {
    Q_OBJECT
public:
    gnomonDynamicFormMetadata();
    gnomonDynamicFormMetadata(const gnomonDynamicFormMetadata& other);
    explicit gnomonDynamicFormMetadata(const QJsonObject& json);

    QJsonObject serialize() const;

    void deserialize(const QJsonObject& json);

    Q_PROPERTY(QStringList keys READ keys NOTIFY keysChanged);

public:
    QStringList keys();

signals:
    void keysChanged();

public:
    Q_INVOKABLE QString get(const QString &key);

    Q_INVOKABLE void set(const QString &key, const QString &value);

private:
    gnomonDynamicFormMetadataPrivate* d = nullptr;
};
