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
    void createSimulation(void);

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
    QAction *action_simulation;
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
    this->action_simulation   = this->menu->addAction("Simulation");

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

    if(action == this->action_simulation)
        emit createSimulation();
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

    for(gnomonToolBarItem *item : items) {
        int current_index = items.indexOf(item);
        if(item->text() == "Fusion")
            item->setStyleSheet(QString("font-size: %1px; font-style: %2; color: rgb(%3,%4,%5);").arg(current_index == index ? "24" : "12").arg(current_index == index ? "bold" : "normal").arg(gnomonToolBar::fusion_color.red()).arg(gnomonToolBar::fusion_color.green()).arg(gnomonToolBar::fusion_color.blue()));
        else if(item->text() == "Segmentation")
            item->setStyleSheet(QString("font-size: %1px; font-style: %2; color: rgb(%3,%4,%5);").arg(current_index == index ? "24" : "12").arg(current_index == index ? "bold" : "normal").arg(gnomonToolBar::segmentation_color.red()).arg(gnomonToolBar::segmentation_color.green()).arg(gnomonToolBar::segmentation_color.blue()));
        else if(item->text() == "Preprocess")
            item->setStyleSheet(QString("font-size: %1px; font-style: %2; color: rgb(%3,%4,%5);").arg(current_index == index ? "24" : "12").arg(current_index == index ? "bold" : "normal").arg(gnomonToolBar::preprocess_color.red()).arg(gnomonToolBar::preprocess_color.green()).arg(gnomonToolBar::preprocess_color.blue()));
        else if(item->text() == "Registration")
            item->setStyleSheet(QString("font-size: %1px; font-style: %2; color: rgb(%3,%4,%5);").arg(current_index == index ? "24" : "12").arg(current_index == index ? "bold" : "normal").arg(gnomonToolBar::registration_color.red()).arg(gnomonToolBar::registration_color.green()).arg(gnomonToolBar::registration_color.blue()));
        else if(item->text() == "Browse")
            item->setStyleSheet(QString("font-size: %1px; font-style: %2; color: rgb(%3,%4,%5);").arg(current_index == index ? "24" : "12").arg(current_index == index ? "bold" : "normal").arg(gnomonToolBar::browser_color.red()).arg(gnomonToolBar::browser_color.green()).arg(gnomonToolBar::browser_color.blue()));
        else if(item->text() == "Simulation")
            item->setStyleSheet(QString("font-size: %1px; font-style: %2; color: rgb(%3,%4,%5);").arg(current_index == index ? "24" : "12").arg(current_index == index ? "bold" : "normal").arg(gnomonToolBar::simulation_color.red()).arg(gnomonToolBar::simulation_color.green()).arg(gnomonToolBar::simulation_color.blue()));
    }
}

// ///////////////////////////////////////////////////////////////////
// gnomonToolBar
// ///////////////////////////////////////////////////////////////////

gnomonToolBar::gnomonToolBar(QWidget *parent) : QFrame(parent)
{
    d = new gnomonToolBarPrivate;
    d->q = this;

    gnomonToolBarItem *item = new gnomonToolBarItem("Browse", this);
    item->setStyleSheet(QString("color: rgb(%1,%2,%3);").arg(browser_color.red()).arg(browser_color.green()).arg(browser_color.blue()));

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
    connect(button, SIGNAL(createSimulation()), this, SLOT(onCreateSimulation()));

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

void gnomonToolBar::setCurrentIndex(int i)
{
    d->onItemClicked(i);
}

void gnomonToolBar::onCreateFusion(void)
{
    gnomonToolBarItem *item = new gnomonToolBarItem("Fusion", this);
    item->setStyleSheet(QString("color: rgb(%1,%2,%3);").arg(fusion_color.red()).arg(fusion_color.green()).arg(fusion_color.blue()));

    d->layout->insertWidget(d->layout->count()-1, new gnomonToolBarSeparator(this));
    d->layout->insertWidget(d->layout->count()-1, item);

    d->items << item;

    connect(item, SIGNAL(clicked(int)), d, SLOT(onItemClicked(int)));
    d->onItemClicked(d->items.count()-1);

    emit createFusion();
}

void gnomonToolBar::onCreateSegmentation(void)
{
    gnomonToolBarItem *item = new gnomonToolBarItem("Segmentation", this);
    item->setStyleSheet(QString("color: rgb(%1,%2,%3);").arg(segmentation_color.red()).arg(segmentation_color.green()).arg(segmentation_color.blue()));

    d->layout->insertWidget(d->layout->count()-1, new gnomonToolBarSeparator(this));
    d->layout->insertWidget(d->layout->count()-1, item);

    d->items << item;

    connect(item, SIGNAL(clicked(int)), d, SLOT(onItemClicked(int)));
    d->onItemClicked(d->items.count()-1);

    emit createSegmentation();
}

void gnomonToolBar::onCreatePreprocess(void)
{
    gnomonToolBarItem *item = new gnomonToolBarItem("Preprocess", this);
    item->setStyleSheet(QString("color: rgb(%1,%2,%3);").arg(preprocess_color.red()).arg(preprocess_color.green()).arg(preprocess_color.blue()));

    d->layout->insertWidget(d->layout->count()-1, new gnomonToolBarSeparator(this));
    d->layout->insertWidget(d->layout->count()-1, item);

    d->items << item;

    connect(item, SIGNAL(clicked(int)), d, SLOT(onItemClicked(int)));
    d->onItemClicked(d->items.count()-1);

    emit createPreprocess();
}

void gnomonToolBar::onCreateRegistration(void)
{
    gnomonToolBarItem *item = new gnomonToolBarItem("Registration", this);
    item->setStyleSheet(QString("color: rgb(%1,%2,%3);").arg(registration_color.red()).arg(registration_color.green()).arg(registration_color.blue()));

    d->layout->insertWidget(d->layout->count()-1, new gnomonToolBarSeparator(this));
    d->layout->insertWidget(d->layout->count()-1, item);

    d->items << item;

    connect(item, SIGNAL(clicked(int)), d, SLOT(onItemClicked(int)));
    d->onItemClicked(d->items.count()-1);

    emit createRegistration();
}

void gnomonToolBar::onCreateSimulation(void)
{
    gnomonToolBarItem *item = new gnomonToolBarItem("Simulation", this);
    item->setStyleSheet(QString("color: rgb(%1,%2,%3);").arg(simulation_color.red()).arg(simulation_color.green()).arg(simulation_color.blue()));

    d->layout->insertWidget(d->layout->count()-1, new gnomonToolBarSeparator(this));
    d->layout->insertWidget(d->layout->count()-1, item);

    d->items << item;

    connect(item, SIGNAL(clicked(int)), d, SLOT(onItemClicked(int)));

    emit createSimulation();
}

// ///////////////////////////////////////////////////////////////////

QColor gnomonToolBar::browser_color = QColor("#ff3b30");
QColor gnomonToolBar::fusion_color = QColor("#ff9500");
QColor gnomonToolBar::registration_color = QColor("#ffcc00");
QColor gnomonToolBar::preprocess_color = QColor("#4cd964");
QColor gnomonToolBar::segmentation_color = QColor("#5ac8fa");
QColor gnomonToolBar::simulation_color = QColor("#5ac8fa");

// ///////////////////////////////////////////////////////////////////

#include "gnomonToolBar.moc"

//
// gnomonToolBar.cpp ends here
