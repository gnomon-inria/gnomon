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

class gnomonFormManagerFocus : public QLabel
{
    Q_OBJECT

public:
     gnomonFormManagerFocus(QWidget *parent = nullptr);
    ~gnomonFormManagerFocus(void);

public:
    QPoint source;
    QPoint destnt;

public:
    QSize s_size;
    QSize d_size;

public:
    bool presented = false;
};

//
// gnomonFormManagerFocus.h ends here
