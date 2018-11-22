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

#pragma once

#include <gnomonWidgetsExport>

#include <QtCore>
#include <QtWidgets>

class gnomonImagesSerie;
using gnomonImagesSeriePtr = QSharedPointer<gnomonImagesSerie>;

class GNOMONWIDGETS_EXPORT gnomonImageManager : public QFrame
{
    Q_OBJECT

public:
    static gnomonImageManager *instance(void);

signals:
    void shrink(void);
    void expand(void);

public:
    QSize sizeHint(void) const;

public slots:
    void addImage(gnomonImagesSeriePtr, const QColor&);

public:
    gnomonImagesSeriePtr get(int index);

public:
    QPixmap thumbnail(int index);

protected:
     gnomonImageManager(QWidget *parent = nullptr);
    ~gnomonImageManager(void);

protected:
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);

protected:
    void mousePressEvent(QMouseEvent *);

protected:
    void paintEvent(QPaintEvent *);

private:
    class gnomonImageManagerPrivate *d;

private:
    static gnomonImageManager *s_instance;
};

//
// gnomonImageManager.h ends here
