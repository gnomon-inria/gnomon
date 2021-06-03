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
    ~gnomonSphereForm(void) = default;

public:
    QString name(void) const override;
    QMap<QString,QString> metadata(void) const override { return QMap<QString,QString>(); }
    QString dataName(void) const override { return "internal";}

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
