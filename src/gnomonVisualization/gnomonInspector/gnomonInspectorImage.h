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

#include <gnomonVisualizationExport.h>

#include <QWidget>

class gnomonActorImage;

class gnomonInspectorImagePrivate;

class GNOMONVISUALIZATION_EXPORT gnomonInspectorImage : public QWidget
{
public:
     gnomonInspectorImage(QWidget *parent = Q_NULLPTR);
    ~gnomonInspectorImage(void);

 public:
    void setActor(gnomonActorImage *actor) const;

private:
    gnomonInspectorImage(const gnomonInspectorImage&) = delete;
    void operator = (const gnomonInspectorImage&) = delete;

 private:
    class gnomonInspectorImagePrivate *d;
};

//
// gnomonInspectorImage.h ends here
