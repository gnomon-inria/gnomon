#pragma once

#include <gnomonWorkspaceExport>
#include "gnomonAbstractWorkspace.h"

#include <gnomonVisualization/gnomonView/gnomonViewFormList>

#include <QJSValue>

#define MORPHOPLOT_TMP_FILE "edited_seg_img.inr.gz"

class gnomonViewForm;
class gnomonPythonAlgorithmPluginCode;

class GNOMONWORKSPACE_EXPORT gnomonWorkspaceMorphonet : public gnomonAbstractWorkspace
{
    Q_OBJECT
    Q_CLASSINFO("description", "\
This workspace allows to connect to Morphonet and import/export datasets\
\n")

public:
     gnomonWorkspaceMorphonet(QObject *parent = nullptr);
    ~gnomonWorkspaceMorphonet(void);

public:
    Q_PROPERTY(bool connected READ connected NOTIFY connectionStatusChanged)
    Q_PROPERTY(int currentId READ currentId WRITE setCurrentId NOTIFY currentIdChanged);
    Q_PROPERTY(int timeStart READ timeStart WRITE setTimeStart NOTIFY timeStartChanged);
    Q_PROPERTY(int timeEnd   READ timeEnd   WRITE setTimeEnd   NOTIFY timeEndChanged);
    Q_PROPERTY(bool uploadMode READ uploadMode WRITE setUploadMode NOTIFY uploadModeChanged);

    Q_PROPERTY(gnomonViewForm* view READ view CONSTANT);

public:
    Q_INVOKABLE bool login(const QString& login, const QString& passwd);
    bool disconnect(void);
    bool connected(void);

    int currentId(void) const;
    int timeStart(void) const;
    int timeEnd(void) const;
    bool uploadMode(void) const;

    void setCurrentId(int);
    void setTimeStart(int);
    void setTimeEnd(int);
    void setUploadMode(bool);

    Q_INVOKABLE bool deleteDataset(int id=-1);
    Q_INVOKABLE QString datasetsInfo(const QString& search = "");
    Q_INVOKABLE void importDatasetPreview(int id, double voxelsize);
    Q_INVOKABLE void importDataset(int time_start, int time_end, int id, double voxelsize);
    Q_INVOKABLE int importDatasetInfos(void);
    Q_INVOKABLE int exportDataset(QString name, int id_NCBI, int id_type, QString description, double voxelsize);
    Q_INVOKABLE int morphoPlot(void);
    Q_INVOKABLE void morphoPlotCollect(void);

    Q_INVOKABLE gnomonViewForm *view(void);

signals:
    void currentIdChanged(void);
    void started(void);
    void timeStartChanged(void);
    void timeEndChanged(void);
    void connectionStatusChanged(void);
    void uploadModeChanged(void);
    void message(const QString& msg);

public slots:
    void saveState(void);
    void restoreState(void);


private:
    class gnomonWorkspaceMorphonetPrivate *d;
};