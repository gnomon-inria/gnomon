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

class gnomonFormManagerItem;

class vtkCamera;
class vtkGenericOpenGLRenderWindow;

class GNOMONVISUALIZATION_EXPORT gnomonFormManager : public QObject
{
    Q_OBJECT

public:
    static gnomonFormManager *instance(void);

signals:
    void added(int id, QString name);
    void removed(int id);
    void alreadyAdded(void);

public slots:
    // void addForm(std::shared_ptr<gnomonAbstractDynamicForm>, const QImage& image, std::shared_ptr<gnomonAbstractVisualization> visualization = nullptr);
    void addForm(const QString&, const QImage& image, std::shared_ptr<gnomonAbstractVisualization> visualization = nullptr);

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
    void setFormDropped(const QString& form_uuid);

public:
    int formIndex(const QString& form_uuid);
    QString get(int index);
    std::shared_ptr<gnomonAbstractVisualization> getVisualization(int index);

public:
    void setCamera(int index, vtkCamera *cam);
    vtkCamera *getCamera(int index);

public:
    Q_INVOKABLE QImage thumbnail(int index);

public:
    QJsonObject dumpState(void);
    void loadState(const QJsonObject& state);

protected:
     gnomonFormManager(QObject *parent = nullptr);
    ~gnomonFormManager(void);

private:
    class gnomonFormManagerPrivate *d;

};

//
// gnomonFormManager.h ends here
