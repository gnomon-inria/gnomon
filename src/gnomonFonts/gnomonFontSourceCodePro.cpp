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

#include "gnomonFontSourceCodePro.h"

gnomonFontSourceCodePro::gnomonFontSourceCodePro(QObject *parent) : QObject(parent)
{

}

bool gnomonFontSourceCodePro::initFontSourceCodePro(void)
{
    static int fontSourceCodeProFontId = -1;

    if(fontSourceCodeProFontId < 0) {

        QFile res(":gnomonFontSourceCodePro.ttf");
        if(!res.open(QIODevice::ReadOnly)) {
            qDebug() << "Font awesome font could not be loaded!";
            return false;
        }

        QByteArray fontData(res.readAll());
        res.close();

        fontSourceCodeProFontId = QFontDatabase::addApplicationFontFromData(fontData);
    }

    QStringList loadedFontFamilies = QFontDatabase::applicationFontFamilies(fontSourceCodeProFontId);
    if(!loadedFontFamilies.empty()) {
        fontName_= loadedFontFamilies.at(0);
    } else {
        qDebug() << "Font awesome font is empty?!";
        fontSourceCodeProFontId = -1;
        return false;
    }

    return true;
}

QFont gnomonFontSourceCodePro::font(int size)
{
    QFont font(fontName_);
    font.setPixelSize(size);
    return font;
}

//
// gnomonFontSourceCodePro.cpp ends here
