#pragma once

#include <gnomonCoreExport.h>

#include "gnomonAbstractModel.h"

#include <QtCore>
#include <dtkCore>

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class gnomonAbstractDynamicForm;
class gnomonTime;

class GNOMONCORE_EXPORT gnomonExpansionSphereModel : public gnomonAbstractModel
{
public:
             gnomonExpansionSphereModel(void);
    virtual ~gnomonExpansionSphereModel(void);

public:
    void run(const gnomonTime&, unsigned long) override;

public:
    void setExpansionRate(double expansion_rate);
    double expansionRate(void);

private:
    class gnomonExpansionSphereModelPrivate *d;
};

typedef QSharedPointer<gnomonExpansionSphereModel> gnomonExpansionSphereModelPtr;

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonExpansionSphereModel *)

//
// gnomonExpansionSphereModel.h ends here
