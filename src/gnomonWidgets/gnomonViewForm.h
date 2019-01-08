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

#include <gnomonLandmark.h>

class gnomonLandmark;

class gnomonAbstractForm;

class gnomonImagesSerie;
class gnomonCellImage;
class gnomonMesh;

class vtkRenderer;
class vtkRenderWindowInteractor;

class GNOMONWIDGETS_EXPORT gnomonViewForm : public QFrame
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
    void sliceChange(int);

public slots:
    void setExportColor(const QColor& color);

public:
    void setForm(const QString&, gnomonAbstractForm *);
    void setCellImage(gnomonCellImage*);
    void setImagesSerie(gnomonImagesSerie*);
    void setMesh(gnomonMesh*);

public:
    gnomonAbstractForm *form(const QString&); 
    gnomonImagesSerie *imagesSerie(void);
    gnomonCellImage *cellImage(void);
    gnomonMesh *mesh(void);

public:
    vtkRenderer *renderer2D(void);
    vtkRenderer *renderer3D(void);

public:
    vtkRenderWindowInteractor *interactor(void);

public:
    int orientation(void);

public slots:
    void setBounds(double bounds[6]);

public slots:
    void render(void);

public slots:
    void onSliceChanged(int);

signals:
    void sliceOrientationChanged(int);
    void sliceChanged(int);

    void formAdded(void);

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
