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

class gnomonAbstractForm;
class gnomonAbstractCommand;
class gnomonAbstractVisualization;

class gnomonFormManagerItem;

class GNOMONWIDGETS_EXPORT gnomonFormManager : public QFrame
{
    Q_OBJECT

public:
    static gnomonFormManager *instance(void);

signals:
    void shrink(void);
    void expand(void);

public:
    QSize sizeHint(void) const;

public slots:
    void addForm(gnomonAbstractForm *, const QColor&, gnomonAbstractVisualization* visualization);

public slots:
    void present(gnomonFormManagerItem *);

public:
    gnomonAbstractForm * get(int index);
    gnomonAbstractVisualization * getVisualization(int index);

public:
    QPixmap thumbnail(int index);

protected:
     gnomonFormManager(QWidget *parent = nullptr);
    ~gnomonFormManager(void);

protected:
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);

protected:
    void mousePressEvent(QMouseEvent *);

protected:
    void paintEvent(QPaintEvent *);

private:
    class gnomonFormManagerPrivate *d;

private:
    static gnomonFormManager *s_instance;
};

//
// gnomonFormManager.h ends here
