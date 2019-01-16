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

#include "gnomonItemButton.h"

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
    void createLSystemSimulator(void);
    void createPreprocess(void);
    void createRegistration(void);
    void createSegmentation(void);
    void createSimulation(void);
    void createTreeAnlysis(void);

public slots:
    void create(QAction *);

protected:
    void mousePressEvent(QMouseEvent *);

private:
    gnomonFontAwesome *font = nullptr;

private:
    QMenu *menu;
    QAction *action_fusion;
    QAction *action_lsystem;
    QAction *action_segmentation;
    QAction *action_preprocess;
    QAction *action_registration;
    QAction *action_simulation;
    QAction *action_tree_analysis;
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

    this->action_fusion        = this->menu->addAction("Fusion");
    this->action_lsystem       = this->menu->addAction("LSystem simulator");
    this->action_preprocess    = this->menu->addAction("Preprocess");
    this->action_registration  = this->menu->addAction("Registration");
    this->action_segmentation  = this->menu->addAction("Segmentation");
    this->action_simulation    = this->menu->addAction("Simulation");
    this->action_tree_analysis = this->menu->addAction("Tree analysis");

    connect(this->menu, SIGNAL(triggered(QAction *)), this, SLOT(create(QAction *)));
}

gnomonToolBarButton::~gnomonToolBarButton(void)
{

}

void gnomonToolBarButton::create(QAction *action)
{
    if(action == this->action_fusion)
        emit createFusion();

    if(action == this->action_lsystem)
        emit createLSystemSimulator();

    if(action == this->action_preprocess)
        emit createPreprocess();

    if(action == this->action_registration)
        emit createRegistration();

    if(action == this->action_segmentation)
        emit createSegmentation();

    if(action == this->action_simulation)
        emit createSimulation();

    if(action == this->action_tree_analysis)
        emit createTreeAnlysis();
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
    gnomonToolBarItem(const QColor& color, const QString& label, QWidget *parent = nullptr, bool display_destroy = true) : QLabel(label, parent) {

        m_color = color;

        if (display_destroy) {

            this->button_destroy = new gnomonItemButton(color, fa::times, this);
            this->button_destroy->setAlignment(Qt::AlignRight);
            this->button_destroy->setVisible(false);

            connect(this->button_destroy, SIGNAL(clicked()), this, SIGNAL(destroy()));
        }

        this->setAlignment(Qt::AlignCenter);
        this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
        this->setMouseTracking(true);
        this->setStyleSheet(QString("color: rgb(%1,%2,%3);").arg(color.red()).arg(color.green()).arg(color.blue()));

    };

signals:
    void clicked(void);
    void destroy(void);

public:
    gnomonItemButton *button_destroy = nullptr;

public:
    const QColor& color(void) { return m_color; } ;

protected:
    void mousePressEvent(QMouseEvent *) {
        emit clicked();
    }

    void enterEvent(QEvent *)
    {
        if (this->button_destroy)
            this->button_destroy->setVisible(true);
    }

    void leaveEvent(QEvent *)
    {
        if (this->button_destroy)
            this->button_destroy->setVisible(false);
    }

private:
    QColor m_color;
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
    gnomonFontAwesome *font = nullptr;
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
    QList<gnomonToolBarSeparator *> separators;

public:
    gnomonToolBarButton *button;

public:
    bool inside = false;

public:
    QHBoxLayout *layout;

public slots:
    void onItemClicked(int);

signals:
    void indexDeleted(int);

public:
    void createWorkspace(const QColor &, const QString &);
};

void gnomonToolBarPrivate::createWorkspace(const QColor & color, const QString& name)
{
    gnomonToolBarItem *item = new gnomonToolBarItem(color, name, q);
    gnomonToolBarSeparator *separator = new gnomonToolBarSeparator(q);

    this->layout->insertWidget(this->layout->count()-1, separator);
    this->layout->insertWidget(this->layout->count()-1, item);

    this->items << item;
    this->separators << separator;

    connect(item, &gnomonToolBarItem::clicked, [=] () {
        int index =  this->items.indexOf(item);
        this->onItemClicked(index);
    });

    connect(item, &gnomonToolBarItem::destroy, [=] () {
        int index =  this->items.indexOf(item);
        this->items.takeAt(index);
        auto separator = this->separators.takeAt(index-1);
        this->layout->removeWidget(separator);
        this->layout->removeWidget(item);
        delete item;
        delete separator;
        emit indexDeleted(index);
    });

    this->onItemClicked(this->items.count()-1);
}

