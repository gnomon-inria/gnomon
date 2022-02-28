#pragma once

#include <QJsonObject>

class gnomonDynamicFormMetadata: public QObject {
    Q_OBJECT
public:
    gnomonDynamicFormMetadata() = default;
    gnomonDynamicFormMetadata(const gnomonDynamicFormMetadata& other);
    explicit gnomonDynamicFormMetadata(const QJsonObject& json);

    QJsonObject serialize() const;

    void deserialize(const QJsonObject& json);

public:
    Q_PROPERTY(QString name MEMBER name NOTIFY nameChanged)
    Q_PROPERTY(QString source MEMBER source NOTIFY sourceChanged)
    Q_PROPERTY(QString description MEMBER description NOTIFY descriptionChanged)

signals:
    void nameChanged(void);

    void sourceChanged(void);

    void descriptionChanged(void);

public:
    const QString &getName() const;

    void setName(const QString &name);

    const QString &getSource() const;

    void setSource(const QString &source);

    const QString &getDescription() const;

    void setDescription(const QString &description);

private:
    QString name = "";
    QString source = "";
    QString description = "";
};
