#pragma once

#include <gnomonWidgetsExport>

#include <QtWidgets>

#include <dtkCore>

class GNOMONWIDGETS_EXPORT gnomonWidgetsParameter
{
public:
    static QWidget *widget(dtkCoreParameter *parameter, QWidget *parent);
};