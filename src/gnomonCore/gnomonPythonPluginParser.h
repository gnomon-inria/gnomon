#pragma once

#include <gnomonCoreExport.h>

#include <QtCore>


class GNOMONCORE_EXPORT gnomonFormDescription
{
public:
     gnomonFormDescription(void) = default;
     gnomonFormDescription(const QString& name, const QString& type, const QString& data_plugin) : name(name), type(type), data_plugin(data_plugin) {};;
    ~gnomonFormDescription(void) = default;

public:
    QString name;
    QString type;
    QString data_plugin;
};


class GNOMONCORE_EXPORT gnomonParameterDescription
{
public:
     gnomonParameterDescription(void) = default;
     gnomonParameterDescription(const QString& name, const QString& type, const QString& doc, const QString& value, const QJsonObject& args) : name(name), type(type), doc(doc), value(value), args(args) {};
    ~gnomonParameterDescription(void) = default;

public:
    QString name;
    QString type;
    QString doc;
    QString value;
    QJsonObject args;
};


GNOMONCORE_EXPORT QString argumentValue(const QString& arguments, const QString& argument_name, int argument_position=0);

GNOMONCORE_EXPORT QString capitalize(const QString& str);
GNOMONCORE_EXPORT QString unCapitalize(const QString& str);

GNOMONCORE_EXPORT QString stripQuotes(const QString& str);

class GNOMONCORE_EXPORT gnomonPythonPluginParser
{
public:
     gnomonPythonPluginParser(void);
    ~gnomonPythonPluginParser(void);

public:
    const QString& pluginName(void) const;
    const QString& pluginClassName(void) const;
    const QString& pluginDocumentation(void) const;

public:
    const QMap<QString, gnomonFormDescription>& inputForms(void) const;
    const QMap<QString, gnomonFormDescription>& outputForms(void) const;
    const QMap<QString, gnomonParameterDescription>& parameters(void) const;

public:
    const QMap<QString, QString>& parameterTypes(void) const;
    const QMap<QString, QString>& defaultFormDataPlugins(void) const;

public:
    void parsePluginCode(const QString&);

private:
    class gnomonPythonPluginParserPrivate *d;
};



