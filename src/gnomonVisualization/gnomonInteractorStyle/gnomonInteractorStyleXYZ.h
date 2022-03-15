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

#include <gnomonVisualizationExport>

#include "gnomonInteractorStyle/gnomonInteractorStyle.h"

class gnomonViewForm;

class GNOMONVISUALIZATION_EXPORT gnomonInteractorStyleXYZ : public gnomonInteractorStyle
{
public:
     gnomonInteractorStyleXYZ(void);
    ~gnomonInteractorStyleXYZ(void);


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
    void setView(gnomonViewForm *) override;

public slots:
    void updateCamera(void);

public:
    virtual QMap<int, QString> keyMap(void) const override;

public:
    virtual QString description(void) const override;
    virtual int icon(void) const override;

protected:
	class gnomonInteractorStyleXYZPrivate *dd;
};

//
// gnomonInteractorStyleXYZ.h ends here
