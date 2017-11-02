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

#include <tissueVisualizationExport.h>

#include <vtkAssembly.h>

class TISSUEVISUALIZATION_EXPORT tissueActor : public QObject, public vtkAssembly
{
public:
    virtual ~tissueActor(void);

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
// tissueActor.h ends here
