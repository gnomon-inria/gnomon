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
class gnomonImagesSerie;
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
    void switchedTo3D  (void);
    void switchedTo2D  (void);
    void switchedTo2DXY(void);
    void switchedTo2DXZ(void);
    void switchedTo2DYZ(void);

signals:
    void sliceChanged(int);

signals:
    void timeChanged(int);

public slots:
    void   link(gnomonViewVolumic *other);
    void unlink(gnomonViewVolumic *other);

public slots:
    void switchTo3D  (void);
    void switchTo2D  (void);
    void switchTo2DXY(void);
    void switchTo2DXZ(void);
    void switchTo2DYZ(void);

public slots:
    void sliceChange(int);

public slots:
    void timeChange(int);

public:
    void setImage(dtkImage*, const QMap<double, QColor>& = QMap<double, QColor>());
    void setBlending(bool);
    void setImagesSerie(gnomonImagesSeriePtr, const QMap<double, QColor>& = QMap<double, QColor>());

public:
    dtkImage* image(void);
    gnomonImagesSeriePtr imagesSerie(void);

public:
    vtkRenderer *renderer2D(void);
    vtkRenderer *renderer3D(void);

public:
    vtkRenderWindowInteractor *interactor(void);

public slots:
    void render(void);

public slots:
    void applyLut(const QMap<double, QColor>&);
    void onSliceChanged(int);
    void onTimeChanged(int);
    void onChannelChanged(const QString&, const QMap<double, QColor>&);

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
