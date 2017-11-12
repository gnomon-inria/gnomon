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

#include "gnomonInspectorImage.h"

#include <gnomonVisualizationExport.h>

class gnomonClutEditor;

class GNOMONVISUALIZATION_EXPORT gnomonInspectorSlicePlanes : public gnomonInspectorImage
{
    Q_OBJECT

public:
     gnomonInspectorSlicePlanes(QWidget *parent = Q_NULLPTR);
    ~gnomonInspectorSlicePlanes(void);

private:
    gnomonInspectorSlicePlanes(const gnomonInspectorSlicePlanes&) = delete;
    void operator = (const gnomonInspectorSlicePlanes&) = delete;

public:
    double xPlaneOpacity(void);
    double yPlaneOpacity(void);
    double zPlaneOpacity(void);

    void setXPlaneOpacity(double);
    void setYPlaneOpacity(double);
    void setZPlaneOpacity(double);

public:
    double xPlanePos(void);
    double yPlanePos(void);
    double zPlanePos(void);

    void setXPlanePos(double);
    void setYPlanePos(double);
    void setZPlanePos(double);

signals:
    void xPlanePosChanged(void);
    void yPlanePosChanged(void);
    void zPlanePosChanged(void);

    void xPlaneOpacityChanged(void);
    void yPlaneOpacityChanged(void);
    void zPlaneOpacityChanged(void);

private:
    class gnomonInspectorSlicePlanesPrivate *d;
};

//
// gnomonInspectorSlicePlanes.h ends here
