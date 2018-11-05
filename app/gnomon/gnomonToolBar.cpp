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
// gnomonToolBarItem
// ///////////////////////////////////////////////////////////////////

class gnomonToolBarItem : public QLabel
{
    Q_OBJECT

public:
    gnomonToolBarItem(const QString& label, QWidget *parent) : QLabel(label, parent) {
        this->setAlignment(Qt::AlignCenter);
        this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
    };

signals:
    void clicked(void);

protected:
    void mousePressEvent(QMouseEvent *) {
        emit clicked();
    }
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
    gnomonToolBarItem *browse_workspace;
    gnomonToolBarItem *fusion_workspace;
    gnomonToolBarItem *segmtt_workspace;
    gnomonToolBarItem *prepro_workspace;
    gnomonToolBarItem *regist_workspace;

public slots:
    void onItemClicked(void);
};

void gnomonToolBarPrivate::onItemClicked(void)
{
    if(sender() == this->browse_workspace)
        q->emit indexChanged(0);

    if(sender() == this->fusion_workspace)
        q->emit indexChanged(1);

    if(sender() == this->segmtt_workspace)
        q->emit indexChanged(2);

    if(sender() == this->prepro_workspace)
        q->emit indexChanged(3);

    if(sender() == this->regist_workspace)
        q->emit indexChanged(4);

    this->browse_workspace->setStyleSheet(QString("color: %1;").arg(sender() == this->browse_workspace ? "#ffffff" : "#777777"));
    this->fusion_workspace->setStyleSheet(QString("color: %1;").arg(sender() == this->fusion_workspace ? "#ffffff" : "#777777"));
    this->segmtt_workspace->setStyleSheet(QString("color: %1;").arg(sender() == this->segmtt_workspace ? "#ffffff" : "#777777"));
    this->prepro_workspace->setStyleSheet(QString("color: %1;").arg(sender() == this->prepro_workspace ? "#ffffff" : "#777777"));
    this->regist_workspace->setStyleSheet(QString("color: %1;").arg(sender() == this->regist_workspace ? "#ffffff" : "#777777"));
}

// ///////////////////////////////////////////////////////////////////
// gnomonToolBar
// ///////////////////////////////////////////////////////////////////

gnomonToolBar::gnomonToolBar(QWidget *parent) : QFrame(parent)
{
    d = new gnomonToolBarPrivate;
    d->q = this;

    d->browse_workspace = new gnomonToolBarItem("Browse", this);
    d->fusion_workspace = new gnomonToolBarItem("Fusion", this);
    d->fusion_workspace->setStyleSheet("color: #777777");
    d->segmtt_workspace = new gnomonToolBarItem("Segmentation", this);
    d->segmtt_workspace->setStyleSheet("color: #777777");
    d->prepro_workspace = new gnomonToolBarItem("Preprocess", this);
    d->prepro_workspace->setStyleSheet("color: #777777");
    d->regist_workspace = new gnomonToolBarItem("Registering", this);
    d->regist_workspace->setStyleSheet("color: #777777");

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(d->browse_workspace);
    layout->addWidget(new gnomonToolBarSeparator(this));
    layout->addWidget(d->fusion_workspace);
    layout->addWidget(new gnomonToolBarSeparator(this));
    layout->addWidget(d->segmtt_workspace);
    layout->addWidget(new gnomonToolBarSeparator(this));
    layout->addWidget(d->prepro_workspace);
    layout->addWidget(new gnomonToolBarSeparator(this));
    layout->addWidget(d->regist_workspace);

    connect(d->browse_workspace, SIGNAL(clicked()), d, SLOT(onItemClicked()));
    connect(d->fusion_workspace, SIGNAL(clicked()), d, SLOT(onItemClicked()));
    connect(d->segmtt_workspace, SIGNAL(clicked()), d, SLOT(onItemClicked()));
    connect(d->prepro_workspace, SIGNAL(clicked()), d, SLOT(onItemClicked()));
    connect(d->regist_workspace, SIGNAL(clicked()), d, SLOT(onItemClicked()));

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

// ///////////////////////////////////////////////////////////////////

#include "gnomonToolBar.moc"

//
// gnomonToolBar.cpp ends here
