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

namespace gnomonForm {

    void registerForms(void)
    {
        qRegisterMetaType<gnomonBinaryImageSeries>();
        qRegisterMetaType<gnomonCellComplexSeries>();
        qRegisterMetaType<gnomonCellGraphSeries>();
        qRegisterMetaType<gnomonCellImageSeries>();
        qRegisterMetaType<gnomonDataDictSeries>();
        qRegisterMetaType<gnomonDataFrameSeries>();
        qRegisterMetaType<gnomonImageSeries>();
        //qRegisterMetaType<gnomonIntensityImageSeries>();
        qRegisterMetaType<gnomonLStringSeries>();
        qRegisterMetaType<gnomonMeshSeries>();
        qRegisterMetaType<gnomonPointCloudSeries>();
        qRegisterMetaType<gnomonTreeSeries>();
    }

    std::shared_ptr<gnomonAbstractDynamicForm> createDynamicForm(const QString &type) {
        void *myClassPtr = nullptr;
        QMetaType metatype = QMetaType::fromName(type.toUtf8());

        if (metatype.isValid()) {
            myClassPtr = metatype.create();
        } else {
            // try as a time series
            QString type_series = "gnomonTimeSeries<" + type + ">";
            metatype = QMetaType::fromName(type_series.toUtf8());
            if (metatype.isValid()) {
                myClassPtr = metatype.create();
            }
        }
         
        if (myClassPtr) {
            return std::shared_ptr<gnomonAbstractDynamicForm>(static_cast<gnomonAbstractDynamicForm*>(myClassPtr));
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

} // namespace gnomonForm