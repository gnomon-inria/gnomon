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

class gnomonActorVolume;

class gnomonClutEditor;

class gnomonInspectorVolumePrivate;

class GNOMONVISUALIZATION_EXPORT gnomonInspectorVolume : public QWidget
{
public:
     gnomonInspectorVolume(QWidget *parent = Q_NULLPTR);
    ~gnomonInspectorVolume(void);

 public:
    void setActor(gnomonActorVolume *actor) const;

 public:
    gnomonClutEditor *editor(void) const;

private:
    gnomonInspectorVolume(const gnomonInspectorVolume&) = delete;
    void operator = (const gnomonInspectorVolume&) = delete;

 private:
    class gnomonInspectorVolumePrivate *d;
};

//
// gnomonInspectorVolume.h ends here
