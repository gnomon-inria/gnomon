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

#include <QtWidgets>

class gnomonOverlayPaneItem : public QFrame
{
    Q_OBJECT

public:
     gnomonOverlayPaneItem(QWidget *parent = Q_NULLPTR);
    ~gnomonOverlayPaneItem(void);

public slots:
    void setTitle(const QString& title);

public slots:
    void toggle(void);

public slots:
    void addLayout(QLayout *);
    void addWidget(QWidget *);

protected:
    void setSlider(QWidget *);

private:
    class gnomonOverlayPaneItemPrivate *d;
};

//
// gnomonOverlayPaneItem.h ends here
