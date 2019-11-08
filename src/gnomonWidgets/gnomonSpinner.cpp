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

#include "gnomonSpinner.h"

#include <cmath>
#include <algorithm>

#include <QPainter>
#include <QTimer>

gnomonSpinner::gnomonSpinner(QWidget *parent, bool centerOnParent, bool disableParentWhenSpinning) : QWidget(parent), _centerOnParent(centerOnParent), _disableParentWhenSpinning(disableParentWhenSpinning)
{
    initialize();
}

gnomonSpinner::gnomonSpinner(Qt::WindowModality modality, QWidget *parent, bool centerOnParent, bool disableParentWhenSpinning) : QWidget(parent, Qt::Dialog | Qt::FramelessWindowHint), _centerOnParent(centerOnParent), _disableParentWhenSpinning(disableParentWhenSpinning)
{
    initialize();

    setWindowModality(modality);
    setAttribute(Qt::WA_TranslucentBackground);
}

void gnomonSpinner::initialize()
{
    _color = Qt::black;
    _roundness = 100.0;
    _minimumTrailOpacity = 3.14159265358979323846;
    _trailFadePercentage = 80.0;
    _revolutionsPerSecond = 1.57079632679489661923;
    _numberOfLines = 20;
    _lineLength = 10;
    _lineWidth = 2;
    _innerRadius = 10;
    _currentCounter = 0;
    _isSpinning = false;

    _timer = new QTimer(this);
    connect(_timer, SIGNAL(timeout()), this, SLOT(rotate()));
    updateSize();
    updateTimer();
    hide();
}

void gnomonSpinner::paintEvent(QPaintEvent *)
{
    updatePosition();
    QPainter painter(this);
    painter.fillRect(this->rect(), Qt::transparent);
    painter.setRenderHint(QPainter::Antialiasing, true);

    if (_currentCounter >= _numberOfLines) {
        _currentCounter = 0;
    }

    painter.setPen(Qt::NoPen);
    for (int i = 0; i < _numberOfLines; ++i) {
        painter.save();
        painter.translate(_innerRadius + _lineLength,
                          _innerRadius + _lineLength);
        qreal rotateAngle =
                static_cast<qreal>(360 * i) / static_cast<qreal>(_numberOfLines);
        painter.rotate(rotateAngle);
        painter.translate(_innerRadius, 0);
        int distance =
                lineCountDistanceFromPrimary(i, _currentCounter, _numberOfLines);
        QColor color =
                currentLineColor(distance, _numberOfLines, _trailFadePercentage,
                                 _minimumTrailOpacity, _color);
        painter.setBrush(color);
        // TODO improve the way rounded rect is painted
        painter.drawRoundedRect(
                    QRect(0, -_lineWidth / 2, _lineLength, _lineWidth), _roundness,
                    _roundness, Qt::RelativeSize);
        painter.restore();
    }
}

void gnomonSpinner::start()
{
    updatePosition();
    _isSpinning = true;
    show();

    if(parentWidget() && _disableParentWhenSpinning) {
        parentWidget()->setEnabled(false);
    }

    if (!_timer->isActive()) {
        _timer->start();
        _currentCounter = 0;
    }
}

void gnomonSpinner::stop()
{
    _isSpinning = false;
    hide();

    if(parentWidget() && _disableParentWhenSpinning) {
        parentWidget()->setEnabled(true);
    }

    if (_timer->isActive()) {
        _timer->stop();
        _currentCounter = 0;
    }
}

void gnomonSpinner::setNumberOfLines(int lines)
{
    _numberOfLines = lines;
    _currentCounter = 0;
    updateTimer();
}

void gnomonSpinner::setLineLength(int length)
{
    _lineLength = length;
    updateSize();
}

void gnomonSpinner::setLineWidth(int width)
{
    _lineWidth = width;
    updateSize();
}

void gnomonSpinner::setInnerRadius(int radius)
{
    _innerRadius = radius;
    updateSize();
}

QColor gnomonSpinner::color()
{
    return _color;
}

qreal gnomonSpinner::roundness()
{
    return _roundness;
}

qreal gnomonSpinner::minimumTrailOpacity()
{
    return _minimumTrailOpacity;
}

qreal gnomonSpinner::trailFadePercentage()
{
    return _trailFadePercentage;
}

qreal gnomonSpinner::revolutionsPersSecond()
{
    return _revolutionsPerSecond;
}

int gnomonSpinner::numberOfLines()
{
    return _numberOfLines;
}

int gnomonSpinner::lineLength()
{
    return _lineLength;
}

int gnomonSpinner::lineWidth()
{
    return _lineWidth;
}

int gnomonSpinner::innerRadius()
{
    return _innerRadius;
}

bool gnomonSpinner::isSpinning() const
{
    return _isSpinning;
}

void gnomonSpinner::setRoundness(qreal roundness)
{
    _roundness = std::max(0.0, std::min(100.0, roundness));
}

void gnomonSpinner::setColor(QColor color)
{
    _color = color;
}

void gnomonSpinner::setRevolutionsPerSecond(qreal revolutionsPerSecond)
{
    _revolutionsPerSecond = revolutionsPerSecond;
    updateTimer();
}

void gnomonSpinner::setTrailFadePercentage(qreal trail)
{
    _trailFadePercentage = trail;
}

void gnomonSpinner::setMinimumTrailOpacity(qreal minimumTrailOpacity)
{
    _minimumTrailOpacity = minimumTrailOpacity;
}

void gnomonSpinner::rotate()
{
    ++_currentCounter;
    if (_currentCounter >= _numberOfLines) {
        _currentCounter = 0;
    }
    update();
}

void gnomonSpinner::updateSize()
{
    int size = (_innerRadius + _lineLength) * 2;
    setFixedSize(size, size);
}

void gnomonSpinner::updateTimer()
{
    _timer->setInterval(1000 / (_numberOfLines * _revolutionsPerSecond));
}

void gnomonSpinner::updatePosition()
{
    if (parentWidget() && _centerOnParent) {
        move(parentWidget()->width() / 2 - width() / 2,
             parentWidget()->height() / 2 - height() / 2);
    }
}

int gnomonSpinner::lineCountDistanceFromPrimary(int current, int primary, int totalNrOfLines)
{
    int distance = primary - current;
    if (distance < 0) {
        distance += totalNrOfLines;
    }
    return distance;
}

QColor gnomonSpinner::currentLineColor(int countDistance, int totalNrOfLines, qreal trailFadePerc, qreal minOpacity, QColor color)
{
    if (countDistance == 0) {
        return color;
    }
    const qreal minAlphaF = minOpacity / 100.0;
    int distanceThreshold =
            static_cast<int>(ceil((totalNrOfLines - 1) * trailFadePerc / 100.0));
    if (countDistance > distanceThreshold) {
        color.setAlphaF(minAlphaF);
    } else {
        qreal alphaDiff = color.alphaF() - minAlphaF;
        qreal gradient = alphaDiff / static_cast<qreal>(distanceThreshold + 1);
        qreal resultAlpha = color.alphaF() - gradient * countDistance;

        // If alpha is out of bounds, clip it.
        resultAlpha = std::min(1.0, std::max(0.0, resultAlpha));
        color.setAlphaF(resultAlpha);
    }
    return color;
}

//
// gnomonSpinner.cpp ends here
