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

class gnomonFontSourceCodePro : public QObject
{
    Q_OBJECT

public:
     gnomonFontSourceCodePro(QObject *parent = 0);
    ~gnomonFontSourceCodePro(void) = default;

public:
    bool initFontSourceCodePro(void);

    QFont font(int size);

    QString fontName(void) { return fontName_ ; }

private:
    QString fontName_;
};

//
// gnomonFontSourceCodePro.h ends here
