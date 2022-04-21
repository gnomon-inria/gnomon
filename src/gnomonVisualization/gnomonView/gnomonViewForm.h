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

#include <QtCore>
#include <QtQml>
#include <QtGui>

#include <gnomonLandmark.h>

class dtkWidgetsMenu;
class dtkWidgetsMenuBar;

class gnomonAbstractForm;
class gnomonAbstractDynamicForm;
class gnomonAbstractVisualization;

class gnomonInteractorStyle;

struct gnomonLandmark;

template <typename T> class gnomonTimeSeries;

#include <gnomonCore/gnomonForm/gnomonBinaryImage/gnomonBinaryImage.h>
#include <gnomonCore/gnomonForm/gnomonCellComplex/gnomonCellComplex.h>
#include <gnomonCore/gnomonForm/gnomonCellImage/gnomonCellImage.h>
#include <gnomonCore/gnomonForm/gnomonImage/gnomonImage.h>
#include <gnomonCore/gnomonForm/gnomonMesh/gnomonMesh.h>
#include <gnomonCore/gnomonForm/gnomonPointCloud/gnomonPointCloud.h>

class vtkCamera;
class vtkRenderer;
class vtkRenderWindowInteractor;
class vtkGenericOpenGLRenderWindow;

class GNOMONVISUALIZATION_EXPORT gnomonViewForm : public QObject
{
    Q_OBJECT
public:
     gnomonViewForm(QObject *parent = nullptr);
    ~gnomonViewForm(void);

public:
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
    Q_PROPERTY(QStringList formNames READ formNames NOTIFY formsChanged);
    Q_PROPERTY(QStringList acceptedForms READ acceptedForms);
    Q_PROPERTY(bool inputView READ inputView WRITE setInputView NOTIFY inputViewChanged);
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

    Q_ENUM(Mode);
    Q_ENUM(Orientation);
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
    void badFormDropped(QString badFormName, QString acceptedForms);

    void syncedChanged(void);
    void syncingChanged(void);
    void inputViewChanged(void);

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
    void tryLinking(void);

    void   link(gnomonViewForm *other);
    void unlink(gnomonViewForm *other);
    void disconnectTime();

public slots:
    void drop(int);

public slots:
    void setExportColor(const QColor& color);

public:
    void setForm(const QString&, gnomonAbstractDynamicForm *,const QJsonObject &visualization={});
    void setBinaryImage(gnomonBinaryImageSeries *, const QJsonObject &visu_properties={});
    void setCellComplex(gnomonCellComplexSeries *, const QJsonObject &visu_properties={});
    void setCellImage(gnomonCellImageSeries *,const QJsonObject &visu_properties={});
    void setImage(gnomonImageSeries *, const QJsonObject &visu_properties={});
    void setMesh(gnomonMeshSeries *, const QJsonObject &visu_properties={});
    void setPointCloud(gnomonPointCloudSeries *, const QJsonObject &visu_properties={});

public:
    void setAdaptedForm(const QString&, gnomonAbstractDynamicForm *, gnomonAbstractVisualization * = nullptr);

public:
    QStringList formNames(void);
    QStringList acceptedForms(void);
    bool inputView(void);
    bool synced(void);
    bool syncing(void);
    bool inPool(void);

signals:
    void formsChanged(void);
    void formVisuParametersChanged(void);


public:
    gnomonAbstractDynamicForm *form(const QString&);
    gnomonBinaryImageSeries *binaryImage(void);
    gnomonCellComplexSeries *cellComplex(void);
    gnomonCellImageSeries *cellImage(void);
    gnomonImageSeries *image(void);
    gnomonMeshSeries *mesh(void);
    gnomonPointCloudSeries *pointCloud(void);

public:
    Q_INVOKABLE QString formVisuName(const QString& name);
    Q_INVOKABLE QStringList formVisualizations(const QString& name);
    Q_INVOKABLE void setFormVisuName(const QString& name, const QString& visu_name);
    Q_INVOKABLE QJSValue formVisuParameters(const QString& name);

    Q_INVOKABLE void setFormVisible(const QString& name, bool visible);
    Q_INVOKABLE void removeForm(const QString& name);

    Q_INVOKABLE gnomonDynamicFormMetadata* formMetadata(const QString& name);


public:
    vtkRenderer *renderer2D(void);
    vtkRenderer *renderer3D(void);

// public:
//     dtkWidgetsMenu *menu(void);
//     dtkWidgetsMenuBar *menubar(void);

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
    void render(void);
    void update(void);
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
    void timeMaxChanged(double);
    void timesChanged(void);

public:
    QList<double> times(void);

public slots:
    void setCurrentTime(double);

public slots:
    void onTimeChanged(double);

public slots:
    void setInputView(bool);
    void setInPool(bool);

public slots:
    void setEnableMenus(bool);

public slots:
    void setInteractorStyle(gnomonInteractorStyle *);
    void updateShortcutKeys(void);

public slots:
    void transmit(void);

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

//Q_DECLARE_METATYPE(gnomonViewForm *)
//
// gnomonViewForm.h ends here
