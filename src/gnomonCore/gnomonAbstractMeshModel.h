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

#include <QtCore>
#include <dtkCore>
#include <gnomonCore>

#include <gnomonCoreExport.h>



//  ///////////////////////////////////////////////////////////////////
//  
//  ///////////////////////////////////////////////////////////////////

class gnomonMesh;


class GNOMONCORE_EXPORT gnomonAbstractMeshModel : public QRunnable
{

// model Inputs
public:
    virtual void setParameter(const QString& parameterName, const QVariant& parameterValue) = 0; //gnomonParameter: a tuple (parameter_name, parameter_value)
    virtual void setMesh(gnomonMesh *mesh)     = 0;

// model Output
public:
    virtual gnomonMesh *updatedMesh(void) const = 0;
    virtual const QMap<QString, QVariant>& parameters(void) const = 0;
    virtual       QMap<QString, QVariant>& parameters(void) = 0;
    
public:
    virtual void run(void) = 0;
    
};





// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT(gnomonAbstractMeshModel *)

DTK_DECLARE_PLUGIN(gnomonAbstractMeshModel, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractMeshModel, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractMeshModel, GNOMONCORE_EXPORT)


// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

namespace gnomon {
    DTK_DECLARE_CONCEPT(gnomonAbstractMeshModel, GNOMONCORE_EXPORT, meshModel);
}





// dtkAbstractWatershedFilter.h ends here