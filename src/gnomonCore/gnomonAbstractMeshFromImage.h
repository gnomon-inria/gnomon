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

#include <gnomonCoreExport.h> // enables the visibility of the concept

#include <dtkCore>

class dtkImage;
class gnomonMesh;

class GNOMONCORE_EXPORT gnomonAbstractMeshFromImage : public QRunnable
{

    //Inputs
public:
    virtual void setImage(dtkImage *image) = 0;

    virtual void setComplexDimension(int dimension) = 0;

    // Outputs
public:
    virtual gnomonMesh *computedComplex(void) const = 0;

    virtual gnomonMesh *adjacencyComplex(void) const = 0;

public:
    virtual void run(void) = 0;

};

DTK_DECLARE_OBJECT(gnomonAbstractMeshFromImage *)

DTK_DECLARE_PLUGIN(gnomonAbstractMeshFromImage, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractMeshFromImage, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractMeshFromImage, GNOMONCORE_EXPORT)


namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractMeshFromImage, GNOMONCORE_EXPORT, MeshFromImage);
}

//
// gnomonAbstractMeshFromImage.h ends here
