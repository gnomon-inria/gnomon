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
    static gnomonImageManager *instance(void);

public:
    QSize sizeHint(void) const;

public slots:
    void addImage(Image);

public:
    Image get(int index);

protected:
     gnomonImageManager(QWidget *parent = nullptr);
    ~gnomonImageManager(void);

private:
    class gnomonImageManagerPrivate *d;

private:
    static gnomonImageManager *s_instance;
};

//
// gnomonImageManager.h ends here
