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

#include <gnomonCore/gnomonImagesSerie>

#include <QtWidgets>

class dtkImage;
using dtkImagePtr = dtkImage*;

using gnomonImagesSeriePtr = QSharedPointer<gnomonImagesSerie>;

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
    void setImagesSerie(gnomonImagesSeriePtr);
    void setImage(dtkImage*);

public:
    dtkImagePtr image(void);
    gnomonImagesSeriePtr imagesSerie(void);

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
