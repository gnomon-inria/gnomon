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

#include <gnomonCore/gnomonCoreParameter.h>

// ///////////////////////////////////////////////////////////////////
// gnomonWidgetsParameter
// ///////////////////////////////////////////////////////////////////

class GNOMONWIDGETS_EXPORT gnomonWidgetsParameter
{
public:
    static QWidget *widget(gnomonCoreParameter *parameter, QWidget *parent);
};

// ///////////////////////////////////////////////////////////////////
// gnomonWidgetsParameterInt
// ///////////////////////////////////////////////////////////////////

class GNOMONWIDGETS_EXPORT gnomonWidgetsParameterInt
{
public:
    static QString style;
    static QWidget *widget(gnomonCoreParameterInt *parameter, QWidget *parent);
};

// ///////////////////////////////////////////////////////////////////
// gnomonWidgetsParameterDouble
// ///////////////////////////////////////////////////////////////////

class GNOMONWIDGETS_EXPORT gnomonWidgetsParameterDouble
{
public:
    static QString style;
    static QWidget *widget(gnomonCoreParameterDouble *parameter, QWidget *parent);
};

// ///////////////////////////////////////////////////////////////////
// gnomonWidgetsParameterBool
// ///////////////////////////////////////////////////////////////////

class GNOMONWIDGETS_EXPORT gnomonWidgetsParameterBool
{
public:
    static QString style;
    static QWidget *widget(gnomonCoreParameterBool *parameter, QWidget *parent);
};

// ///////////////////////////////////////////////////////////////////
// gnomonWidgetsParameterString
// ///////////////////////////////////////////////////////////////////

class GNOMONWIDGETS_EXPORT gnomonWidgetsParameterString
{
public:
    static QString style;
    static QWidget *widget(gnomonCoreParameterString *parameter, QWidget *parent);
};

// ///////////////////////////////////////////////////////////////////
// gnomonWidgetsParameterStringList
// ///////////////////////////////////////////////////////////////////

class GNOMONWIDGETS_EXPORT gnomonWidgetsParameterStringList
{
public:
    static QString style;
    static QWidget *widget(gnomonCoreParameterStringList *parameter, QWidget *parent);
};

//
// gnomonWidgetsParameter.h ends here
