#pragma once

#include <gnomonWidgetsExport>

#include <gnomonStyle>
#include <gnomonFonts>

#include <QtCore>
#include <QtWidgets>

// ///////////////////////////////////////////////////////////////////
// gnomonViewVolumicOverlay
// ///////////////////////////////////////////////////////////////////

class GNOMONWIDGETS_EXPORT gnomonViewVolumicOverlayIcon : public QLabel
{
    Q_OBJECT

public:
    gnomonViewVolumicOverlayIcon(QWidget *parent = nullptr);

public:
    QSize sizeHint(void) const override;

 signals:
    void clicked(void);

protected:
    void mousePressEvent(QMouseEvent *);
};

//////////

class GNOMONWIDGETS_EXPORT gnomonViewVolumicOverlayText : public QLabel
{
    Q_OBJECT

public:
    gnomonViewVolumicOverlayText(QString = "", QWidget *parent = nullptr);

signals:
    void clicked(void);

protected:
    void mousePressEvent(QMouseEvent *);
};

//////////

class GNOMONWIDGETS_EXPORT gnomonViewVolumicOverlay : public QFrame
{
    Q_OBJECT

public:
    gnomonViewVolumicOverlay(fa::icon, QString = "", QWidget *parent = nullptr);
    gnomonViewVolumicOverlay(const QString& path_on, const QString& path_off, QString = "", QWidget *parent = nullptr);
    ~gnomonViewVolumicOverlay(void);

    void activate(bool);
    void toggle(bool);

    void changeColor(const QColor&);
    void changePaths(const QString&, const QString&);
    void changeIcon(fa::icon);

    QString text() const;

    bool isActivated(void) const;
    bool isToggled(void) const;

public:
    QSize sizeHint(void) const override;
    int textWidth(void) const;

signals:
    void iconClicked(void);
    void textClicked(void);

public:
    bool toggled   = false;
    bool activated = false;

public:
    gnomonViewVolumicOverlayIcon *label_icon = nullptr;
    gnomonViewVolumicOverlayText *label_text = nullptr;

public:
    gnomonFontAwesome *font = nullptr;
    fa::icon           icon;
    QString            path_on;
    QString            path_off;
    bool               pixmap = false;
};
