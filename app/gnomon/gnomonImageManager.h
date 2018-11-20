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
using dtkImagePtr = QSharedPointer<dtkImage>;

class gnomonImageManager : public QFrame
{
    Q_OBJECT

public:
    static gnomonImageManager *instance(void);

public:
    QSize sizeHint(void) const;

public slots:
    void addImage(dtkImagePtr, const QColor&);

public:
    dtkImagePtr get(int index);

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
