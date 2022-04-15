#pragma once

#include <gnomonCoreExport.h> // enables the visibility of the concept

#include <QtCore>

#include <dtkCore>

class gnomonCellComplex; // must forward define this useful class to enable proper compilation.

//  ///////////////////////////////////////////////////////////////////
//  Definition of the abstract class for our gnomon growth models
//  ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonAbstractTissueGrowthModel : public QRunnable
{

public: // setters definition
	virtual void setCellComplex(gnomonCellComplex *gnomon) = 0;

public: // getters defintion
	virtual gnomonCellComplex *grownCellComplex(void) const = 0;

public: //run from QVariant makes sure that the implementation will be performed.
	virtual void run(void) = 0;

};

//  ///////////////////////////////////////////////////////////////////
//  Adding the dtk macros for the visibility of the abstraction
//  ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT 		  (gnomonAbstractTissueGrowthModel *)


DTK_DECLARE_PLUGIN    	  (gnomonAbstractTissueGrowthModel, GNOMONCORE_EXPORT) 		  // The following lines 
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractTissueGrowthModel, GNOMONCORE_EXPORT)  // are not needed right now 
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractTissueGrowthModel, GNOMONCORE_EXPORT)  // for we won't define plugins first

//  ///////////////////////////////////////////////////////////////////
//  Register gnomonAbstractTissueGrowthModel to the layer
//  ///////////////////////////////////////////////////////////////////

namespace gnomonCore {
	DTK_DECLARE_CONCEPT(gnomonAbstractTissueGrowthModel, GNOMONCORE_EXPORT, growthmodel)
}

//
// gnomonAbstractTissueGrowthModel.h ends here
