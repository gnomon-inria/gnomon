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

#include <QWidget>
#include <QTimer>
#include <QColor>

class gnomonSpinner : public QWidget
{
    Q_OBJECT

public:
    gnomonSpinner(QWidget *parent = 0, bool centerOnParent = true, bool disableParentWhenSpinning = true);
    gnomonSpinner(Qt::WindowModality modality, QWidget *parent = 0, bool centerOnParent = true, bool disableParentWhenSpinning = true);
                                                 
public slots:
    void start();
    void stop();

public:
    void setColor(QColor color);
    void setRoundness(qreal roundness);
    void setMinimumTrailOpacity(qreal minimumTrailOpacity);
    void setTrailFadePercentage(qreal trail);
    void setRevolutionsPerSecond(qreal revolutionsPerSecond);
    void setNumberOfLines(int lines);
    void setLineLength(int length);
    void setLineWidth(int width);
    void setInnerRadius(int radius);
    void setText(QString text);

    QColor color();
    qreal roundness();
    qreal minimumTrailOpacity();
    qreal trailFadePercentage();
    qreal revolutionsPersSecond();
    int numberOfLines();
    int lineLength();
    int lineWidth();
    int innerRadius();

    bool isSpinning() const;

private slots:
    void rotate();

protected:
    void paintEvent(QPaintEvent *paintEvent);

private:
    static int lineCountDistanceFromPrimary(int current, int primary, int totalNrOfLines);
    static QColor currentLineColor(int distance, int totalNrOfLines, qreal trailFadePerc, qreal minOpacity, QColor color);

    void initialize();
    void updateSize();
    void updateTimer();
    void updatePosition();

private:
    QColor  _color;
    qreal   _roundness; // 0..100
    qreal   _minimumTrailOpacity;
    qreal   _trailFadePercentage;
    qreal   _revolutionsPerSecond;
    int     _numberOfLines;
    int     _lineLength;
    int     _lineWidth;
    int     _innerRadius;

private:
    gnomonSpinner(const gnomonSpinner&);
    gnomonSpinner& operator=(const gnomonSpinner&);

    QTimer *_timer;
    bool    _centerOnParent;
    bool    _disableParentWhenSpinning;
    int     _currentCounter;
    bool    _isSpinning;
};

//
// gnomonSpinner.h ends here
