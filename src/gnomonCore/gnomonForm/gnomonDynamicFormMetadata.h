#pragma once

#include <QJsonObject>

class gnomonDynamicFormMetadata: public QObject {
    Q_OBJECT

public:
    Q_PROPERTY(QStringList keys READ keys NOTIFY keysChanged);
    Q_PROPERTY(QJsonObject data READ data NOTIFY dataChanged);

public:
    gnomonDynamicFormMetadata() = default;
    explicit gnomonDynamicFormMetadata(const QJsonObject& json);
    virtual ~gnomonDynamicFormMetadata() = default;
    gnomonDynamicFormMetadata& operator =(const gnomonDynamicFormMetadata& o);

    QJsonObject serialize() const;
    void deserialize(const QJsonObject& json);
    void clear(void);

public:
    QStringList keys(void) const;
    QJsonObject data(void) const;

signals:
    void keysChanged();
    void dataChanged();

public:
    Q_INVOKABLE QString get(const QString &key) const;
    Q_INVOKABLE void set(const QString &key, const QString &value);

protected:
    QMap<QString, QString> m_data = {};
};
