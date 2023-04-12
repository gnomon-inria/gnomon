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

#include "gnomonVtkViewList.h"
#include "gnomonVtkView.h"

// #include <dtkFonts>
// #include <dtkThemes>

// #include <math.h>

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

/* class gnomonVtkViewListButton : public QLabel
{
    Q_OBJECT

public:
    gnomonVtkViewListButton(QWidget *parent = nullptr) : QLabel(parent) {
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

class gnomonVtkViewListPrivate
{
public:
    QList<gnomonVtkView *> views;

/*public:
    gnomonVtkViewListButton *button;

public:
    std::size_t landmark_id = 0;*/
};

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

gnomonVtkViewList::gnomonVtkViewList(QObject *parent) : QObject(parent)
{
    d = new gnomonVtkViewListPrivate;

    /*d->button = new gnomonVtkViewListButton;

    connect(d->button, SIGNAL(increase()), this, SLOT(addView()));

    this->setContentsMargins(0, 0, 0, 0);
    this->setSpacing(0);
    this->addWidget(d->button);*/
}

gnomonVtkViewList::~gnomonVtkViewList(void)
{
    // delete d->button;
    for(auto view: d->views) {
        delete view;
    }
    d->views.clear();
    delete d;
}

void gnomonVtkViewList::addView(const QVector<QString> &accepted_forms, QStringList nodePortNames)
{
    gnomonVtkView *new_view = new gnomonVtkView(this->parent());
    new_view->setNodePortNames(nodePortNames);

//    for(gnomonVtkView *view : d->views) {
//        connect(new_view, &gnomonVtkView::landmarkAdded, view, &gnomonVtkView::addLandmark);
//        connect(view, &gnomonVtkView::landmarkAdded, new_view, &gnomonVtkView::addLandmark);
//
//        connect(new_view, &gnomonVtkView::landmarkRemoved, view, &gnomonVtkView::removeLandmark);
//        connect(view, &gnomonVtkView::landmarkRemoved, new_view, &gnomonVtkView::removeLandmark);
//    }

    connect(new_view, &gnomonVtkView::formAdded, this, &gnomonVtkViewList::formAdded);

    connect(new_view, &gnomonVtkView::formsChanged, this, &gnomonVtkViewList::formsChanged);

    d->views << new_view;

    // this->update();

    emit viewAdded(new_view);

    for(const auto & form_type: accepted_forms) {
        new_view->setAcceptForm(form_type, true);
    }
}

gnomonVtkView *gnomonVtkViewList::operator[](int i)
{
    return this->views()[i];
}

QList<gnomonVtkView *> gnomonVtkViewList::views(void)
{
    return d->views;
}

QList<QObject *> gnomonVtkViewList::viewsAsQObject(void)
{
    QList<QObject *> o_views;
    for(gnomonVtkView *v : d->views)
        o_views.append(static_cast<QObject *>(v));

    return o_views;
}


void gnomonVtkViewList::removeAllViews(void)
{
    for(auto view: d->views) {
        delete view;
    }
    d->views.clear();
    // this->update();
}

/* void gnomonVtkViewList::clear(void)
{
    QLayoutItem *child; while ((child = this->takeAt(0)) != 0);
}

void gnomonVtkViewList::update(void)
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

#include "gnomonVtkViewList.moc"

//
// gnomonVtkViewList.cpp ends here
