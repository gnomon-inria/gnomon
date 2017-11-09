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
class gnomonCellComplex;

class GNOMONCORE_EXPORT gnomonAbstractCellComplexFromImage : public QRunnable
{

    //Inputs
public:
    virtual void setImage(dtkImage *image) = 0;

    virtual void setComplexDimension(int dimension) = 0;

    // Outputs
public:
    virtual gnomonCellComplex *computedComplex(void) const = 0;

    virtual gnomonCellComplex *adjacencyComplex(void) const = 0;

public:
    virtual void run(void) = 0;

};

DTK_DECLARE_OBJECT(gnomonAbstractCellComplexFromImage *)

DTK_DECLARE_PLUGIN(gnomonAbstractCellComplexFromImage, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractCellComplexFromImage, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractCellComplexFromImage, GNOMONCORE_EXPORT)


namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractCellComplexFromImage, GNOMONCORE_EXPORT, cellcomplexFromImage);
}

//
// gnomonAbstractCellComplexFromImage.h ends here
