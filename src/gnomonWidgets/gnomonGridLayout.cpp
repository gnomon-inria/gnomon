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

#include "gnomonGridLayout.h"
#include "gnomonViewVolumic.h"

#include <gnomonFonts>

#include <math.h>

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class gnomonGridLayoutButton : public QLabel
{
    Q_OBJECT

public:
    gnomonGridLayoutButton(QWidget *parent = nullptr) : QLabel(parent) {

        this->font = new gnomonFontAwesome(this);
        this->font->initFontAwesome();
        this->font->setDefaultOption("color", QColor("#ffffff"));

        this->setAlignment(Qt::AlignCenter);
        this->setPixmap(this->font->icon(fa::plus).pixmap(64, 64));
        this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        this->setStyleSheet("background: #777777");
    }

signals:
    void increase(void);

public:
    QSize sizeHint(void) const {
        return QSize(800, 600);
    }

protected:
    void mousePressEvent(QMouseEvent *) {
        emit increase();
    }

private:
    gnomonFontAwesome *font;
};

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class gnomonGridLayoutPrivate
{
public:
    QList<gnomonViewVolumic *> views;

public:
    gnomonGridLayoutButton *button;
};

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

gnomonGridLayout::gnomonGridLayout(QWidget *parent) : QGridLayout(parent)
{
    d = new gnomonGridLayoutPrivate;

    d->button = new gnomonGridLayoutButton;

    connect(d->button, SIGNAL(increase()), this, SLOT(addView()));

    this->setContentsMargins(0, 0, 0, 0);
    this->setSpacing(0);
    this->addWidget(d->button);
    this->addView();
}

gnomonGridLayout::~gnomonGridLayout(void)
{
    delete d->button;
    delete d;
}

void gnomonGridLayout::addView(void)
{
    d->views << new gnomonViewVolumic;

    this->update();
}

QList<gnomonViewVolumic *> gnomonGridLayout::views(void)
{
    return d->views;
}

void gnomonGridLayout::clear(void)
{
    QLayoutItem *child; while ((child = this->takeAt(0)) != 0);
}

void gnomonGridLayout::update(void)
{
    this->clear();

    int n = ceil(sqrt(d->views.count() + 1));
    int c = 0;
    int C = d->views.count();
    int i;
    int j;

    for(i = 0; c < C && i < n; i++)
        for(j = 0; c < C && j < n; j++)
            this->addWidget(d->views.at(c++), i, j);

    i--;

    if (j == n) {
        j = 0;
        i++;
    }

    this->addWidget(d->button, i, j);
}

// ///////////////////////////////////////////////////////////////////

#include "gnomonGridLayout.moc"

//
// gnomonGridLayout.cpp ends here
