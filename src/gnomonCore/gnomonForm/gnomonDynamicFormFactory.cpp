#include "gnomonDynamicFormFactory.h"

#include "gnomonBinaryImage/gnomonBinaryImage.h"
#include "gnomonCellComplex/gnomonCellComplex.h"
#include "gnomonCellImage/gnomonCellImage.h"
#include "gnomonCellGraph/gnomonCellGraph.h"
#include "gnomonDataDict/gnomonDataDict.h"
#include "gnomonDataFrame/gnomonDataFrame.h"
#include "gnomonImage/gnomonImage.h"
#include "gnomonLString/gnomonLString.h"
#include "gnomonMesh/gnomonMesh.h"
#include "gnomonPointCloud/gnomonPointCloud.h"
#include "gnomonTree/gnomonTree.h"

#include <exception>

std::shared_ptr<gnomonAbstractDynamicForm> createDynamicForm(const QString &type) {
    if(type == gnomonBinaryImage::formName()) {

        return std::make_shared<gnomonBinaryImageSeries>();

    } else if(type == gnomonCellComplex::formName()) {

        return std::make_shared<gnomonCellComplexSeries>();

    } else if(type == gnomonCellImage::formName()) {

        return std::make_shared<gnomonCellImageSeries>();

    } else if(type == gnomonCellGraph::formName()) {

        return std::make_shared<gnomonCellGraphSeries>();

    } else if(type == gnomonDataDict::formName()) {

        return std::make_shared<gnomonDataDictSeries>();

    } else if(type == gnomonDataFrame::formName()) {

        return std::make_shared<gnomonDataFrameSeries>();

    } else if(type == gnomonImage::formName()) {

        return std::make_shared<gnomonImageSeries>();

    } else if(type == gnomonLString::formName()) {

        return std::make_shared<gnomonLStringSeries>();

    } else if(type == gnomonMesh::formName()) {

        return std::make_shared<gnomonMeshSeries>();

    } else if(type == gnomonPointCloud::formName()) {

        return std::make_shared<gnomonPointCloudSeries>();

    } else if(type == gnomonTree::formName()) {

        return std::make_shared<gnomonTreeSeries>();

    }

    qCritical() << Q_FUNC_INFO << "Could not create dynamic form of type " << type;
    throw std::invalid_argument(type.toStdString());
}

std::shared_ptr<gnomonAbstractDynamicForm> createDynamicForm(const QJsonObject &serialization) {
    QString type = serialization["form_type"].toString();
    auto formSeries = createDynamicForm(type);
    formSeries->deserialize(serialization);
    return formSeries;
}
