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

class GNOMONCORE_EXPORT gnomonWallContext : public gnomonAbstractModel
{
public:
             gnomonWallContext(void);
    virtual ~gnomonWallContext(void);

public:
    void run(const gnomonTime&, unsigned long) override;

public:
    void setTimeT1(const gnomonTime& t_1);
    void setWallPosition0(double coord, char axis);
    void setWallPosition1(double coord, char axis);

public:
    const gnomonTime& timeT1(void);
    char wallAxis();
    char wallCoord();

private:
    class gnomonWallContextPrivate *d;
};

typedef QSharedPointer<gnomonWallContext> gnomonWallContextPtr;

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonWallContext *)

//
// gnomonWallContext.h ends here
