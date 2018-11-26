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

class gnomonImageManagerFocus : public QLabel
{
    Q_OBJECT

public:
     gnomonImageManagerFocus(QWidget *parent = nullptr);
    ~gnomonImageManagerFocus(void);

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
// gnomonImageManagerFocus.h ends here
