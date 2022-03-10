#pragma once

#include <QJsonObject>

class gnomonDynamicFormMetadataPrivate;

class gnomonDynamicFormMetadata: public QObject {
    Q_OBJECT

public:
    Q_PROPERTY(QStringList keys READ keys NOTIFY keysChanged);
    Q_PROPERTY(QJsonObject data READ data NOTIFY dataChanged);

public:
    gnomonDynamicFormMetadata();
    gnomonDynamicFormMetadata(const gnomonDynamicFormMetadata& other);
    explicit gnomonDynamicFormMetadata(const QJsonObject& json);

    virtual ~gnomonDynamicFormMetadata();

    QJsonObject serialize() const;

    void deserialize(const QJsonObject& json);

public:
    QStringList keys(void);
    QJsonObject data(void) const;

signals:
    void keysChanged();
    void dataChanged();

public:
    Q_INVOKABLE QString get(const QString &key);
    Q_INVOKABLE void set(const QString &key, const QString &value);

private:
    gnomonDynamicFormMetadataPrivate* d = nullptr;
};
