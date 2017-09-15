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

#include <tissueCoreExport.h> // enables the visibility of the concept

#include <QtCore>

#include <dtkCore>

class tissueCellComplex; // must forward define this useful class to enable proper compilation.

//  ///////////////////////////////////////////////////////////////////
//  Definition of the abstract class for our tissue growth models
//  ///////////////////////////////////////////////////////////////////

class TISSUECORE_EXPORT tissueAbstractTissueGrowthModel : public QRunnable
{

public: // setters definition
	virtual void setCellComplex(tissueCellComplex *tissue) = 0;

public: // getters defintion
	virtual tissueCellComplex *grownCellComplex(void) const = 0;

public: //run from QVariant makes sure that the implementation will be performed.
	virtual void run(void) = 0;

};

//  ///////////////////////////////////////////////////////////////////
//  Adding the dtk macros for the visibility of the abstraction
//  ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT 		  (tissueAbstractTissueGrowthModel *)


DTK_DECLARE_PLUGIN    	  (tissueAbstractTissueGrowthModel, TISSUECORE_EXPORT) 		  // The following lines 
DTK_DECLARE_PLUGIN_FACTORY(tissueAbstractTissueGrowthModel, TISSUECORE_EXPORT)  // are not needed right now 
DTK_DECLARE_PLUGIN_MANAGER(tissueAbstractTissueGrowthModel, TISSUECORE_EXPORT)  // for we won't define plugins first

//  ///////////////////////////////////////////////////////////////////
//  Register tissueAbstractTissueGrowthModel to the layer
//  ///////////////////////////////////////////////////////////////////

namespace tissueCore {
	DTK_DECLARE_CONCEPT(tissueAbstractTissueGrowthModel, TISSUECORE_EXPORT, growthmodel)
}

//
// tissueAbstractTissueGrowthModel.h ends here
