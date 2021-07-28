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

class dtkCoreParameter;

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
    virtual QMap<QString, dtkCoreParameter *> parameters(void) const override = 0;

    // CellComplex
public:
    virtual void setInputCellComplex(gnomonCellComplexSeries *cellComplex) { return; };
    virtual gnomonCellComplexSeries *inputCellComplex() const { return nullptr; };
    virtual gnomonCellComplexSeries *outputCellComplex() const { return nullptr; };

    // CellImage
public:
    virtual void setInputCellImage(gnomonCellImageSeries *cellImage) { return; };
    virtual gnomonCellImageSeries *inputCellImage() const { return nullptr; };
    virtual gnomonCellImageSeries *outputCellImage() const { return nullptr; };

    // DataFrame
public:
    virtual void setInputDataFrame(gnomonDataFrameSeries *dataFrame) { return; };
    virtual gnomonDataFrameSeries *inputDataFrame() const { return nullptr; };
    virtual gnomonDataFrameSeries *outputDataFrame() const { return nullptr; };

    // Image
public:
    virtual void setInputImage(gnomonImageSeries *image) { return; };
    virtual gnomonImageSeries *inputImage() const { return nullptr; };
    virtual gnomonImageSeries *outputImage() const { return nullptr; };

    // LString
public:
    virtual void setInputLString(gnomonLStringSeries *lString) { return; };
    virtual gnomonLStringSeries *inputLString() const { return nullptr; };
    virtual gnomonLStringSeries *outputLString() const { return nullptr; };

    // Mesh
public:
    virtual void setInputMesh(gnomonMeshSeries *mesh) { return; };
    virtual gnomonMeshSeries *inputMesh() const { return nullptr; };
    virtual gnomonMeshSeries *outputMesh() const { return nullptr; };

    // PointCloud
public:
    virtual void setInputPointCloud(gnomonPointCloudSeries *pointCloud) { return; };
    virtual gnomonPointCloudSeries *inputPointCloud() const { return nullptr; };
    virtual gnomonPointCloudSeries *outputPointCloud() const { return nullptr; };

    // Tree
public:
    virtual void setInputTree(gnomonTreeSeries *tree) { return; };
    virtual gnomonTreeSeries *inputTree() const { return nullptr; };
    virtual gnomonTreeSeries *outputTree() const { return nullptr; };

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
