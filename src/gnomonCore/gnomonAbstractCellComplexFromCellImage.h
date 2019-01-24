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

class gnomonCellImage;
class gnomonCellComplex;

class GNOMONCORE_EXPORT gnomonAbstractCellComplexFromCellImage : public QRunnable
{

    //Inputs
public:
    virtual void setCellImage(gnomonCellImage *cellImage) = 0;

    virtual void setComplexDimension(int dimension) = 0;

    // Outputs
public:
    virtual gnomonCellComplex *computedComplex(void) const = 0;

    virtual gnomonCellComplex *adjacencyComplex(void) const = 0;

public:
    virtual void run(void) = 0;

};

DTK_DECLARE_OBJECT(gnomonAbstractCellComplexFromCellImage *)

DTK_DECLARE_PLUGIN(gnomonAbstractCellComplexFromCellImage, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractCellComplexFromCellImage, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractCellComplexFromCellImage, GNOMONCORE_EXPORT)


namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractCellComplexFromCellImage, GNOMONCORE_EXPORT, cellComplexFromCellImage);
}

//
// gnomonAbstractCellComplexFromCellImage.h ends here
