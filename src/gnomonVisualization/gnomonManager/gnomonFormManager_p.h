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

#include <QtCore>
#include <QtWidgets>

class gnomonAbstractDynamicForm;
class gnomonAbstractCommand;
class gnomonAbstractWriterCommand;
class gnomonAbstractVisualization;
class gnomonAbstractMatplotlibVisualization;

class gnomonFormManager;
class gnomonFormManagerItem;
class gnomonFormManagerData;
class gnomonViewForm;

class gnomonPipeline;

class vtkCamera;

class gnomonFormManagerPrivate : public QObject
{
    Q_OBJECT

public:
     gnomonFormManagerPrivate(QObject *parent = nullptr);
    ~gnomonFormManagerPrivate(void);

public:
    QHash<int, gnomonAbstractDynamicForm *> forms;
    // QHash<int, gnomonAbstractVisualization *> formVisualizations;
    QHash<int, QJsonObject> visualization_description;
    QHash<int, gnomonAbstractMatplotlibVisualization *> formMatplotlibVisualizations;
    QHash<int, gnomonAbstractWriterCommand *> formWriterCommand;
    QHash<int, QImage> formData;
    QHash<int, vtkCamera *> formCameras;
    QHash<QString, gnomonAbstractWriterCommand *> commands;

public:
    gnomonFormManager *q;

public:
    gnomonPipeline *pipeline = nullptr;

public:
    gnomonViewForm *view = nullptr;

public:
    static int item_counter;

public:
        QMetaObject::Connection connection;
};

//
// gnomonFormManager_p.h ends here
