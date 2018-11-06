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

#include <vtkImageData.h>
#include <vtkSmartPointer.h>

class gnomonImageManager : public QFrame
{
    Q_OBJECT

public:
    typedef vtkSmartPointer<vtkImageData> Image;

public:
     gnomonImageManager(QWidget *parent = nullptr);
    ~gnomonImageManager(void);

public:
    QSize sizeHint(void) const;

public slots:
    void addImage(Image);

private:
    class gnomonImageManagerPrivate *d;
};

//
// gnomonImageManager.h ends here
