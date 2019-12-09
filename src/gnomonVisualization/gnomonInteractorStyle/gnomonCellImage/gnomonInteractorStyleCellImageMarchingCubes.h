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

#include <QtCore>

#include <gnomonVisualizationExport>

#include "gnomonInteractorStyle/gnomonInteractorStyle.h"

class gnomonAbstractVisualizationCellImage;

class GNOMONVISUALIZATION_EXPORT gnomonInteractorStyleCellImageMarchingCubes : public gnomonInteractorStyle
{

public:
     gnomonInteractorStyleCellImageMarchingCubes(void);
    ~gnomonInteractorStyleCellImageMarchingCubes(void);

public:
    virtual void OnMouseMove(void) override;
    virtual void OnLeftButtonDown(void) override;
    virtual void OnLeftButtonUp(void) override;

public:
    void OnDoubleClick(long vtkId);

public:
    virtual QMap<int, QString> keyMap(void) const override;

public slots:
    void updateTextActor(long vtkId);
    void setActor(vtkProp *actor);
    void setVisualization(gnomonAbstractVisualizationCellImage *visu);

public slots:
    virtual void disable(void) override;

public:
    virtual QString description(void) const override;
    virtual int icon(void) const override;

private:
    class gnomonInteractorStyleCellImageMarchingCubesPrivate *dd;
};

//
// gnomonInteractorStyleCellImageMarchingCubes.h ends here