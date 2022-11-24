#pragma once

#include <gnomonVisualizationExport>

#include <memory>
#include <QtCore>
#include <QtGui>

#include <gnomonCore/gnomonForm/gnomonAbstractDynamicForm>
#include "gnomonForm/gnomonDynamicFormMetadata.h"
#include "gnomonVisualizations/gnomonAbstractMatplotlibVisualization"
#include "gnomonVisualizations/gnomonAbstractVisualization"

class gnomonAbstractForm;
class gnomonAbstractDynamicForm;
class gnomonAbstractCommand;
//class gnomonAbstractVisualization;
//class gnomonAbstractMatplotlibVisualization;

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
    void removed(int id);
    void alreadyAdded(void);

public slots:    
    void addForm(std::shared_ptr<gnomonAbstractDynamicForm>, std::shared_ptr<gnomonAbstractVisualization> visualization,const QImage& image, vtkCamera *cam=nullptr);
    void addForm(std::shared_ptr<gnomonAbstractDynamicForm>, std::shared_ptr<gnomonAbstractMatplotlibVisualization> visualization);
    void addForm(std::shared_ptr<gnomonAbstractDynamicForm>, const QImage& image);

public slots:
    void saveAs(int id, const QString& filename, bool add_to_pipeline = true) const;
    bool deleteForm(int id, bool force = false);
    void compose(int first, int second);
    void addToCache(int id) const;
    void loadFromCache(int id) const;

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
    void setFormDropped(std::shared_ptr<gnomonAbstractDynamicForm> form);

public:
    std::shared_ptr<gnomonAbstractDynamicForm> get(int index);
    std::shared_ptr<gnomonAbstractVisualization> getVisualization(int index);
    //QJsonObject getVisuDescription(int index);
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
