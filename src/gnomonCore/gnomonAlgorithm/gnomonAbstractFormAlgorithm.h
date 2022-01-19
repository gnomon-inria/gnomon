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

#include "gnomonForm/gnomonBinaryImage/gnomonBinaryImage.h"
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
    virtual dtkCoreParameters parameters(void) const override = 0;
    
    // BinaryImage
public:
    virtual void setInputBinaryImage(gnomonBinaryImageSeries *binaryImage) { return; };
    // set update argument to false to avoid re-generating gnomon classes in Python plugins
    virtual gnomonBinaryImageSeries *inputBinaryImage(bool update=true) const { return nullptr; };
    virtual gnomonBinaryImageSeries *outputBinaryImage(bool update=true) const { return nullptr; };

    
    // CellComplex
public:
    virtual void setInputCellComplex(gnomonCellComplexSeries *cellComplex) { return; };
    // set update argument to false to avoid re-generating gnomon classes in Python plugins
    virtual gnomonCellComplexSeries *inputCellComplex(bool update=true) const { return nullptr; };
    virtual gnomonCellComplexSeries *outputCellComplex(bool update=true) const { return nullptr; };

    // CellImage
public:
    virtual void setInputCellImage(gnomonCellImageSeries *cellImage) { return; };
    virtual gnomonCellImageSeries *inputCellImage(bool update=true) const { return nullptr; };
    virtual gnomonCellImageSeries *outputCellImage(bool update=true) const { return nullptr; };

    // DataFrame
public:
    virtual void setInputDataFrame(gnomonDataFrameSeries *dataFrame) { return; };
    virtual gnomonDataFrameSeries *inputDataFrame(bool update=true) const { return nullptr; };
    virtual gnomonDataFrameSeries *outputDataFrame(bool update=true) const { return nullptr; };

    // Image
public:
    virtual void setInputImage(gnomonImageSeries *image) { return; };
    virtual gnomonImageSeries *inputImage(bool update=true) const { return nullptr; };
    virtual gnomonImageSeries *outputImage(bool update=true) const { return nullptr; };

    // LString
public:
    virtual void setInputLString(gnomonLStringSeries *lString) { return; };
    virtual gnomonLStringSeries *inputLString(bool update=true) const { return nullptr; };
    virtual gnomonLStringSeries *outputLString(bool update=true) const { return nullptr; };

    // Mesh
public:
    virtual void setInputMesh(gnomonMeshSeries *mesh) { return; };
    virtual gnomonMeshSeries *inputMesh(bool update=true) const { return nullptr; };
    virtual gnomonMeshSeries *outputMesh(bool update=true) const { return nullptr; };

    // PointCloud
public:
    virtual void setInputPointCloud(gnomonPointCloudSeries *pointCloud) { return; };
    virtual gnomonPointCloudSeries *inputPointCloud(bool update=true) const { return nullptr; };
    virtual gnomonPointCloudSeries *outputPointCloud(bool update=true) const { return nullptr; };

    // Tree
public:
    virtual void setInputTree(gnomonTreeSeries *tree) { return; };
    virtual gnomonTreeSeries *inputTree(bool update=true) const { return nullptr; };
    virtual gnomonTreeSeries *outputTree(bool update=true) const { return nullptr; };

public:
    static inline QString defaultSetter(QString formName) {
        return "setInput" +  formName.split("gnomon")[1];
    };
    static inline QString defaultGetter(QString formName) {
        return "input" +  formName.split("gnomon")[1];
    };
    static inline QString defaultOutput(QString formName) {
        return "output" +  formName.split("gnomon")[1];
    };

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
