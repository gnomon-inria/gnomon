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

#include "gnomonAbstractForm.h"

#include "gnomonCore.h"

#include "gnomonCellComplex/gnomonCellComplex.h"
#include "gnomonCellGraph/gnomonCellGraph.h"
#include "gnomonCellImage/gnomonCellImage.h"
#include "gnomonDataDict/gnomonDataDict.h"
#include "gnomonDataFrame/gnomonDataFrame.h"
#include "gnomonImage/gnomonImage.h"
#include "gnomonLString/gnomonLString.h"
#include "gnomonMesh/gnomonMesh.h"
#include "gnomonPointCloud/gnomonPointCloud.h"
#include "gnomonTree/gnomonTree.h"
#include "gnomonSphereForm.h"

//void gnomonAbstractForm::record(void)
//{
//    static int counter = 0;
//    dtkObjectManager::instance()->add(dtkMetaType::variantFromValue(this),
//                                      QString("%1_%2").arg(this->name()).arg(counter++));
//}

gnomonCellComplex *gnomonAbstractForm::asCellComplex()
{ return dynamic_cast<gnomonCellComplex *>(this); }

gnomonCellGraph *gnomonAbstractForm::asCellGraph()
{ return dynamic_cast<gnomonCellGraph *>(this); }

gnomonCellImage *gnomonAbstractForm::asCellImage()
{ return dynamic_cast<gnomonCellImage *>(this); }

gnomonDataDict *gnomonAbstractForm::asDataDict()
{ return dynamic_cast<gnomonDataDict *>(this); }

gnomonDataFrame *gnomonAbstractForm::asDataFrame()
{ return dynamic_cast<gnomonDataFrame *>(this); }

gnomonImage *gnomonAbstractForm::asImage()
{ return dynamic_cast<gnomonImage *>(this); }

gnomonLString *gnomonAbstractForm::asLString()
{ return dynamic_cast<gnomonLString *>(this); }

gnomonMesh *gnomonAbstractForm::asMesh()
{ return dynamic_cast<gnomonMesh *>(this); }

gnomonPointCloud *gnomonAbstractForm::asPointCloud()
{ return dynamic_cast<gnomonPointCloud *>(this); }

gnomonSphereForm *gnomonAbstractForm::asSphere()
{ return dynamic_cast<gnomonSphereForm *>(this); }

gnomonTree *gnomonAbstractForm::asTree()
{ return dynamic_cast<gnomonTree *>(this); }


namespace gnomonCore {
    DTK_DEFINE_CONCEPT(gnomonAbstractForm, abstractForm, gnomonCore);
}

//
// gnomonAbstractForm.cpp ends here
