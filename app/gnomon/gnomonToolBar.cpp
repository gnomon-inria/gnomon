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

#include "gnomonToolBar.h"

#include <gnomonFonts>

// ///////////////////////////////////////////////////////////////////
// gnomonToolBarButton
// ///////////////////////////////////////////////////////////////////

class gnomonToolBarButton : public QLabel
{
    Q_OBJECT

public:
     gnomonToolBarButton(QWidget *parent = nullptr);
    ~gnomonToolBarButton(void);

signals:
    void createFusion(void);
    void createSegmentation(void);
    void createPreprocess(void);
    void createRegistration(void);

public slots:
    void create(QAction *);

protected:
    void mousePressEvent(QMouseEvent *);

private:
    gnomonFontAwesome *font;

private:
    QMenu *menu;
    QAction *action_fusion;
    QAction *action_segmentation;
    QAction *action_preprocess;
    QAction *action_registration;
};

gnomonToolBarButton::gnomonToolBarButton(QWidget *parent)
{
    this->font = new gnomonFontAwesome(this);
    this->font->initFontAwesome();
    this->font->setDefaultOption("color", QColor("#ffffff"));

    this->setAlignment(Qt::AlignCenter);
    this->setPixmap(this->font->icon(fa::plussquare).pixmap(32, 32));
    this->setFixedWidth(32);

    this->menu = new QMenu(this);

    this->action_fusion       = this->menu->addAction("Fusion");
    this->action_segmentation = this->menu->addAction("Segmentation");
    this->action_preprocess   = this->menu->addAction("Preprocess");
    this->action_registration = this->menu->addAction("Registration");

    connect(this->menu, SIGNAL(triggered(QAction *)), this, SLOT(create(QAction *)));
}

gnomonToolBarButton::~gnomonToolBarButton(void)
{

}

void gnomonToolBarButton::create(QAction *action)
{
    if(action == this->action_fusion)
        emit createFusion();

    if(action == this->action_segmentation)
        emit createSegmentation();

    if(action == this->action_preprocess)
        emit createPreprocess();

    if(action == this->action_registration)
        emit createRegistration();
}

void gnomonToolBarButton::mousePressEvent(QMouseEvent *event)
{
    this->menu->exec(event->globalPos());
}

// ///////////////////////////////////////////////////////////////////
// gnomonToolBarItem
// ///////////////////////////////////////////////////////////////////

class gnomonToolBarItem : public QLabel
{
    Q_OBJECT

public:
    gnomonToolBarItem(const QString& label, QWidget *parent) : QLabel(label, parent) {
        static int id = 0;

        this->index = id++;

        this->setAlignment(Qt::AlignCenter);
        this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
        this->setStyleSheet("color: #777777;");
    };

signals:
    void clicked(int);

protected:
    void mousePressEvent(QMouseEvent *) {
        emit clicked(this->index);
    }

private:
    int index = 0;
};

// ///////////////////////////////////////////////////////////////////
// gnomonToolBarSeparator
// ///////////////////////////////////////////////////////////////////

class gnomonToolBarSeparator : public QLabel
{
public:
    gnomonToolBarSeparator(QWidget *parent) : QLabel(parent) {
        this->font = new gnomonFontAwesome(this);
        this->font->initFontAwesome();
        this->font->setDefaultOption("color", QColor("#777777"));

        this->setAlignment(Qt::AlignCenter);
        this->setPixmap(this->font->icon(fa::chevronright).pixmap(32, 32));
        this->setFixedWidth(32);
    }

private:
    gnomonFontAwesome *font;
};

// ///////////////////////////////////////////////////////////////////
// gnomonToolBarPrivate
// ///////////////////////////////////////////////////////////////////

class gnomonToolBarPrivate : public QObject
{
    Q_OBJECT

public:
    gnomonToolBar *q;

public:
    QList<gnomonToolBarItem *> items;

public:
    gnomonToolBarButton *button;

public:
    QHBoxLayout *layout;

public slots:
    void onItemClicked(int);
};

void gnomonToolBarPrivate::onItemClicked(int index)
{
    q->emit indexChanged(index);

    for(gnomonToolBarItem *item : items)
        item->setStyleSheet(QString("color: %1;").arg(sender() == item ? "#ffffff" : "#777777"));
}

// ///////////////////////////////////////////////////////////////////
// gnomonToolBar
// ///////////////////////////////////////////////////////////////////

gnomonToolBar::gnomonToolBar(QWidget *parent) : QFrame(parent)
{
    d = new gnomonToolBarPrivate;
    d->q = this;

    gnomonToolBarItem *item = new gnomonToolBarItem("Browse", this);
    item->setStyleSheet("color: #ffffff;");

    gnomonToolBarButton *button = new gnomonToolBarButton(this);

    d->layout = new QHBoxLayout(this);
    d->layout->addWidget(item);
    d->layout->addWidget(button);

    d->items << item;

    connect(item, SIGNAL(clicked(int)), d, SLOT(onItemClicked(int)));

    connect(button, SIGNAL(createFusion()), this, SLOT(onCreateFusion()));
    connect(button, SIGNAL(createSegmentation()), this, SLOT(onCreateSegmentation()));
    connect(button, SIGNAL(createPreprocess()), this, SLOT(onCreatePreprocess()));
    connect(button, SIGNAL(createRegistration()), this, SLOT(onCreateRegistration()));

    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
}

gnomonToolBar::~gnomonToolBar(void)
{
    delete d;
}

QSize gnomonToolBar::sizeHint(void) const
{
    return QSize(200, 24);
}

void gnomonToolBar::onCreateFusion(void)
{
    gnomonToolBarItem *item = new gnomonToolBarItem("Fusion", this);

    d->layout->insertWidget(d->layout->count()-1, new gnomonToolBarSeparator(this));
    d->layout->insertWidget(d->layout->count()-1, item);

    d->items << item;

    connect(item, SIGNAL(clicked(int)), d, SLOT(onItemClicked(int)));

    emit createFusion();
}

void gnomonToolBar::onCreateSegmentation(void)
{
    gnomonToolBarItem *item = new gnomonToolBarItem("Segmentation", this);

    d->layout->insertWidget(d->layout->count()-1, new gnomonToolBarSeparator(this));
    d->layout->insertWidget(d->layout->count()-1, item);

    d->items << item;

    connect(item, SIGNAL(clicked(int)), d, SLOT(onItemClicked(int)));

    emit createSegmentation();
}

void gnomonToolBar::onCreatePreprocess(void)
{
    gnomonToolBarItem *item = new gnomonToolBarItem("Preprocess", this);

    d->layout->insertWidget(d->layout->count()-1, new gnomonToolBarSeparator(this));
    d->layout->insertWidget(d->layout->count()-1, item);

    d->items << item;

    connect(item, SIGNAL(clicked(int)), d, SLOT(onItemClicked(int)));

    emit createPreprocess();
}

void gnomonToolBar::onCreateRegistration(void)
{
    gnomonToolBarItem *item = new gnomonToolBarItem("Registration", this);

    d->layout->insertWidget(d->layout->count()-1, new gnomonToolBarSeparator(this));
    d->layout->insertWidget(d->layout->count()-1, item);

    d->items << item;

    connect(item, SIGNAL(clicked(int)), d, SLOT(onItemClicked(int)));

    emit createRegistration();
}

// ///////////////////////////////////////////////////////////////////

#include "gnomonToolBar.moc"

//
// gnomonToolBar.cpp ends here
