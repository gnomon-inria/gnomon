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

#include <QtCore>
#include <QtWidgets>

// #include <dtkFonts/dtkFontAwesome>

#include <gnomonVisualizationExport>

#include <vtkInteractorStyleTrackballCamera.h>

class gnomonViewForm;

class GNOMONVISUALIZATION_EXPORT gnomonInteractorStyle : public QObject, public vtkInteractorStyleTrackballCamera
{
public:
     gnomonInteractorStyle(void);
    ~gnomonInteractorStyle(void);

public:
    virtual QMap<int, QString> keyMap(void) const;

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
//    virtual void SetDefaultRenderer(vtkRenderer *) override;

public slots:
    virtual void setView(gnomonViewForm *);
    virtual void setMode(QString);

public slots:
    virtual void disable(void);

public:
    virtual QString description(void) const;
    virtual int icon(void) const;

protected:
	class gnomonInteractorStylePrivate *d;
};

//
// gnomonInteractorStyle.h ends here
