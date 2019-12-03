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

#include <dtkFonts>

#include <QtCore>
#include <QtWidgets>

// ///////////////////////////////////////////////////////////////////
// gnomonOverlayButton
// ///////////////////////////////////////////////////////////////////

class GNOMONWIDGETS_EXPORT gnomonOverlayButtonIcon : public QLabel
{
    Q_OBJECT

public:
    gnomonOverlayButtonIcon(QWidget *parent = nullptr);

public:
    QSize sizeHint(void) const override;

signals:
    void clicked(void);

protected:
    void mousePressEvent(QMouseEvent *) override;
};

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONWIDGETS_EXPORT gnomonOverlayButtonText : public QLabel
{
    Q_OBJECT

public:
    gnomonOverlayButtonText(QString = "", QWidget *parent = nullptr);

public:
    QSize sizeHint(void) const override;

signals:
    void clicked(void);

protected:
    void mousePressEvent(QMouseEvent *) override;
};

// ///////////////////////////////////////////////////////////////////
//
// ///////////////////////////////////////////////////////////////////

class GNOMONWIDGETS_EXPORT gnomonOverlayButton : public QFrame
{
    Q_OBJECT

public:
     gnomonOverlayButton(QChar, QString = "", QWidget *parent = nullptr);
     gnomonOverlayButton(fa::icon, QString = "", QWidget *parent = nullptr);
     gnomonOverlayButton(const QString& path_on, const QString& path_off, QString = "", QWidget *parent = nullptr);
    ~gnomonOverlayButton(void);

    void activate(bool);
    void toggle(bool);

    void changeColor(const QColor&);
    void changePaths(const QString&, const QString&);
    void changeIcon(fa::icon);

    QString text(void) const;

    bool isActivated(void) const;
    bool isToggled(void) const;

signals:
    void iconClicked(void);
    void textClicked(void);

public:
    bool toggled   = false;
    bool activated = false;

public:
    gnomonOverlayButtonIcon *label_icon = nullptr;
    gnomonOverlayButtonText *label_text = nullptr;

public:
    fa::icon           icon;
    QPixmap            pix_on;
    QPixmap            pix_off;
    QString            path_on;
    QString            path_off;
    bool               pixmap = false;
};

//
// gnomonOverlayButton.h ends here
