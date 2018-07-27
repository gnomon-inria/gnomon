#pragma once

#include <gnomonCoreExport.h>

#include "gnomonAbstractForm.h"

#include <dtkCore>

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonWallForm : public gnomonAbstractForm
{

public:
    gnomonWallForm(void);
    ~gnomonWallForm(void);

public:
    void setNormal(double *);
    void setPosition(double *);

public:
    QString name(void) const override ;
    const double *normal(void) const;
    const double *position(void) const;


private:
    class gnomonWallFormPrivate *d;
};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT (gnomonWallForm *)

//
// gnomonWallContext.h ends here
