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

#include "gnomonViewFormList.h"
#include "gnomonViewForm.h"

// #include <dtkFonts>
// #include <dtkThemes>

// #include <math.h>

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

/* class gnomonViewFormListButton : public QLabel
{
    Q_OBJECT

public:
    gnomonViewFormListButton(QWidget *parent = nullptr) : QLabel(parent) {
        dtkFontAwesome::instance()->initFontAwesome();
        dtkFontAwesome::instance()->setDefaultOption("color", dtkThemesEngine::instance()->color("@fg"));

        this->setAlignment(Qt::AlignCenter);
        this->setPixmap(dtkFontAwesome::instance()->icon(fa::plus).pixmap(64, 64));
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
}; */

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class gnomonViewFormListPrivate
{
public:
    QList<gnomonViewForm *> views;

/*public:
    gnomonViewFormListButton *button;

public:
    std::size_t landmark_id = 0;*/
};

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

gnomonViewFormList::gnomonViewFormList(QObject *parent) : QObject(parent)
{
    d = new gnomonViewFormListPrivate;

    /*d->button = new gnomonViewFormListButton;

    connect(d->button, SIGNAL(increase()), this, SLOT(addView()));

    this->setContentsMargins(0, 0, 0, 0);
    this->setSpacing(0);
    this->addWidget(d->button);*/
}

gnomonViewFormList::~gnomonViewFormList(void)
{
    // delete d->button;
    delete d;
}

void gnomonViewFormList::addView(void)
{
    gnomonViewForm *new_view = new gnomonViewForm(this->parent());

//    for(gnomonViewForm *view : d->views) {
//        connect(new_view, &gnomonViewForm::landmarkAdded, view, &gnomonViewForm::addLandmark);
//        connect(view, &gnomonViewForm::landmarkAdded, new_view, &gnomonViewForm::addLandmark);
//
//        connect(new_view, &gnomonViewForm::landmarkRemoved, view, &gnomonViewForm::removeLandmark);
//        connect(view, &gnomonViewForm::landmarkRemoved, new_view, &gnomonViewForm::removeLandmark);
//    }

    connect(new_view, &gnomonViewForm::formAdded, this, &gnomonViewFormList::formAdded);

    d->views << new_view;

    // this->update();

    emit viewAdded(new_view);
}

QList<gnomonViewForm *> gnomonViewFormList::views(void)
{
    return d->views;
}

QList<QObject *> gnomonViewFormList::viewsAsQObject(void)
{
    QList<QObject *> o_views;
    for(gnomonViewForm *v : d->views)
        o_views.append(static_cast<QObject *>(v));

    return o_views;
}


void gnomonViewFormList::removeAllViews(void)
{
    d->views.clear();
    // this->update();
}

/* void gnomonViewFormList::clear(void)
{
    QLayoutItem *child; while ((child = this->takeAt(0)) != 0);
}

void gnomonViewFormList::update(void)
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
} */

// ///////////////////////////////////////////////////////////////////

#include "gnomonViewFormList.moc"

//
// gnomonViewFormList.cpp ends here
