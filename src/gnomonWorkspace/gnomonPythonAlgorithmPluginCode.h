#pragma once

#include <gnomonWorkspaceExport>

#include <gnomonCore>

class GNOMONWORKSPACE_EXPORT gnomonPythonAlgorithmPluginCode : public QObject
{
    Q_OBJECT

public:
     gnomonPythonAlgorithmPluginCode(QObject *parent = Q_NULLPTR);
    ~gnomonPythonAlgorithmPluginCode(void);

public:
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)

    Q_PROPERTY(QString pluginName READ pluginName WRITE setPluginName NOTIFY pluginNameChanged)
    Q_PROPERTY(QString pluginDocumentation READ pluginDocumentation WRITE setPluginDocumentation NOTIFY pluginDocumentationChanged)

    Q_PROPERTY(QJsonObject inputForms READ inputFormsJson NOTIFY inputFormsChanged)
    Q_PROPERTY(QJsonObject outputForms READ outputFormsJson NOTIFY outputFormsChanged)
    Q_PROPERTY(QJsonObject parameters READ parametersJson NOTIFY parametersChanged)

    Q_PROPERTY(QStringList formTypes READ formTypes CONSTANT)
    Q_PROPERTY(QStringList parameterTypes READ parameterTypes CONSTANT)

public:
    QString text(void);
    void setText(const QString& text);

public:
    QJsonObject inputFormsJson(void) const;
    QJsonObject outputFormsJson(void) const;
    QJsonObject parametersJson(void) const;

signals:
    void textChanged(const QString&);

    void pluginNameChanged(void);
    void pluginDocumentationChanged(void);

    void inputFormsChanged(void);
    void outputFormsChanged(void);
    void parametersChanged(void);
//    void openButtonClicked(void);
//    void saveButtonClicked(void);
//    void loadButtonClicked(void);

public:
    const QString& pluginName(void) const;
    void setPluginName(const QString& name);

public:
    const QString& pluginDocumentation(void) const;
    void setPluginDocumentation(const QString& doc);

public:
    const QMap<QString, gnomonFormDescription>& inputForms(void) const;
    const QMap<QString, gnomonFormDescription>& outputForms(void) const;
    const QMap<QString, gnomonParameterDescription>& parameters(void) const;

public slots:
    void updateDataPlugins(const QString& form_type);
    Q_INVOKABLE QStringList formDataPlugins(const QString& form_type);

    QStringList formTypes() const;
    QStringList parameterTypes() const;

    void addInputForm(gnomonFormDescription desc, bool update_code=true);
    void addOutputForm(gnomonFormDescription desc, bool update_code=true);
    void addParameter(gnomonParameterDescription desc, bool update_code=true);

    Q_INVOKABLE void addInputForm(const QJsonObject& desc);
    Q_INVOKABLE void addOutputForm(const QJsonObject& desc);
    Q_INVOKABLE void addParameter(const QJsonObject& desc);

public slots:
    void updateCode(void);
    void parseCode(void);

public:
    class gnomonPythonAlgorithmPluginCodePrivate *d;
};


//
// gnomonPythonAlgorithmPluginCode.h ends here