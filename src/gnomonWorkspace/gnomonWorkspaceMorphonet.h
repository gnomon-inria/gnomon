#pragma once

#include <gnomonWorkspaceExport>
#include "gnomonAbstractWorkspace.h"

#include <gnomonVisualization/gnomonView/gnomonViewFormList>

#include <QJSValue>

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

    Q_PROPERTY(gnomonViewForm* view READ view CONSTANT);

public:
    Q_INVOKABLE bool login(const QString& login, const QString& passwd);
    bool disconnect(void);
    bool connected(void);

    int currentId(void) const;
    int timeStart(void) const;
    int timeEnd(void) const;

    void setCurrentId(int);
    void setTimeStart(int);
    void setTimeEnd(int);

    Q_INVOKABLE QString datasetsInfo(const QString& search = "");
    Q_INVOKABLE void importDatasetPreview(int id=-1);
    Q_INVOKABLE void importDataset(int time_start, int time_end, int id=-1);
    Q_INVOKABLE void exportDataset();

    Q_INVOKABLE gnomonViewForm *view(void);

signals:
    void currentIdChanged(void);
    void timeStartChanged(void);
    void timeEndChanged(void);
    void connectionStatusChanged();
    void message(const QString& msg);

private:
    class gnomonWorkspaceMorphonetPrivate *d;
};