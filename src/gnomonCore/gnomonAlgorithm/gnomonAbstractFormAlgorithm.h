#pragma once

#include <gnomonCoreExport>

#include <QtCore>
#include <dtkCore/dtkCorePlugin>

#include "gnomonAbstractAlgorithm.h"

#include "gnomonForm/gnomonBinaryImage/gnomonBinaryImage.h"
#include "gnomonForm/gnomonCellComplex/gnomonCellComplex.h"
#include "gnomonForm/gnomonCellImage/gnomonCellImage.h"
#include "gnomonForm/gnomonDataFrame/gnomonDataFrame.h"
#include "gnomonForm/gnomonImage/gnomonImage.h"
#include "gnomonForm/gnomonLString/gnomonLString.h"
#include "gnomonForm/gnomonMesh/gnomonMesh.h"
#include "gnomonForm/gnomonPointCloud/gnomonPointCloud.h"
#include "gnomonForm/gnomonTree/gnomonTree.h"

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONCORE_EXPORT gnomonAbstractFormAlgorithm : public gnomonAbstractAlgorithm
{
public:
             gnomonAbstractFormAlgorithm(void) = default;
    virtual ~gnomonAbstractFormAlgorithm(void) = default;
    
    // BinaryImage
public:
    virtual void setInputBinaryImage(std::shared_ptr<gnomonBinaryImageSeries> binaryImage) { return; };
    // set update argument to false to avoid re-generating gnomon classes in Python plugins
    virtual std::shared_ptr<gnomonBinaryImageSeries> inputBinaryImage(bool update=true) const { return nullptr; };
    virtual std::shared_ptr<gnomonBinaryImageSeries> outputBinaryImage(bool update=true) const { return nullptr; };

    
    // CellComplex
public:
    virtual void setInputCellComplex(std::shared_ptr<gnomonCellComplexSeries> cellComplex) { return; };
    // set update argument to false to avoid re-generating gnomon classes in Python plugins
    virtual std::shared_ptr<gnomonCellComplexSeries> inputCellComplex(bool update=true) const { return nullptr; };
    virtual std::shared_ptr<gnomonCellComplexSeries> outputCellComplex(bool update=true) const { return nullptr; };

    // CellImage
public:
    virtual void setInputCellImage(std::shared_ptr<gnomonCellImageSeries> cellImage) { return; };
    virtual std::shared_ptr<gnomonCellImageSeries> inputCellImage(bool update=true) const { return nullptr; };
    virtual std::shared_ptr<gnomonCellImageSeries> outputCellImage(bool update=true) const { return nullptr; };

    // DataFrame
public:
    virtual void setInputDataFrame(std::shared_ptr<gnomonDataFrameSeries> dataFrame) { return; };
    virtual std::shared_ptr<gnomonDataFrameSeries> inputDataFrame(bool update=true) const { return nullptr; };
    virtual std::shared_ptr<gnomonDataFrameSeries> outputDataFrame(bool update=true) const { return nullptr; };

    // Image
public:
    virtual void setInputImage(std::shared_ptr<gnomonImageSeries> image) { return; };
    virtual std::shared_ptr<gnomonImageSeries> inputImage(bool update=true) const { return nullptr; };
    virtual std::shared_ptr<gnomonImageSeries> outputImage(bool update=true) const { return nullptr; };

    // LString
public:
    virtual void setInputLString(std::shared_ptr<gnomonLStringSeries> lString) { return; };
    virtual std::shared_ptr<gnomonLStringSeries> inputLString(bool update=true) const { return nullptr; };
    virtual std::shared_ptr<gnomonLStringSeries> outputLString(bool update=true) const { return nullptr; };

    // Mesh
public:
    virtual void setInputMesh(std::shared_ptr<gnomonMeshSeries> mesh) { return; };
    virtual std::shared_ptr<gnomonMeshSeries> inputMesh(bool update=true) const { return nullptr; };
    virtual std::shared_ptr<gnomonMeshSeries> outputMesh(bool update=true) const { return nullptr; };

    // PointCloud
public:
    virtual void setInputPointCloud(std::shared_ptr<gnomonPointCloudSeries> pointCloud) { return; };
    virtual std::shared_ptr<gnomonPointCloudSeries> inputPointCloud(bool update=true) const { return nullptr; };
    virtual std::shared_ptr<gnomonPointCloudSeries> outputPointCloud(bool update=true) const { return nullptr; };

    // Tree
public:
    virtual void setInputTree(std::shared_ptr<gnomonTreeSeries> tree) { return; };
    virtual std::shared_ptr<gnomonTreeSeries> inputTree(bool update=true) const { return nullptr; };
    virtual std::shared_ptr<gnomonTreeSeries> outputTree(bool update=true) const { return nullptr; };

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
