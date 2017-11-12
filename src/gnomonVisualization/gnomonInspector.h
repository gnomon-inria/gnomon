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

#include <gnomonVisualizationExport.h>

#include <vtkAssembly.h>

class GNOMONVISUALIZATION_EXPORT gnomonInspector : public QFrame, public vtkAssembly
{
public:
             gnomonInspector(QWidget *parent = Q_NULLPTR);
    virtual ~gnomonInspector(void);

public:
    virtual bool isVisible(void);

public slots:
    virtual void hide(void);
    virtual void show(void);

};

//
// gnomonInspector.h ends here
