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

#include <QtWidgets>

class GNOMONWIDGETS_EXPORT gnomonOverlayPane : public QFrame
{
    Q_OBJECT
    Q_PROPERTY(int width READ width WRITE setWidth NOTIFY widthChanged)

public:
     gnomonOverlayPane(QWidget *parent = nullptr);
    ~gnomonOverlayPane(void);

signals:
    void entered(void);
    void left(void);

signals:
    void widthChanged(void);

public:
    int width(void);

public:
    void setWidth(int);

public:
    bool isToggled(void);

public slots:
    void toggle(void);

public slots:
    void deactivate(void);

public:
    void addLayout(QLayout *);

public:
    void addWidget(QWidget *);

public slots:
    void clear(void);
    void clearLayout(void);

public:
    static void setColor(QColor);

protected:
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    void paintEvent(QPaintEvent *event);

private:
    static QColor color;

private:
    class gnomonOverlayPanePrivate *d;
};

//
// gnomonOverlayPane.h ends here
