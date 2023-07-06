// Version: $Id$
//
//

// Commentary:
//
//

// Change Log:Ce
//
//

// Code:

#pragma once

#include <QtCore>

#include <gnomonVisualizationExport.h>

#include "gnomonAbstractCellComplexVtkVisualization.h"

class gnomonVtkView;
class gnomonCellComplex;

class GNOMONVISUALIZATION_EXPORT gnomonCellComplexVtkVisualization : public gnomonAbstractCellComplexVtkVisualization
{
    Q_OBJECT

public:
     gnomonCellComplexVtkVisualization(void);
    ~gnomonCellComplexVtkVisualization(void);

public:
    virtual const QString pluginName(void) override;
    virtual const QString name(void) override;

public:
    void setCellComplex(std::shared_ptr<gnomonCellComplexSeries> cellComplex) override;
    std::shared_ptr<gnomonCellComplexSeries> cellComplex(void) override;

public:
    QImage imageRendering(void) override;

public slots:
    void update(void) override;
    void render(void) override;

public slots:
    void clear(void) override;
    void fill(void) override;
    virtual void setVisible(bool visible) override;

public slots:
    void on2D(void) override;
    void on3D(void) override;
    void onXY(void) override;
    void onXZ(void) override;
    void onYZ(void) override;
    void onSliceChanged(double) override;
    void onSliceOrientationChanged(int) override;
    void onTimeChanged(double) override;

public:
    void setParameter(const QString&, const QVariant&) override;
    void setParameters(const dtkCoreParameters&) override;
    dtkCoreParameters parameters(void) const override;
    QMap<QString, QString> parameterGroups(void) override;

private:
    class gnomonCellComplexVtkVisualizationPrivate *ddd;
};


inline gnomonAbstractCellComplexVtkVisualization *gnomonCellComplexVtkVisualizationCreator(void)
{
    return new gnomonCellComplexVtkVisualization();
}

//
// gnomonCellComplexVtkVisualization.h ends here
