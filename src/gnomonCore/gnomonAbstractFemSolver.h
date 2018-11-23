// Version: $Id$
//
//

// Commentary:
//
//

// Change Log:
//
//

// Code:

#pragma once

#include <dtkCore>

#include <gnomonCoreExport>

class gnomonMesh;
class gnomonCoreParameter;

//  ///////////////////////////////////////////////////////////////////
//
//  ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonAbstractFemSolver : public QRunnable
{
public:
    virtual void setParameter(const QString&, const QVariant&) = 0;
	virtual void setMesh(gnomonMesh *mesh) = 0;

public:
    virtual gnomonMesh *updatedMesh(void) const = 0;

    virtual QMap<QString, gnomonCoreParameter *> parameters(void) const = 0;

public:
    virtual void run(void) = 0;
};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonAbstractFemSolver *)
DTK_DECLARE_PLUGIN        (gnomonAbstractFemSolver, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractFemSolver, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractFemSolver, GNOMONCORE_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractFemSolver, GNOMONCORE_EXPORT, femSolver);
}

//
// gnomonAbstractFemSolver.h ends here
