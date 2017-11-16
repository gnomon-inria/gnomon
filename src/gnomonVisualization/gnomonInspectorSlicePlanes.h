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
class gnomonActorImage;

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
    void setActor(gnomonActorImage *actor_image);

public:
    double xPlaneOpacity(void) const;
    double yPlaneOpacity(void) const;
    double zPlaneOpacity(void) const;

    void setXPlaneOpacity(double);
    void setYPlaneOpacity(double);
    void setZPlaneOpacity(double);

public:
    double xPlanePos(void) const;
    double yPlanePos(void) const;
    double zPlanePos(void) const;

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
