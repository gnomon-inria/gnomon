#include "gnomonSphereForm.h"


const double *gnomonSphereForm::center(void) const
{
    return m_center;
}

void gnomonSphereForm::setCenter(double *center)
{
    this->m_center[0] = center[0];
    this->m_center[1] = center[1];
    this->m_center[2] = center[2];
}


double gnomonSphereForm::radius(void) const
{
    return m_radius;
}

void gnomonSphereForm::setRadius(double radius)
{
    this->m_radius = radius;
}
