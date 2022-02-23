//
// Created by Arthur Luciani on 22/02/2022.
//

#ifndef GNOMON_GNOMONDYNAMICFORMMETADATA_H
#define GNOMON_GNOMONDYNAMICFORMMETADATA_H


#include <QJsonObject>

class gnomonDynamicFormMetadata: QObject {
    Q_OBJECT
public:
    gnomonDynamicFormMetadata() = default;

    explicit gnomonDynamicFormMetadata(const QJsonObject& json);

    QJsonObject serialize() const;

    void deserialize(const QJsonObject& json);

public:
    Q_PROPERTY(QString name MEMBER name)
    Q_PROPERTY(QString source MEMBER source)
    Q_PROPERTY(QString description MEMBER description)

private:
    QString name = "";
    QString source = "";
    QString description = "";
};


#endif //GNOMON_GNOMONDYNAMICFORMMETADATA_H
