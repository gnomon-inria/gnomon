#pragma once

#include <gnomonVisualizationExport>

#include <gnomonCore/gnomonForm/gnomonAbstractDynamicForm>
#include <gnomonCore/gnomonForm/gnomonBinaryImage/gnomonBinaryImage.h>
#include <gnomonCore/gnomonForm/gnomonCellComplex/gnomonCellComplex.h>
#include <gnomonCore/gnomonForm/gnomonCellImage/gnomonCellImage.h>
#include <gnomonCore/gnomonForm/gnomonImage/gnomonImage.h>
#include <gnomonCore/gnomonForm/gnomonLString/gnomonLString.h>
#include <gnomonCore/gnomonForm/gnomonMesh/gnomonMesh.h>
#include <gnomonCore/gnomonForm/gnomonPointCloud/gnomonPointCloud.h>

#include "gnomonAbstractView.h"

class gnomonAbstractVtkVisualization;
//class gnomonInteractorStyle;

class vtkCamera;
class vtkRenderer;
class vtkRenderWindowInteractor;
class vtkGenericOpenGLRenderWindow;

class GNOMONVISUALIZATION_EXPORT gnomonVtkView : public gnomonAbstractView
{
    Q_OBJECT
public:
     gnomonVtkView(QObject *parent = nullptr);
    ~gnomonVtkView(void);

public:
    Q_ENUMS(Mode);
    Q_ENUMS(Orientation);

    enum Mode {
        VIEW_MODE_3D = 3,
        VIEW_MODE_2D = 2,
    };

    enum Orientation {
        SLICE_ORIENTATION_XY = 2,
        SLICE_ORIENTATION_XZ = 1,
        SLICE_ORIENTATION_YZ = 0,
        NONE = -1
    };

    enum Representation {
        VTK_REPRESENTATION_POINTS = 0,
        VTK_REPRESENTATION_WIREFRAME  = 1,
        VTK_REPRESENTATION_SURFACE = 2,
    };

    enum Grid {
        GRID_CUBE = 0,
        GRID_PLANES = 1,
    };

public:
    Q_PROPERTY(bool synced READ synced NOTIFY syncedChanged);
    Q_PROPERTY(bool syncing READ syncing NOTIFY syncingChanged);

    Q_PROPERTY(double xMin READ xMin NOTIFY boundsChanged);
    Q_PROPERTY(double xMax READ xMax NOTIFY boundsChanged);
    Q_PROPERTY(double yMin READ xMin NOTIFY boundsChanged);
    Q_PROPERTY(double yMax READ yMax NOTIFY boundsChanged);
    Q_PROPERTY(double zMin READ zMin NOTIFY boundsChanged);
    Q_PROPERTY(double zMax READ zMax NOTIFY boundsChanged);
    Q_PROPERTY(Mode mode READ mode NOTIFY modeChanged);
    Q_PROPERTY(Orientation orientation READ orientation NOTIFY orientationChanged);
    Q_PROPERTY(Representation representation READ representation WRITE setRepresentation NOTIFY representationChanged)
    Q_PROPERTY(bool inPool READ inPool WRITE setInPool NOTIFY inPoolChanged);
    Q_PROPERTY(double currentTime READ currentTime WRITE setCurrentTime NOTIFY timeChanged);
    Q_PROPERTY(double timeMax READ timeMax NOTIFY timeMaxChanged);
    Q_PROPERTY(QList<double> times READ times NOTIFY timesChanged);

    Q_PROPERTY(QColor bgColor READ bgColor WRITE setBgColor NOTIFY bgColorChanged);
    Q_PROPERTY(bool gridVisible READ gridVisible WRITE setGridVisible NOTIFY gridVisibleChanged);
    Q_PROPERTY(Grid gridType READ gridType WRITE setGridType NOTIFY gridTypeChanged)
    Q_PROPERTY(Orientation gridOrientation READ gridOrientation WRITE setGridOrientation NOTIFY gridOrientationChanged);
    Q_PROPERTY(bool axesVisible READ axesVisible WRITE setAxesVisible NOTIFY axesVisibleChanged);
    Q_PROPERTY(bool cameraFixed READ cameraFixed WRITE setCameraFixed NOTIFY cameraFixedChanged);

    Q_PROPERTY(QList<long> pickedCells READ pickedCells NOTIFY pickedCellsChanged)

