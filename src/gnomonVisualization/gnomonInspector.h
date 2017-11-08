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

#include <gnomonVisualizationExport.h>

#include <vtkAssembly.h>

class GNOMONVISUALIZATION_EXPORT gnomonInspector : public QObject, public vtkAssembly
{
public:
    virtual ~gnomonInspector(void);

public:
    virtual bool isVisible(void);

public slots:
    virtual void hide(void);
    virtual void show(void);

public slots:
    virtual void update(void) = 0;
};

//
// gnomonInspector.h ends here
