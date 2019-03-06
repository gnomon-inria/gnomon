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
#include "gnomonAlgorithm/gnomonAbstractAlgorithm.h"

class gnomonCellImage;
class gnomonCellComplex;

class GNOMONCORE_EXPORT gnomonAbstractCellComplexFromCellImage : public gnomonAbstractAlgorithm
{

public:
    virtual void setParameter(const QString& parameterName, const QVariant& parameterValue) override = 0;
    virtual QMap<QString, gnomonCoreParameter *> parameters(void) const override = 0;
    virtual void run(void) override = 0;

    //Inputs
public:
    virtual void setInput(gnomonCellImage *cellimage) = 0;

    // Outputs
public:
    virtual gnomonCellImage *input(void) = 0;
    virtual gnomonCellComplex *output(void) const = 0;
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
