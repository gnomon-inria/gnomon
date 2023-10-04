#pragma once

#include <QtCore>

class gnomonAbstractDynamicForm;
class gnomonAbstractView;
class gnomonAbstractVisualizationCommand;

class gnomonAbstractViewPrivate : public QObject
{
    Q_OBJECT

public:
    typedef struct {
        QString currentFormType;
        int currentFormIndex = -1;
        QMap<QString, QString> visuSelected;  // form_type --> visu_name
        QMap<QString, QVariantMap> parameters; // visu_name --> parameters

    } ViewParameters;

public:
    gnomonAbstractViewPrivate(QObject *parent = Q_NULLPTR);
    ~gnomonAbstractViewPrivate(void);

public slots:
    void exportToManager(void);
    void setFormVisualization(const QString& form_type, const QString& visu_name, const QVariantMap &parameters = {});

    void updateFormsTimes(void);

public:
    gnomonAbstractView *q = nullptr;

public:
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > forms;

public:
    QStringList nodePortNames;

public:
    QMap<QString, bool> acceptForms;

public:
    QMap<QString, gnomonAbstractVisualizationCommand *> visualizationCommands;
    ViewParameters viewParameters;

public:
    bool input_view = false;

public:
    QSet<double> forms_times;
    double current_time = 0.;

};