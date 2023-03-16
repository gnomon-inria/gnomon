#pragma once

#include <QtCore>

class gnomonAbstractDynamicForm;
class gnomonAbstractView;
class gnomonAbstractVisualizationCommand;

class gnomonAbstractViewPrivate : public QObject
{
    Q_OBJECT

public:
    gnomonAbstractViewPrivate(QObject *parent = Q_NULLPTR);
    ~gnomonAbstractViewPrivate(void);

public:
    gnomonAbstractView *q = nullptr;

public slots:
    void exportToManager(void);

public:
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > forms;

public:
    QMap<QString, bool> acceptForms;

public:
    QMap<QString, gnomonAbstractVisualizationCommand *> visualizationCommands;

public:
    bool input_view = false;
};