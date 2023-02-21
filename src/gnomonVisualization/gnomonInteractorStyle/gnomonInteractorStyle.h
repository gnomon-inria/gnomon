#pragma once

#include <QtCore>
#include <gnomonVisualizationExport>

#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkRenderWindowInteractor.h>

class gnomonViewForm;

class GNOMONVISUALIZATION_EXPORT gnomonInteractorStyle : public QObject, public vtkInteractorStyleTrackballCamera
{
    Q_OBJECT

public:
     gnomonInteractorStyle(void);
    ~gnomonInteractorStyle(void);

public:
    virtual void OnMouseMove(void) override;
    virtual void OnLeftButtonDown(void) override;
    virtual void OnLeftButtonUp(void) override;

public:
    virtual void OnKeyDown(void) override;
    virtual void OnKeyPress(void) override;
    virtual void OnKeyUp(void) override;
    virtual void OnKeyRelease(void) override;

public:
    virtual void OnChar(void) override;

public:
    virtual vtkRenderer *GetDefaultRenderer(void) override;
    //virtual void SetDefaultRenderer(vtkRenderer *) override;

public slots:
    virtual void setView(gnomonViewForm *);
    virtual void setMode(QString);

public slots:
    virtual void disable(void);

public:
    virtual QMap<int, QString> keyMap(void) const;
    virtual QString description(void) const;

protected:
    class gnomonInteractorStylePrivate *d;
};

//
// gnomonInteractorStyle.h ends here
