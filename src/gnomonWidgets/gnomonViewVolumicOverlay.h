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

class GNOMONWIDGETS_EXPORT gnomonViewVolumicOverlay : public QWidget
{
    Q_OBJECT

public:
    gnomonViewVolumicOverlay(fa::icon, QString = "", QWidget *parent = nullptr);
    gnomonViewVolumicOverlay(const QString& path, QString = "", QWidget *parent = nullptr);
    ~gnomonViewVolumicOverlay(void);

    void changePath(const QString&);

    QString text() const;
    void activate(bool);
    void toggle(bool);

    bool isActivated(void) const;
    bool isToggled(void) const;

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
    QColor             default_color;
};
