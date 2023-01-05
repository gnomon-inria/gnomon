#pragma once

#include <QtCore>

#include <vtkInteractorStyleTrackballCamera.h>

#include <gnomonVisualizationExport>

class GNOMONVISUALIZATION_EXPORT gnomonInteractorStyleCellImagePicking : public QObject, public vtkInteractorStyleTrackballCamera
{
    Q_OBJECT

public:
     static gnomonInteractorStyleCellImagePicking* New();
     gnomonInteractorStyleCellImagePicking(void);
    ~gnomonInteractorStyleCellImagePicking(void);

public:
    virtual void OnLeftButtonDown(void) override;

 signals:
    void pickedCell(int);
};
