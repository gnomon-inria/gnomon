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

#include <QtWidgets>

#include <vtkColorTransferFunction.h>
#include <vtkImageData.h>
#include <vtkSmartPointer.h>

class gnomonViewVolumic : public QFrame
{
    Q_OBJECT

public:
     gnomonViewVolumic(QWidget *parent = nullptr);
    ~gnomonViewVolumic(void);

public:
    void setImage(vtkSmartPointer<vtkImageData>);

public slots:
    void onSliceChanged(int);

private:
    class gnomonViewVolumicPrivate *d;
};

//
// gnomonViewVolumic.h ends here
