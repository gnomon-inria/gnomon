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

#include <gnomonVisualizationExport>

#include <vtkInteractorStyleTrackballCamera.h>

class gnomonViewForm;

class GNOMONVISUALIZATION_EXPORT gnomonInteractorStyle : public QObject, public vtkInteractorStyleTrackballCamera
{
public:
     gnomonInteractorStyle(void);
    ~gnomonInteractorStyle(void);

public:
    QMap<QShortcut *, QString> keyMap(void) const;

public:
    virtual void OnMouseMove(void) override;

public:
    virtual void OnKeyDown(void) override;
    virtual void OnKeyPress(void) override;
    virtual void OnKeyUp(void) override;
    virtual void OnKeyRelease(void) override;

public:
    virtual void OnChar(void) override;

public slots:
    void setView(gnomonViewForm *);

protected:
	class gnomonInteractorStylePrivate *d;
};

//
// gnomonInteractorStyle.h ends here
