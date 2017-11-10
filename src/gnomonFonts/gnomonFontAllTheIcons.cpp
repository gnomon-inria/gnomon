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

#include "gnomonFontAllTheIcons.h"

// ///////////////////////////////////////////////////////////////////
// gnomonFontAllTheIconsCharIconPainter
// ///////////////////////////////////////////////////////////////////

class gnomonFontAllTheIconsCharIconPainter: public gnomonFontAllTheIconsIconPainter
{
public:
    virtual void paint(gnomonFontAllTheIcons *awesome, QPainter *painter, const QRect& rect, QIcon::Mode mode, QIcon::State state, const QVariantMap& options)
    {
        Q_UNUSED(mode);
        Q_UNUSED(state);
        Q_UNUSED(options);

        painter->save();

        QColor color = options.value("color").value<QColor>();
        QString text = options.value("text").toString();

        if(mode == QIcon::Disabled) {
            color = options.value("color-disabled").value<QColor>();
            QVariant alt = options.value("text-disabled");
            if(alt.isValid()) {
                text = alt.toString();
            }
        } else if(mode == QIcon::Active) {
            color = options.value("color-active").value<QColor>();
            QVariant alt = options.value("text-active");
            if(alt.isValid()) {
                text = alt.toString();
            }
        } else if(mode == QIcon::Selected) {
            color = options.value("color-selected").value<QColor>();
            QVariant alt = options.value("text-selected");
            if(alt.isValid()) {
                text = alt.toString();
            }
        }

        painter->setPen(color);

        int drawSize = qRound(rect.height()*options.value("scale-factor").toFloat());

        painter->setFont(awesome->font(drawSize));
        painter->drawText(rect, text, QTextOption(Qt::AlignCenter | Qt::AlignVCenter));
        painter->restore();
    }
};

// ///////////////////////////////////////////////////////////////////
// gnomonFontAllTheIconsIconPainterIconEngine
// ///////////////////////////////////////////////////////////////////

class gnomonFontAllTheIconsIconPainterIconEngine : public QIconEngine
{
public:
    gnomonFontAllTheIconsIconPainterIconEngine(gnomonFontAllTheIcons *awesome, gnomonFontAllTheIconsIconPainter *painter, const QVariantMap& options) : awesomeRef_(awesome), iconPainterRef_(painter), options_(options)
    {

    }

    virtual ~gnomonFontAllTheIconsIconPainterIconEngine(void) {}

    gnomonFontAllTheIconsIconPainterIconEngine *clone(void) const
    {
        return new gnomonFontAllTheIconsIconPainterIconEngine(awesomeRef_, iconPainterRef_, options_);
    }

    virtual void paint(QPainter *painter, const QRect& rect, QIcon::Mode mode, QIcon::State state)
    {
        Q_UNUSED(mode);
        Q_UNUSED(state);

        iconPainterRef_->paint(awesomeRef_, painter, rect, mode, state, options_);
    }

    virtual QPixmap pixmap(const QSize& size, QIcon::Mode mode, QIcon::State state)
    {
        QPixmap pm(size);
        pm.fill(Qt::transparent);

        QPainter p(&pm);
        paint(&p, QRect(QPoint(0,0),size), mode, state);

        return pm;
    }

private:
    gnomonFontAllTheIcons *awesomeRef_;
    gnomonFontAllTheIconsIconPainter *iconPainterRef_;
    QVariantMap options_;
};

// ///////////////////////////////////////////////////////////////////
// gnomonFontAllTheIcons
// ///////////////////////////////////////////////////////////////////

gnomonFontAllTheIcons::gnomonFontAllTheIcons(QObject *parent) : QObject(parent)
{
    setDefaultOption("color", QColor(50,50,50));
    setDefaultOption("color-disabled", QColor(70,70,70,60));
    setDefaultOption("color-active", QColor(10,10,10));
    setDefaultOption("color-selected", QColor(10,10,10));
    setDefaultOption("scale-factor", 0.9);

    setDefaultOption("text", QVariant());
    setDefaultOption("text-disabled", QVariant());
    setDefaultOption("text-active", QVariant());
    setDefaultOption("text-selected", QVariant());

    fontIconPainter_ = new gnomonFontAllTheIconsCharIconPainter();
}

gnomonFontAllTheIcons::~gnomonFontAllTheIcons(void)
{
    delete fontIconPainter_;
    qDeleteAll(painterMap_);
}

void gnomonFontAllTheIcons::init(const QString& fontname)
{
    fontName_ = fontname;
}

bool gnomonFontAllTheIcons::initFontAllTheIcons()
{
    static int fontAllTheIconsFontId = -1;

    if(fontAllTheIconsFontId < 0) {

        QFile res(":gnomonFontAllTheIcons.ttf");
        if(!res.open(QIODevice::ReadOnly)) {
            return false;
        }
        QByteArray fontData(res.readAll());
        res.close();

        fontAllTheIconsFontId = QFontDatabase::addApplicationFontFromData(fontData);
    }

    QStringList loadedFontFamilies = QFontDatabase::applicationFontFamilies(fontAllTheIconsFontId);
    if(!loadedFontFamilies.empty()) {
        fontName_= loadedFontFamilies.at(0);
    } else {
        fontAllTheIconsFontId = -1;
        return false;
    }

    return true;
}

void gnomonFontAllTheIcons::setDefaultOption(const QString& name, const QVariant& value)
{
    defaultOptions_.insert(name, value);
}

QVariant gnomonFontAllTheIcons::defaultOption(const QString& name)
{
    return defaultOptions_.value(name);
}

static QVariantMap mergeOptions(const QVariantMap& defaults, const QVariantMap& override)
{
    QVariantMap result= defaults;
    if(!override.isEmpty()) {
        QMapIterator<QString,QVariant> itr(override);
        while(itr.hasNext()) {
            itr.next();
            result.insert(itr.key(), itr.value());
        }
    }
    return result;
}

QIcon gnomonFontAllTheIcons::icon(int character, const QVariantMap &options)
{
    QVariantMap optionMap = mergeOptions(defaultOptions_, options);
    optionMap.insert("text", QString(QChar(static_cast<int>(character))));

    return icon(fontIconPainter_, optionMap);
}

QIcon gnomonFontAllTheIcons::icon(gnomonFontAllTheIconsIconPainter *painter, const QVariantMap& optionMap)
{
    gnomonFontAllTheIconsIconPainterIconEngine *engine = new gnomonFontAllTheIconsIconPainterIconEngine(this, painter, optionMap);

    return QIcon(engine);
}

void gnomonFontAllTheIcons::give(const QString& name, gnomonFontAllTheIconsIconPainter *painter)
{
    delete painterMap_.value(name);

    painterMap_.insert(name, painter);
}

QFont gnomonFontAllTheIcons::font(int size)
{
    QFont font(fontName_);
    font.setPixelSize(size);
    return font;
}

//
// gnomonFontAllTheIcons.cpp ends here
