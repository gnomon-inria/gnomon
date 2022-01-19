#pragma once

#include <gnomonWorkspaceExport>

#include <QtCore>
#include <QtQml>

#include <gnomonVisualization/gnomonView/gnomonViewFormList>

class gnomonAbstractCommand;
class gnomonViewForm;

class GNOMONWORKSPACE_EXPORT gnomonAlgorithmWorkspace : public QObject
{
    Q_OBJECT

public:
     gnomonAlgorithmWorkspace(QObject *parent = nullptr);
    virtual ~gnomonAlgorithmWorkspace(void);

public:
    Q_PROPERTY(QString algoName READ algoName WRITE setAlgoName NOTIFY algorithmChanged);
    Q_PROPERTY(QStringList algorithms READ algorithms NOTIFY algorithmsLoaded);
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged);
    Q_PROPERTY(gnomonViewFormList* sources READ sources CONSTANT);
    Q_PROPERTY(gnomonViewFormList* targets READ targets CONSTANT);
    Q_PROPERTY(gnomonViewForm* source READ source CONSTANT); //for ease of use
    Q_PROPERTY(gnomonViewForm* target READ target CONSTANT); //for ease of use
w
    Q_PROPERTY(QJSValue parameters READ parameters NOTIFY parametersChanged)

signals:
    void algorithmsLoaded(void);
    void algorithmChanged(const QString& algorithm);
    void parametersChanged(void);
    void currentIndexChanged(void);

public slots:
    virtual void run(void);
    virtual void setInputs(void);
    virtual void viewOutputs(void);
    void saveState(void);
    void restoreState(void);

public:
    QString algoName(void) const;
    QStringList algorithms(void) const;
    void setAlgoName(const QString &);
    int currentIndex(void) const;
    void setCurrentIndex(int);

public:
    gnomonViewFormList *sources(void) const;
    gnomonViewFormList *targets(void) const;

    //for old compatibility
    gnomonViewForm *source(void) const { return (*this->sources())[0]; };
    gnomonViewForm *target(void) const {return (*this->targets())[0]; };

    QJSValue parameters(void);
    QJsonObject serialize(void);
    void unSerialize(QJsonObject&);

protected:
    class gnomonAlgorithmWorkspacePrivate *d = nullptr;
};

//
// gnomonAlgorithmWorkspace.h ends here
