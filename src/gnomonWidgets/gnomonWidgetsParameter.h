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

#include <dtkCore>

//class gnomonCoreParameterLookupTable;
//class gnomonCoreParameterColorMap;

// ///////////////////////////////////////////////////////////////////
// gnomonWidgetsParameter
// ///////////////////////////////////////////////////////////////////

class GNOMONWIDGETS_EXPORT gnomonWidgetsParameter
{
public:
    static QWidget *widget(dtkCoreParameter *parameter, QWidget *parent);
};

/*
// ///////////////////////////////////////////////////////////////////
// gnomonWidgetsParameterInt
// ///////////////////////////////////////////////////////////////////

class GNOMONWIDGETS_EXPORT gnomonWidgetsParameterInt
{
public:
    static QString style;
    static QWidget *widget(dtk::d_int *parameter, QWidget *parent);
};

// ///////////////////////////////////////////////////////////////////
// gnomonWidgetsParameterDouble
// ///////////////////////////////////////////////////////////////////

class GNOMONWIDGETS_EXPORT gnomonWidgetsParameterDouble
{
public:
    static QString style;
    static QWidget *widget(dtk::d_real *parameter, QWidget *parent);
};

// ///////////////////////////////////////////////////////////////////
// gnomonWidgetsParameterIntRange
// ///////////////////////////////////////////////////////////////////

class GNOMONWIDGETS_EXPORT gnomonWidgetsParameterIntRange
{
public:
    static QString style;
    static QWidget *widget(dtk::d_range_int *parameter, QWidget *parent);
};

// ///////////////////////////////////////////////////////////////////
// gnomonWidgetsParameterDoubleRange
// ///////////////////////////////////////////////////////////////////

class GNOMONWIDGETS_EXPORT gnomonWidgetsParameterDoubleRange
{
public:
    static QString style;
    static QWidget *widget(dtk::d_range_real *parameter, QWidget *parent);
};

// ///////////////////////////////////////////////////////////////////
// gnomonWidgetsParameterBool
// ///////////////////////////////////////////////////////////////////

class GNOMONWIDGETS_EXPORT gnomonWidgetsParameterBool
{
public:
    static QString style;
    static QWidget *widget(dtk::d_bool *parameter, QWidget *parent);
};


// ///////////////////////////////////////////////////////////////////
// gnomonWidgetsParameterText
// ///////////////////////////////////////////////////////////////////

class GNOMONWIDGETS_EXPORT gnomonWidgetsParameterText
{
public:
    static QString style;
    static QWidget *widget(dtkCoreParameterText *parameter, QWidget *parent);
};


// ///////////////////////////////////////////////////////////////////
// gnomonWidgetsParameterString
// ///////////////////////////////////////////////////////////////////

class GNOMONWIDGETS_EXPORT gnomonWidgetsParameterString
{
public:
    static QString style;
    static QWidget *widget(dtk::d_inliststring *parameter, QWidget *parent);
};

// ///////////////////////////////////////////////////////////////////
// gnomonWidgetsParameterStringList
// ///////////////////////////////////////////////////////////////////

class GNOMONWIDGETS_EXPORT gnomonWidgetsParameterStringList
{
public:
    static QString style;
    static QWidget *widget(dtk::d_inliststring *parameter, QWidget *parent);
};

// ///////////////////////////////////////////////////////////////////
// gnomonWidgetsParameterFile
// ///////////////////////////////////////////////////////////////////

class GNOMONWIDGETS_EXPORT gnomonWidgetsParameterFile
{
public:
    static QString style;
    static QWidget *widget(dtk::d_path *parameter, QWidget *parent);
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

*/