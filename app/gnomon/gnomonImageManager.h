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

class dtkImage;

class gnomonImageManager : public QFrame
{
    Q_OBJECT

public:
    using Image = dtkImage *;

public:
    static gnomonImageManager *instance(void);

public:
    QSize sizeHint(void) const;

public slots:
    void addImage(Image, const QColor&);

public:
    Image get(int index);

public:
    QPixmap thumbnail(int index);

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
