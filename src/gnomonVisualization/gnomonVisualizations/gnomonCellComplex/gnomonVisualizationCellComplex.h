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

#include "gnomonAbstractVisualizationCellComplex.h"

class gnomonViewForm;

class gnomonCellComplex;
class gnomonCoreParameter;

class GNOMONVISUALIZATION_EXPORT gnomonVisualizationCellComplex : public gnomonAbstractVisualizationCellComplex
{
    Q_OBJECT

public:
     gnomonVisualizationCellComplex(void);
    ~gnomonVisualizationCellComplex(void);

public:
	void setCellComplex(gnomonCellComplex *cellComplex) override;

public:
    QImage imageRendering(void) override;

public slots:
    void update(void) override;
    void render(void) override;

public slots:
    void clear(void);

public slots:
    void onTimeChanged(double) override;

public:
    void setParameter(const QString&, const QVariant&) override;
    void setParameters(const QMap<QString, gnomonCoreParameter *>&) override;
    QMap<QString, gnomonCoreParameter *> parameters(void) const override;

private:
	class gnomonVisualizationCellComplexPrivate *dd;
};


inline gnomonAbstractVisualizationCellComplex *gnomonVisualizationCellComplexCreator(void)
{
    return new gnomonVisualizationCellComplex();
}

//
// gnomonVisualizationCellComplex.h ends here