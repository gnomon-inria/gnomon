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

class gnomonAbstractFormVisualization;
//class gnomonInteractorStyle;

class vtkCamera;
class vtkRenderer;
class vtkRenderWindowInteractor;
class vtkGenericOpenGLRenderWindow;

class GNOMONVISUALIZATION_EXPORT gnomonViewForm : public gnomonAbstractView
{
    Q_OBJECT
public:
     gnomonViewForm(QObject *parent = nullptr);
    ~gnomonViewForm(void);

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
    Q_PROPERTY(bool inPool READ inPool WRITE setInPool NOTIFY inPoolChanged);
    Q_PROPERTY(double currentTime READ currentTime WRITE setCurrentTime NOTIFY timeChanged);
    Q_PROPERTY(double timeMax READ timeMax NOTIFY timeMaxChanged);
    Q_PROPERTY(QList<double> times READ times NOTIFY timesChanged);
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
    void inPoolChanged(void);

    void syncedChanged(void);
    void syncingChanged(void);

signals:
    void   linking(void);
    void unlinking(void);

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

    void   link(gnomonViewForm *other);
    void unlink(gnomonViewForm *other);
    void disconnectTime();

public:
    void setForm(const QString&, std::shared_ptr<gnomonAbstractDynamicForm> ,std::shared_ptr<gnomonAbstractFormVisualization> visualization=nullptr);
    void setBinaryImage(std::shared_ptr<gnomonBinaryImageSeries> , std::shared_ptr<gnomonAbstractFormVisualization> visualization=nullptr);
    void setCellComplex(std::shared_ptr<gnomonCellComplexSeries> , std::shared_ptr<gnomonAbstractFormVisualization> visualization=nullptr);
    void setLString(std::shared_ptr<gnomonLStringSeries> , std::shared_ptr<gnomonAbstractFormVisualization> visualization=nullptr);
    void setMesh(std::shared_ptr<gnomonMeshSeries> , std::shared_ptr<gnomonAbstractFormVisualization> visualization=nullptr);
    void setPointCloud(std::shared_ptr<gnomonPointCloudSeries> , std::shared_ptr<gnomonAbstractFormVisualization> visualization=nullptr);

public:
    void setAdaptedForm(const QString&, std::shared_ptr<gnomonAbstractDynamicForm>);

    bool synced(void);
    bool syncing(void);
    bool inPool(void);

signals:
    void formVisuParametersChanged(void);
    void formVisualizationChanged(void);


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
    Q_INVOKABLE QString formVisuName(const QString& name);
    Q_INVOKABLE QVariantList formVisualizations(const QString& name);
    //Q_INVOKABLE QList<gnomonPluginData *> formVisualizations(const QString& name);
    Q_INVOKABLE void setFormVisuName(const QString& name, const QString& visu_name);
    Q_INVOKABLE QJSValue formVisuParameters(const QString& name);
    Q_INVOKABLE QVariant formVisuParameter(const QString& name, const QString& parameter_name);
    Q_INVOKABLE void setFormVisuParameter(const QString& name, const QString& parameter_name, const QVariant& value);

    Q_INVOKABLE void setFormVisible(const QString& name, bool visible);

    Q_INVOKABLE gnomonDynamicFormMetadata* formMetadata(const QString& name);

public:
    vtkRenderer *renderer2D(void);
    vtkRenderer *renderer3D(void);

public:
    vtkRenderWindowInteractor *interactor(void);

public:
    Orientation orientation(void);

public slots:
    void setBounds(double bounds[6]);
    void setBounds(double xMin, double xMax, double yMin, double yMax, double zMin, double zMax);

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

public:
    void setCamera(vtkCamera *);

public slots:
    void render(void) override;
    void update(void) override;
    void clear(void) override;

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

public:
    Q_INVOKABLE void notifyFormSelected(int index, QString formType);
    Q_INVOKABLE int lastFormIndexSelected();
    Q_INVOKABLE QString lastFromTypeSelected();
    Q_INVOKABLE QString lastVisuSelected(QString formType);

public slots:
    void updateShortcutKeys(void);

public slots:
    void restoreState(void);

private:
    class gnomonViewFormPrivate *dd;
};

//Q_DECLARE_METATYPE(gnomonViewForm *)
//
// gnomonViewForm.h ends here
