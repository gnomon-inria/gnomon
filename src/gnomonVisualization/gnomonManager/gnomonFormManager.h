#pragma once

#include <gnomonVisualizationExport>

#include <memory>
#include <QtCore>
#include <QtGui>

#include <gnomonCore/gnomonForm/gnomonAbstractDynamicForm>
#include "gnomonForm/gnomonDynamicFormMetadata.h"

class gnomonAbstractForm;
class gnomonAbstractDynamicForm;
class gnomonAbstractCommand;
class gnomonAbstractVisualization;
class gnomonAbstractMatplotlibVisualization;

class gnomonFormManagerItem;

class vtkCamera;
class vtkGenericOpenGLRenderWindow;

class GNOMONVISUALIZATION_EXPORT gnomonFormManager : public QObject
{
    Q_OBJECT

public:
    static gnomonFormManager *instance(void);

signals:
    void added(int id);

public slots:    
    void addForm(std::shared_ptr<gnomonAbstractDynamicForm>, const QColor&, const QJsonObject &visualization_description,const QImage& image, vtkCamera *cam=nullptr);
    void addForm(std::shared_ptr<gnomonAbstractDynamicForm>, const QColor&, gnomonAbstractMatplotlibVisualization* visualization);
    void addForm(std::shared_ptr<gnomonAbstractDynamicForm>, const QColor&, const QImage& image);

public slots:
    void saveAs(int id, const QString& filename) const;
    void deleteForm(int id);

public:
    Q_INVOKABLE bool contains(int id);
    Q_INVOKABLE gnomonDynamicFormMetadata* getDynamicFormMetadata(int id);

    Q_INVOKABLE QVariantList timeKeys(int id);
    Q_INVOKABLE QStringList formMetadataKeysAtT(int id, double t);
    Q_INVOKABLE QString formMetadataValueAtT(int id, double t, const QString& key);

public:
    Q_INVOKABLE QString formWriterNameFilter(int id);

public:
    int formCount(const QString& form_name);

public:
    std::shared_ptr<gnomonAbstractDynamicForm> get(int index);
    // gnomonAbstractVisualization *getVisualization(int index);
    QJsonObject getVisuDescription(int index);
    vtkCamera *getCamera(int index);

public:
    Q_INVOKABLE QImage thumbnail(int index);

protected:
     gnomonFormManager(QObject *parent = nullptr);
    ~gnomonFormManager(void);

private:
    class gnomonFormManagerPrivate *d;

private:
    static gnomonFormManager *s_instance;
};

//
// gnomonFormManager.h ends here
