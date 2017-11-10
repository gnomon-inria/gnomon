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

#include "gnomonFontSourceSansPro.h"

gnomonFontSourceSansPro::gnomonFontSourceSansPro(QObject *parent) : QObject(parent)
{

}

bool gnomonFontSourceSansPro::initFontSourceSansPro(void)
{
    static int fontSourceSansProFontId = -1;

    if(fontSourceSansProFontId < 0) {

        QFile res(":gnomonFontSourceSansPro.ttf");
        if(!res.open(QIODevice::ReadOnly)) {
            qDebug() << "Font awesome font could not be loaded!";
            return false;
        }

        QByteArray fontData(res.readAll());
        res.close();

        fontSourceSansProFontId = QFontDatabase::addApplicationFontFromData(fontData);
    }

    QStringList loadedFontFamilies = QFontDatabase::applicationFontFamilies(fontSourceSansProFontId);
    if(!loadedFontFamilies.empty()) {
        fontName_= loadedFontFamilies.at(0);
    } else {
        qDebug() << "Font awesome font is empty?!";
        fontSourceSansProFontId = -1;
        return false;
    }

    return true;
}

QFont gnomonFontSourceSansPro::font(int size)
{
    QFont font(fontName_);
    font.setPixelSize(size);
    return font;
}

//
// gnomonFontSourceSansPro.cpp ends here
