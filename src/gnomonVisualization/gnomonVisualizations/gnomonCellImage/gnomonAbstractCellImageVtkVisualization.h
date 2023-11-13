#pragma once

#include <gnomonVisualizationExport.h>
#include "gnomonCore/gnomonCorePlugin.h"

#include <gnomonCore/gnomonForm/gnomonCellImage/gnomonCellImage.h>
#include "gnomonVisualizations/gnomonAbstractVtkVisualization.h"

class GNOMONVISUALIZATION_EXPORT gnomonAbstractCellImageVtkVisualization : public gnomonAbstractVtkVisualization
{
    Q_OBJECT

public:
    gnomonAbstractCellImageVtkVisualization(void) : gnomonAbstractVtkVisualization() {}
    virtual ~gnomonAbstractCellImageVtkVisualization(void) = default;

public:
    virtual void setCellImage(std::shared_ptr<gnomonCellImageSeries> cellImage) = 0;
    virtual std::shared_ptr<gnomonCellImageSeries> cellImage(void) = 0;

    static inline QString defaultSetter(QString formName) {
        if(formName == "gnomonCellImage") {
            return {"setCellImage"};
        }
        return {};
    };
    static inline QString defaultGetter(QString formName) {
        if(formName == "gnomonCellImage") {
            return {"cellImage"};
        }
        return {};
    };

    virtual void stopPicking() { qDebug() << Q_FUNC_INFO << "not implemented"; };

signals:
    void pickedCells(QList<long>);
};

// /////////////////    //////////////////////////////////////////////////
// Give the concept the plugin machinery
// ///////////////////////////////////////////////////////////////////

DTK_DECLARE_OBJECT        (gnomonAbstractCellImageVtkVisualization *)
DTK_DECLARE_PLUGIN        (gnomonAbstractCellImageVtkVisualization, GNOMONVISUALIZATION_EXPORT)
GNOMON_DECLARE_PLUGIN_FACTORY(gnomonAbstractCellImageVtkVisualization, GNOMONVISUALIZATION_EXPORT, cellImageVtkVisualization, gnomonVisualization)


//
// gnomonAbstractCellImageVtkVisualization.h ends here