    Q_INVOKABLE void startPicking();
    Q_INVOKABLE void stopPicking();

// /////////////////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////////////////
    void associate(vtkGenericOpenGLRenderWindow *);
// /////////////////////////////////////////////////////////////////////////////

signals:
    void updated(void);

signals:
    void switchedTo3D  (void);
    void switchedTo2D  (void);
    void switchedTo2DXY(void);
    void switchedTo2DXZ(void);
    void switchedTo2DYZ(void);

signals:
    void boundsChanged(void);
    void modeChanged(void);
    void orientationChanged(void);
    void representationChanged(void);
    void inPoolChanged(void);

    void syncedChanged(void);
    void syncingChanged(void);

signals:
    void   linking(void);
    void unlinking(void);

signals:
    void bgColorChanged(void);
    void gridVisibleChanged(void);
    void gridTypeChanged(void);
    void gridOrientationChanged(void);
    void axesVisibleChanged(void);
    void cameraFixedChanged(void);

signals:
    void pickedCellsChanged();

 public:
    QList<long> pickedCells();
    void setPickedCells(QList<long>);

public slots:
    void switchTo3D  (void);
    void switchTo2D  (void);
    void switchTo2DXY(void);
    void switchTo2DXZ(void);
    void switchTo2DYZ(void);

public slots:
    void tryLinking(void);
    void setAcceptForm(const QString&, bool) override;

    void   link(gnomonVtkView *other);
    void unlink(gnomonVtkView *other);
    void disconnectTime();

public:
    void setAdaptedForm(const QString&, std::shared_ptr<gnomonAbstractDynamicForm>);

    bool synced(void);
    bool syncing(void);
    bool inPool(void);

public:
    std::shared_ptr<gnomonBinaryImageSeries> binaryImage(void);
    std::shared_ptr<gnomonCellComplexSeries> cellComplex(void);
    std::shared_ptr<gnomonCellImageSeries> cellImage(void);
    std::shared_ptr<gnomonImageSeries> image(void);
    std::shared_ptr<gnomonLStringSeries> lString(void);
    std::shared_ptr<gnomonMeshSeries> mesh(void);
    std::shared_ptr<gnomonPointCloudSeries> pointCloud(void);

public slots:
    void drop(int) override;
    void removeForm(const QString& name) override;

public:
    vtkRenderer *renderer2D(void);
    vtkRenderer *renderer3D(void);

public:
    vtkRenderWindowInteractor *interactor(void);

public:
    Orientation orientation(void);
    Representation representation(void);

public slots:
    void setRepresentation(Representation representation);

    void setBounds(double bounds[6]);
    void setBounds(double xMin, double xMax, double yMin, double yMax, double zMin, double zMax);
    void updateBounds(void);

    void getBounds(double bounds[6]);
    double xMin(void) const;
    double xMax(void) const;
    double yMin(void) const;
    double yMax(void) const;
    double zMin(void) const;
    double zMax(void) const;
    Mode mode(void) const;
    double currentTime(void) const;
    double timeMax(void);

    void setBgColor(const QColor& color);
    const QColor& bgColor(void);

    void setGridVisible(bool visible);
    bool gridVisible(void);

    void setGridType(Grid type);
    Grid gridType(void);

    void setGridOrientation(Orientation orientation);
    Orientation gridOrientation(void);

    void setAxesVisible(bool visible);
    bool axesVisible(void);

    void setCameraFixed(bool fixed);
    bool cameraFixed(void);

public slots:
    void setCameraXY(bool flip=false, bool turn=false);
    void setCameraXZ(bool flip=false, bool turn=false);
    void setCameraYZ(bool flip=false, bool turn=false);

public:
    void setCamera(vtkCamera *);
    void resetCamera(void);

public slots:
    void render(void) override;
    void clear(void) override;
    void saveScreenshot(const QString& filename) override;
    QImage toImage(void) override;

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
    void timeChanged(double);
    void timeMaxChanged(double);
    void timesChanged(void);

public:
    QList<double> times(void);

public slots:
    void setCurrentTime(double);

public slots:
    void onTimeChanged(double);

public slots:
    void setInPool(bool);

public slots:
    void setEnableMenus(bool);

public slots:
    void updateShortcutKeys(void);

private:
    class gnomonVtkViewPrivate *dd;
};

//Q_DECLARE_METATYPE(gnomonVtkView *)
//
// gnomonVtkView.h ends here
