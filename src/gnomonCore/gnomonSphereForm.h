#pragma once

#include <gnomonCoreExport.h>

#include "gnomonAbstractForm.h"

#include <QtCore>
#include <dtkCore>

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class gnomonAbstractForm;

class GNOMONCORE_EXPORT gnomonSphereForm : public gnomonAbstractForm
{
public:
             gnomonSphereForm(void);
    virtual ~gnomonSphereForm(void);

    gnomonSphereForm(const gnomonSphereForm& other) : m_radius(other.m_radius) {}
    gnomonAbstractForm *clone(void) { return new gnomonSphereForm(*this); }

public:
    double radius(void);
    void setRadius(double radius);

private:
    double m_radius = 1.;
};

typedef QSharedPointer<gnomonSphereForm> gnomonSphereFormPtr;

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonSphereForm *)

//
// gnomonSphereForm.h ends here
