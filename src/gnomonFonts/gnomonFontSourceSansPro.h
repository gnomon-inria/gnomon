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
#include <QtGui>

class gnomonFontSourceSansPro : public QObject
{
    Q_OBJECT

public:
     gnomonFontSourceSansPro(QObject *parent = 0);
    ~gnomonFontSourceSansPro(void) = default;

public:
    bool initFontSourceSansPro(void);

    QFont font(int size);

    QString fontName(void) { return fontName_ ; }

private:
    QString fontName_;
};

//
// gnomonFontSourceSansPro.h ends here
