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

class dtkWidgetsMenu;
class dtkWidgetsMenuBar;

class gnomonAbstractForm;
class gnomonAbstractDynamicForm;
class gnomonAbstractVisualization;

class gnomonInteractorStyle;

struct gnomonLandmark;

template <typename T> class gnomonTimeSeries;

#include <gnomonCore/gnomonForm/gnomonCellComplex/gnomonCellComplex.h>
#include <gnomonCore/gnomonForm/gnomonCellImage/gnomonCellImage.h>
#include <gnomonCore/gnomonForm/gnomonImage/gnomonImage.h>
#include <gnomonCore/gnomonForm/gnomonMesh/gnomonMesh.h>
#include <gnomonCore/gnomonForm/gnomonPointCloud/gnomonPointCloud.h>

class vtkCamera;
class vtkRenderer;
class vtkRenderWindowInteractor;

class GNOMONVISUALIZATION_EXPORT gnomonViewForm : public QFrame
{
    Q_OBJECT

public:
     gnomonViewForm(QWidget *parent = nullptr);
    ~gnomonViewForm(void);

signals:
    void updated(void);

signals:
    void switchedTo3D  (void);
    void switchedTo2D  (void);
    void switchedTo2DXY(void);
    void switchedTo2DXZ(void);
    void switchedTo2DYZ(void);

signals:
    void   linking(void);
    void unlinking(void);

signals:
    void exportedForm(gnomonAbstractDynamicForm *);

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

public:
    void setForm(const QString&, gnomonAbstractDynamicForm *, gnomonAbstractVisualization *  = nullptr);
    void setCellImage(gnomonCellImageSeries *, gnomonAbstractVisualization *  = nullptr);
    void setImage(gnomonImageSeries *, gnomonAbstractVisualization *  = nullptr);
    void setCellComplex(gnomonCellComplexSeries *, gnomonAbstractVisualization *  = nullptr);
    void setMesh(gnomonMeshSeries *, gnomonAbstractVisualization *  = nullptr);
    void setPointCloud(gnomonPointCloudSeries *, gnomonAbstractVisualization *  = nullptr);

public:
    void setAdaptedForm(const QString&, gnomonAbstractDynamicForm *, gnomonAbstractVisualization * = nullptr);

public:
    gnomonAbstractDynamicForm *form(const QString&);
    gnomonImageSeries *image(void);
    gnomonCellImageSeries *cellImage(void);
    gnomonCellComplexSeries *cellComplex(void);
    gnomonMeshSeries *mesh(void);
    gnomonPointCloudSeries *pointCloud(void);

public:
    vtkRenderer *renderer2D(void);
    vtkRenderer *renderer3D(void);

// public:
//     dtkWidgetsMenu *menu(void);
//     dtkWidgetsMenuBar *menubar(void);

public:
    vtkRenderWindowInteractor *interactor(void);

public:
    int orientation(void);

public slots:
    void setBounds(double bounds[6]);
    void setBounds(double xMin, double xMax, double yMin, double yMax, double zMin, double zMax);

    void getBounds(double bounds[6]);

public:
    void setCamera(vtkCamera *);

public slots:
    void render(void);
    void clear(void);

public slots:
    void setAcceptForm(const QString&, bool);

public slots:
    void setEnableLinking(bool);

public slots:
    void onSliceChanged(int);

public slots:
    void sliceChange(int);

signals:
    void sliceOrientationChanged(int);
    void sliceChanged(int);

signals:
    void formAdded(const QString&);

signals:
    void timeChanged(double);

public:
    QList<double> times(void);

public slots:
    void timeIndexChange(int);

public slots:
    void onTimeChanged(double);

public slots:
    void setInputView(bool);

public slots:
    void setEnableMenus(bool);

public slots:
    void setInteractorStyle(gnomonInteractorStyle *);
    void updateShortcutKeys(void);

// protected:
//     void dragEnterEvent(QDragEnterEvent *);
//     void dragLeaveEvent(QDragLeaveEvent *);
//     void dragMoveEvent(QDragMoveEvent *);
//     void dropEvent(QDropEvent *);

// signals:
//     void fileDropped(const QString&);

// protected:
//     void resizeEvent(QResizeEvent *);

private:
    class gnomonViewFormPrivate *d;
};

//
// gnomonViewForm.h ends here
