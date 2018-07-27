#pragma once

#include <gnomonCoreExport.h>

#include "gnomonAbstractForm.h"
#include "gnomonTypeDef.h"

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
    void setNormal(gnomon::vec3_t);
    void setPosition(gnomon::vec3_t);

public:
    QString name(void) const override ;
    const gnomon::vec3_t normal(void) const;
    const gnomon::vec3_t position(void) const;


private:
    class gnomonWallFormPrivate *d;
};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT (gnomonWallForm *)

//
// gnomonWallContext.h ends here
