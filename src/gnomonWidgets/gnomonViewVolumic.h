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

#include <gnomonWidgetsExport>

#include <QtWidgets>

class dtkImage;
using dtkImagePtr = QSharedPointer<dtkImage>;

class vtkRenderer;
class vtkRenderWindowInteractor;

class GNOMONWIDGETS_EXPORT gnomonViewVolumic : public QFrame
{
    Q_OBJECT

public:
     gnomonViewVolumic(QWidget *parent = nullptr);
    ~gnomonViewVolumic(void);

signals:
    void   linking(void);
    void unlinking(void);

signals:
    void switchedTo2DXY(void);
    void switchedTo2DXZ(void);
    void switchedTo2DYZ(void);

signals:
    void sliceChanged(int);

public slots:
    void   link(gnomonViewVolumic *other);
    void unlink(gnomonViewVolumic *other);

public slots:
    void switchTo2DXY(void);
    void switchTo2DXZ(void);
    void switchTo2DYZ(void);

public slots:
    void sliceChange(int);

public:
    void setBlending(bool);
    void setImage(dtkImagePtr);

public:
    dtkImagePtr image(void);

public:
    vtkRenderer *renderer2D(void);
    vtkRenderer *renderer3D(void);

public:
    vtkRenderWindowInteractor *interactor(void);

public slots:
    void render(void);

public slots:
    void applyLut(double, double, double, double, double, double);
    void onSliceChanged(int);
    void onChannelChanged(const QString&, double lut_hue_min, double lut_hue_max,
                                         double lut_sat_min, double lut_sat_max,
                                         double lut_val_min, double lut_val_max);

signals:
    void channelsChanged(QStringList);

protected:
    void dragEnterEvent(QDragEnterEvent *);
    void dragLeaveEvent(QDragLeaveEvent *);
    void dragMoveEvent(QDragMoveEvent *);
    void dropEvent(QDropEvent *);

private:
    class gnomonViewVolumicPrivate *d;
};

//
// gnomonViewVolumic.h ends here
