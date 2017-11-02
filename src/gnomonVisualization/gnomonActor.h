// Version: $Id: 71d7118592ad295c11438516a544baf94d12696c $
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

#include <gnomonVisualizationExport.h>

#include <vtkAssembly.h>

class GNOMONVISUALIZATION_EXPORT gnomonActor : public QObject, public vtkAssembly
{
public:
    virtual ~gnomonActor(void);

public:
    virtual bool isVisible(void);

public:
    virtual void setInteractor(void *interactor) = 0;

public slots:
    virtual void hide(void);
    virtual void show(void);

public slots:
    virtual void update(void) = 0;
};

//
// gnomonActor.h ends here
