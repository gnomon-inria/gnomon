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

#include <gnomonCoreExport>

#include <QtCore>
#include <dtkCore>

#include "gnomonAlgorithm/gnomonAbstractAlgorithm.h"

#include "gnomonForm/gnomonCellComplex/gnomonCellComplex.h"
#include "gnomonForm/gnomonCellImage/gnomonCellImage.h"
#include "gnomonForm/gnomonDataFrame/gnomonDataFrame.h"
#include "gnomonForm/gnomonImage/gnomonImage.h"
#include "gnomonForm/gnomonLString/gnomonLString.h"
#include "gnomonForm/gnomonMesh/gnomonMesh.h"
#include "gnomonForm/gnomonPointCloud/gnomonPointCloud.h"
#include "gnomonForm/gnomonTree/gnomonTree.h"

class gnomonCoreParameter;

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonAbstractFormAlgorithm : public gnomonAbstractAlgorithm
{
public:
             gnomonAbstractFormAlgorithm(void) = default;
    virtual ~gnomonAbstractFormAlgorithm(void) = default;

public:
    virtual void setParameter(const QString& parameterName, const QVariant& parameterValue) override = 0;
    virtual QMap<QString, gnomonCoreParameter *> parameters(void) const override = 0;

    // CellComplex
public:
    virtual void setInputCellComplex(gnomonCellComplexSeries *cellComplex) = 0;
    virtual gnomonCellComplexSeries *inputCellComplex() const = 0;
    virtual gnomonCellComplexSeries *outputCellComplex() const = 0;

    // CellImage
public:
    virtual void setInputCellImage(gnomonCellImageSeries *cellImage) = 0;
    virtual gnomonCellImageSeries *inputCellImage() const = 0;
    virtual gnomonCellImageSeries *outputCellImage() const = 0;
    
    // DataFrame
public:
    virtual void setInputDataFrame(gnomonDataFrameSeries *dataFrame) = 0;
    virtual gnomonDataFrameSeries *inputDataFrame() const = 0;
    virtual gnomonDataFrameSeries *outputDataFrame() const = 0;
    
    // Image
public:
    virtual void setInputImage(gnomonImageSeries *image) = 0;
    virtual gnomonImageSeries *inputImage() const = 0;
    virtual gnomonImageSeries *outputImage() const = 0;
    
    // LString
public:
    virtual void setInputLString(gnomonLStringSeries *lString) = 0;
    virtual gnomonLStringSeries *inputLString() const = 0;
    virtual gnomonLStringSeries *outputLString() const = 0;
    
    // Mesh
public:
    virtual void setInputMesh(gnomonMeshSeries *mesh) = 0;
    virtual gnomonMeshSeries *inputMesh() const = 0;
    virtual gnomonMeshSeries *outputMesh() const = 0;
    
    // PointCloud
public:
    virtual void setInputPointCloud(gnomonPointCloudSeries *pointCloud) = 0;
    virtual gnomonPointCloudSeries *inputPointCloud() const = 0;
    virtual gnomonPointCloudSeries *outputPointCloud() const = 0;
    
    // Tree
public:
    virtual void setInputTree(gnomonTreeSeries *tree) = 0;
    virtual gnomonTreeSeries *inputTree() const = 0;
    virtual gnomonTreeSeries *outputTree() const = 0;
    
public:
    virtual void run(void) override = 0;
    virtual QString documentation(void) override = 0;
};

// ///////////////////////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonAbstractFormAlgorithm *)
DTK_DECLARE_PLUGIN        (gnomonAbstractFormAlgorithm, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_FACTORY(gnomonAbstractFormAlgorithm, GNOMONCORE_EXPORT)
DTK_DECLARE_PLUGIN_MANAGER(gnomonAbstractFormAlgorithm, GNOMONCORE_EXPORT)

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

namespace gnomonCore {
    DTK_DECLARE_CONCEPT(gnomonAbstractFormAlgorithm, GNOMONCORE_EXPORT, formAlgorithm);
}

//
// gnomonAbstractFormAlgorithm.h ends here
