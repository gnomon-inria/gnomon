#include "gnomonSphereForm.h"

gnomonSphereForm::gnomonSphereForm(void)
{
//    this->record();
}

gnomonSphereForm::gnomonSphereForm(const gnomonSphereForm& other)
{
    this->setCenter(other.m_center);
    this->setRadius(other.m_radius);
}

gnomonAbstractForm *gnomonSphereForm::clone(void)
{
    gnomonSphereForm *newForm = new gnomonSphereForm();
    newForm->setCenter(m_center);
    newForm->setRadius(m_radius);
    return newForm;
}

QString gnomonSphereForm::name(void) const
{
    return "gnomonSphereForm";
}



const gnomon::vec3_t gnomonSphereForm::center(void) const
{
    return m_center;
}

void gnomonSphereForm::setCenter(gnomon::vec3_t center)
{
    this->m_center = center;
}


double gnomonSphereForm::radius(void) const
{
    return m_radius;
}

void gnomonSphereForm::setRadius(double radius)
{
    this->m_radius = radius;
}
