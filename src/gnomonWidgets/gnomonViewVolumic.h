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
