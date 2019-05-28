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

#include <gnomonVisualizationExport>

#include <QtWidgets>

#include <gnomonLandmark.h>

class gnomonLandmark;
class gnomonOverlayPane;

class gnomonAbstractForm;
class gnomonAbstractVisualization;

template <typename T>
class gnomonTimeSeries;

class gnomonImagesSerie;
#include <gnomonCore/gnomonForm/gnomonCellComplex/gnomonCellComplex.h>
#include <gnomonCore/gnomonForm/gnomonCellImage/gnomonCellImage.h>
#include <gnomonCore/gnomonForm/gnomonMesh/gnomonMesh.h>
#include <gnomonCore/gnomonForm/gnomonPointCloud/gnomonPointCloud.h>

class vtkRenderer;
class vtkRenderWindowInteractor;

class GNOMONVISUALIZATION_EXPORT gnomonViewForm : public QFrame
{
    Q_OBJECT

public:
     gnomonViewForm(QWidget *parent = nullptr);
    ~gnomonViewForm(void);


signals:
    void switchedTo3D  (void);
    void switchedTo2D  (void);
    void switchedTo2DXY(void);
    void switchedTo2DXZ(void);
    void switchedTo2DYZ(void);

signals:
    void   linking(void);
    void unlinking(void);

public slots:
    void switchTo3D  (void);
    void switchTo2D  (void);
    void switchTo2DXY(void);
    void switchTo2DXZ(void);
    void switchTo2DYZ(void);

public slots:
    void   link(gnomonViewForm *other);
    void unlink(gnomonViewForm *other);

public slots:
    void setExportColor(const QColor& color);

public slots:
    void toggleVisualizationPane(void);

public:
    void setForm(const QString&, gnomonAbstractDynamicForm *, gnomonAbstractVisualization *  = nullptr);
    void setCellImage(gnomonCellImageSeries *, gnomonAbstractVisualization *  = nullptr);
    void setImagesSerie(gnomonImagesSerie *, gnomonAbstractVisualization *  = nullptr);
    void setCellComplex(gnomonCellComplexSeries *, gnomonAbstractVisualization *  = nullptr);
    void setMesh(gnomonMeshSeries *, gnomonAbstractVisualization *  = nullptr);
    void setPointCloud(gnomonPointCloudSeries *, gnomonAbstractVisualization *  = nullptr);

public:
    gnomonAbstractForm *form(const QString&);
    gnomonImagesSerie *imagesSerie(void);
    gnomonCellImageSeries *cellImage(void);
    gnomonCellComplexSeries *cellComplex(void);
    gnomonMesh *mesh(void);
    gnomonPointCloud *pointCloud(void);

public:
    vtkRenderer *renderer2D(void);
    vtkRenderer *renderer3D(void);

public:
    gnomonOverlayPane *infoPane(void);

public:
    vtkRenderWindowInteractor *interactor(void);

public:
    int orientation(void);

public slots:
    void setBounds(double bounds[6]);

public slots:
    void render(void);

public slots:
    void setAcceptCellComplex(bool);


public slots:
    void onSliceChanged(int);

public slots:
    void sliceChange(int);

signals:
    void sliceOrientationChanged(int);
    void sliceChanged(int);

    void formAdded(const QString&);

signals:
    void timeChanged(double);

public slots:
    void timeIndexChange(int);

public slots:
    void onTimeChanged(double);

protected:
    void dragEnterEvent(QDragEnterEvent *);
    void dragLeaveEvent(QDragLeaveEvent *);
    void dragMoveEvent(QDragMoveEvent *);
    void dropEvent(QDropEvent *);

private:
    class gnomonViewFormPrivate *d;
};

//
// gnomonViewForm.h ends here
