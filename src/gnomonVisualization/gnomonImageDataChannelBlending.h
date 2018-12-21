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

class GNOMONVISUALIZATION_EXPORT gnomonImageDataChannelBlending : public QObject, public vtkImageData
{
public:
    vtkTypeMacro(gnomonImageDataChannelBlending, vtkImageData);

    static gnomonImageDataChannelBlending *New(void);

public:
	void setImages(QMap<QString, vtkImageData *> images);

public slots:
    void update(void);
    void modified(void);

public slots:
    void setValueRange(const QList<int>& value);
    void setColorMap(const QMap<double,QColor>&);

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