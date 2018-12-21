// Version: $Id$
//
//

// Commentary:
//
//

// Change Log:Ce
//
//

// Code:

#pragma once

#include <vtkObject.h>
#include <QtCore>

#include <gnomonVisualizationExport.h>

#include <vtkImageData.h>

class gnomonLookupTable;

class GNOMONVISUALIZATION_EXPORT gnomonImageDataChannelBlending : public QObject, public vtkImageData
{
public:
    vtkTypeMacro(gnomonImageDataChannelBlending, vtkImageData);

    static gnomonImageDataChannelBlending *New(void);

public:
	void setImageChannels(QMap<QString, vtkImageData *> imageChannels);
    void setChannelLookupTables(const QMap<QString, gnomonLookupTable *>&);

public slots:
    void update(void);
    void modified(void);

protected:
     gnomonImageDataChannelBlending(void);
    ~gnomonImageDataChannelBlending(void);

private:
    gnomonImageDataChannelBlending(const gnomonImageDataChannelBlending&);
    void operator = (const gnomonImageDataChannelBlending&);

private:
	class gnomonImageDataChannelBlendingPrivate *d;
};


//
// gnomonImageDataChannelBlending.h ends here