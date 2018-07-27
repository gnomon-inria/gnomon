#pragma once

#include <gnomonCoreExport.h>

#include "gnomonAbstractForm.h"

#include <dtkCore>

class gnomonAbstractForm;

class GNOMONCORE_EXPORT gnomonSphereForm : public gnomonAbstractForm
{

public:
    gnomonSphereForm(void);
    ~gnomonSphereForm(void) = default;

public:
    QString name(void) const override;
    void render(void) override;

public:
    const double *center(void) const;
    double radius(void) const;
    void setCenter(double *);
    void setRadius(double);

private:
    double m_center[3] = {0., 0., 0.};
    double m_radius = 1.;
};

DTK_DECLARE_OBJECT(gnomonSphereForm *)

//
// gnomonSphereForm.h ends here
