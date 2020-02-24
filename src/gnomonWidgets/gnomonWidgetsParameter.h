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

#include <gnomonCore>

class gnomonCoreParameterLookupTable;
class gnomonCoreParameterColorMap;

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
// gnomonWidgetsParameterIntRange
// ///////////////////////////////////////////////////////////////////

class GNOMONWIDGETS_EXPORT gnomonWidgetsParameterIntRange
{
public:
    static QString style;
    static QWidget *widget(gnomonCoreParameterIntRange *parameter, QWidget *parent);
};

// ///////////////////////////////////////////////////////////////////
// gnomonWidgetsParameterDoubleRange
// ///////////////////////////////////////////////////////////////////

class GNOMONWIDGETS_EXPORT gnomonWidgetsParameterDoubleRange
{
public:
    static QString style;
    static QWidget *widget(gnomonCoreParameterDoubleRange *parameter, QWidget *parent);
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
// gnomonWidgetsParameterText
// ///////////////////////////////////////////////////////////////////

class GNOMONWIDGETS_EXPORT gnomonWidgetsParameterText
{
public:
    static QString style;
    static QWidget *widget(gnomonCoreParameterText *parameter, QWidget *parent);
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

// ///////////////////////////////////////////////////////////////////
// gnomonWidgetsParameterFile
// ///////////////////////////////////////////////////////////////////

class GNOMONWIDGETS_EXPORT gnomonWidgetsParameterFile
{
public:
    static QString style;
    static QWidget *widget(gnomonCoreParameterFile *parameter, QWidget *parent);
};

// ///////////////////////////////////////////////////////////////////
// gnomonWidgetsParameterColorMap
// ///////////////////////////////////////////////////////////////////

class GNOMONWIDGETS_EXPORT gnomonWidgetsParameterColorMap
{
public:
    static QString style;
    static QWidget *widget(gnomonCoreParameterColorMap *parameter, QWidget *parent);
};

// ///////////////////////////////////////////////////////////////////
// gnomonWidgetsParameterLookupTable
// ///////////////////////////////////////////////////////////////////

class GNOMONWIDGETS_EXPORT gnomonWidgetsParameterLookupTable
{
public:
    static QString style;
    static QWidget *widget(gnomonCoreParameterLookupTable *parameter, QWidget *parent);
};

//
// gnomonWidgetsParameter.h ends here