void gnomonToolBarPrivate::onItemClicked(int index)
{
    q->emit indexChanged(index);

    for(gnomonToolBarItem *item : items) {
        int current_index = items.indexOf(item);
        if(item->text() == "Fusion")
            item->setStyleSheet(QString("font-size: %1px; font-style: %2; color: rgb(%3,%4,%5);").arg(current_index == index ? "24" : "12").arg(current_index == index ? "bold" : "normal").arg(gnomonToolBar::fusion_color.red()).arg(gnomonToolBar::fusion_color.green()).arg(gnomonToolBar::fusion_color.blue()));
        else if(item->text() == "LSystem simulator")
            item->setStyleSheet(QString("font-size: %1px; font-style: %2; color: rgb(%3,%4,%5);").arg(current_index == index ? "24" : "12").arg(current_index == index ? "bold" : "normal").arg(gnomonToolBar::lsystem_color.red()).arg(gnomonToolBar::lsystem_color.green()).arg(gnomonToolBar::lsystem_color.blue()));
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
        else if(item->text() == "Tree analysis")
            item->setStyleSheet(QString("font-size: %1px; font-style: %2; color: rgb(%3,%4,%5);").arg(current_index == index ? "24" : "12").arg(current_index == index ? "bold" : "normal").arg(gnomonToolBar::tree_analysis_color.red()).arg(gnomonToolBar::tree_analysis_color.green()).arg(gnomonToolBar::tree_analysis_color.blue()));
    }
}

// ///////////////////////////////////////////////////////////////////
// gnomonToolBar
// ///////////////////////////////////////////////////////////////////

gnomonToolBar::gnomonToolBar(QWidget *parent) : QFrame(parent)
{
    d = new gnomonToolBarPrivate;
    d->q = this;

    gnomonToolBarItem *item = new gnomonToolBarItem(browser_color, "Browse", this, false);

    gnomonToolBarButton *button = new gnomonToolBarButton(this);

    d->layout = new QHBoxLayout(this);
    d->layout->addWidget(item);
    d->layout->addWidget(button);

    d->items << item;

    connect(item, &gnomonToolBarItem::clicked, [=] () { d->onItemClicked(0); });
    connect(d, SIGNAL(indexDeleted(int)), this, SIGNAL(indexDeleted(int)));

    connect(button, SIGNAL(createFusion()), this, SLOT(onCreateFusion()));
    connect(button, SIGNAL(createLSystemSimulator()), this, SLOT(onCreateLSystemSimulator()));
    connect(button, SIGNAL(createPreprocess()), this, SLOT(onCreatePreprocess()));
    connect(button, SIGNAL(createRegistration()), this, SLOT(onCreateRegistration()));
    connect(button, SIGNAL(createSegmentation()), this, SLOT(onCreateSegmentation()));
    connect(button, SIGNAL(createSimulation()), this, SLOT(onCreateSimulation()));
    connect(button, SIGNAL(createTreeAnlysis()), this, SLOT(onCreateTreeAnalysis()));

    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    this->setMouseTracking(true);
}

gnomonToolBar::~gnomonToolBar(void)
{
    delete d;
}

void gnomonToolBar::enterEvent(QEvent *)
{
    d->inside = true;

    this->update();
}

void gnomonToolBar::leaveEvent(QEvent *)
{
    d->inside = false;

    this->update();
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
    d->createWorkspace(fusion_color, "Fusion");

    emit createFusion();
}

void gnomonToolBar::onCreateLSystemSimulator(void)
{
    d->createWorkspace(lsystem_color, "LSystem simulator");

    emit createLSystemSimulator();
}

void gnomonToolBar::onCreateSegmentation(void)
{
    d->createWorkspace(segmentation_color, "Segmentation");

    emit createSegmentation();
}

void gnomonToolBar::onCreatePreprocess(void)
{
    d->createWorkspace(preprocess_color, "Preprocess");

    emit createPreprocess();
}

void gnomonToolBar::onCreateRegistration(void)
{
    d->createWorkspace(registration_color, "Registration");

    emit createRegistration();
}

void gnomonToolBar::onCreateSimulation(void)
{
    d->createWorkspace(simulation_color, "Simulation");

    emit createSimulation();
}

void gnomonToolBar::onCreateTreeAnalysis(void)
{
    d->createWorkspace(tree_analysis_color, "Tree analysis");

    emit createTreeAnalysis();
}

// ///////////////////////////////////////////////////////////////////

QColor gnomonToolBar::browser_color = QColor("#ff3b30");
QColor gnomonToolBar::lsystem_color = QColor("#89a348");
QColor gnomonToolBar::fusion_color = QColor("#ff9500");
QColor gnomonToolBar::registration_color = QColor("#ffcc00");
QColor gnomonToolBar::preprocess_color = QColor("#4cd964");
QColor gnomonToolBar::segmentation_color = QColor("#5ac8fa");
QColor gnomonToolBar::simulation_color = QColor("#5856d6");
QColor gnomonToolBar::tree_analysis_color = QColor("#734906");

// ///////////////////////////////////////////////////////////////////

#include "gnomonToolBar.moc"

//
// gnomonToolBar.cpp ends here
