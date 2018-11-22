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

class gnomonLandmark;

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
    void onSliceChanged(int);
    void onChannelChanged(const QString&);
    std::size_t addLandmark(std::size_t, double, double, double);
    void removeLandmark(std::size_t);

signals:
    void channelsChanged(QStringList);
    void landmarkAdded(std::size_t, double, double, double);
    void landmarkRemoved(std::size_t id);

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
