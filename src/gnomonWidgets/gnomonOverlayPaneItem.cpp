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

#include "gnomonOverlayPaneItem.h"

#include <gnomonFonts>

// ///////////////////////////////////////////////////////////////////
// gnomonOverlayPaneItemHeader
// ///////////////////////////////////////////////////////////////////

class gnomonOverlayPaneItemHeader : public QFrame
{
    Q_OBJECT

public:
     gnomonOverlayPaneItemHeader(QWidget *parent = nullptr);
    ~gnomonOverlayPaneItemHeader(void);

public:
    QSize sizeHint(void) const;

public:
    void setTitle(const QString&);

public slots:
    void toggle(void);

signals:
    void toggled(bool);

protected:
    void mousePressEvent(QMouseEvent *);

private:
    QLabel *title;
    QLabel *handle;

private:
    bool expanded = false;

private:
    gnomonFontAwesome *font;
};

gnomonOverlayPaneItemHeader::gnomonOverlayPaneItemHeader(QWidget *parent) : QFrame(parent)
{
    this->font = new gnomonFontAwesome(this);
    this->font->initFontAwesome();
    this->font->setDefaultOption("color", QColor("#ffffff"));

    this->title = new QLabel(this);
    this->title->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    this->handle = new QLabel(this);
    this->handle->setAlignment(Qt::AlignRight | Qt::AlignTop);
    this->handle->setPixmap(this->font->icon(fa::plussquareo).pixmap(25, 25));

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(this->title);
    layout->addStretch();
    layout->addWidget(this->handle);

    this->setLayout(layout);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
}

gnomonOverlayPaneItemHeader::~gnomonOverlayPaneItemHeader(void)
{

}

QSize gnomonOverlayPaneItemHeader::sizeHint(void) const
{
    return QSize(50, 50);
}

void gnomonOverlayPaneItemHeader::setTitle(const QString& title)
{
    this->title->setText(title);
}

void gnomonOverlayPaneItemHeader::toggle(void)
{
    QVariantMap options;
    options.insert("text",           QColor("#ffffff"));
    options.insert("text-active",    QColor("#ffffff"));
    options.insert("text-disabled",  QColor("#ffffff"));
    options.insert("text-selected",  QColor("#ffffff"));
    options.insert("color",          QColor("#ffffff"));
    options.insert("color-active",   QColor("#ffffff"));
    options.insert("color-disabled", QColor("#ffffff"));
    options.insert("color-selected", QColor("#ffffff"));

    this->expanded = !this->expanded;

    if (this->expanded)
        this->handle->setPixmap(this->font->icon(fa::minussquareo, options).pixmap(25, 25));
    else
        this->handle->setPixmap(this->font->icon(fa::plussquareo, options).pixmap(25, 25));

    emit toggled(this->expanded);
}

void gnomonOverlayPaneItemHeader::mousePressEvent(QMouseEvent *)
{
    this->toggle();
}

// ///////////////////////////////////////////////////////////////////
// gnomonOverlayPaneItemFooter
// ///////////////////////////////////////////////////////////////////

class gnomonOverlayPaneItemFooter : public QFrame
{
    Q_OBJECT

public:
     gnomonOverlayPaneItemFooter(QWidget *parent = nullptr);
    ~gnomonOverlayPaneItemFooter(void);

public slots:
    void addLayout(QLayout *);
    void addWidget(QWidget *);

public slots:
    void clear(void);

private:
    QGroupBox *box;
};

gnomonOverlayPaneItemFooter::gnomonOverlayPaneItemFooter(QWidget *parent) : QFrame(parent)
{
    QVBoxLayout *box_layout = new QVBoxLayout;
    box_layout->setContentsMargins(10, 0, 10, 0);
    box_layout->setSpacing(5);

    this->box = new QGroupBox(this);
    this->box->setAlignment(Qt::AlignTop);
    this->box->setLayout(box_layout);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(10, 10, 10, 10);
    layout->addWidget(this->box);

    this->setVisible(false);
}

gnomonOverlayPaneItemFooter::~gnomonOverlayPaneItemFooter(void)
{

}

void gnomonOverlayPaneItemFooter::addLayout(QLayout *layout)
{
    dynamic_cast<QBoxLayout *>(this->box->layout())->addLayout(layout);
}

void gnomonOverlayPaneItemFooter::addWidget(QWidget *widget)
{
    dynamic_cast<QBoxLayout *>(this->box->layout())->addWidget(widget);
}

void gnomonOverlayPaneItemFooter::clear(void)
{
    this->layout()->removeWidget(this->box);
}

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class gnomonOverlayPaneItemPrivate
{
public:
    gnomonOverlayPaneItemHeader *header;
    gnomonOverlayPaneItemFooter *footer;
};

gnomonOverlayPaneItem::gnomonOverlayPaneItem(QWidget *parent) : QFrame(parent)
{
    d = new gnomonOverlayPaneItemPrivate;

    d->header = new gnomonOverlayPaneItemHeader(this);
    d->footer = new gnomonOverlayPaneItemFooter(this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(d->header);
    layout->addWidget(d->footer);

    connect(d->header, SIGNAL(toggled(bool)), d->footer, SLOT(setVisible(bool)));
}

gnomonOverlayPaneItem::~gnomonOverlayPaneItem(void)
{
    delete d;
}

void gnomonOverlayPaneItem::setTitle(const QString& title)
{
    d->header->setTitle(title);
}

void gnomonOverlayPaneItem::toggle(void)
{
    d->header->toggle();
}

void gnomonOverlayPaneItem::addLayout(QLayout *layout)
{
    d->footer->addLayout(layout);
}

void gnomonOverlayPaneItem::addWidget(QWidget *widget)
{
    d->footer->addWidget(widget);
}

void gnomonOverlayPaneItem::setSlider(QWidget *slider)
{
    d->footer->clear();
    d->footer->layout()->setContentsMargins(0, 0, 0, 0);
    d->footer->layout()->addWidget(slider);
}

// ///////////////////////////////////////////////////////////////////

#include "gnomonOverlayPaneItem.moc"

//
// gnomonOverlayPaneItem.cpp ends here
