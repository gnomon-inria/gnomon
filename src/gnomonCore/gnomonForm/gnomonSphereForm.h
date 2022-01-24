#pragma once

#include <gnomonCoreExport.h>

#include "gnomonAbstractForm.h"
#include "gnomonTypeDef.h"
#include <dtkCore>

class gnomonAbstractForm;

class GNOMONCORE_EXPORT gnomonSphereForm : public gnomonAbstractForm
{

public:
    gnomonSphereForm(void);
    explicit gnomonSphereForm(QJsonObject& serialization) {
        deserialize(serialization);
    }
    ~gnomonSphereForm(void) = default;
    gnomonSphereForm *clone(void);

public:
    QString name(void) const override;
    QMap<QString,QString> metadata(void) const override { return QMap<QString,QString>(); }
    QString dataName(void) const override { return "internal";}
    const QString pluginName(void) override {
        return "";
    }

    QJsonObject serialize(void) override {
        QJsonObject out;
        QJsonArray center;
        for (int i = 0; i < m_center.size(); ++i) {
            center[i] = m_center[i];
        }
        out["m_center"] = center;
        out["m_radius"] = m_radius;
        return out;
    }
    void deserialize(QJsonObject &serialization) override {
        m_radius = serialization["m_radius"].toDouble();
        auto center = serialization["m_center"].toArray();
        for (int i = 0; i < m_center.size(); ++i) {
            m_center[i] = center[i].toDouble();
        }
    }

public:
    const gnomon::vec3_t center(void) const;
    double radius(void) const;
    void setCenter(gnomon::vec3_t);
    void setRadius(double);

private:
    gnomon::vec3_t m_center = {0., 0., 0.};
    double m_radius = 1.;
};

DTK_DECLARE_OBJECT(gnomonSphereForm *)

//
// gnomonSphereForm.h ends here
