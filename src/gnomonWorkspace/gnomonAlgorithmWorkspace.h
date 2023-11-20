#pragma once

#include <gnomonWorkspaceExport>
#include "gnomonAbstractWorkspace.h"

#include <QJSValue>

#include <gnomonVisualization/gnomonView/gnomonVtkViewList>
#include <gnomonVisualization/gnomonView/gnomonVtkView.h>
#include <gnomonVisualization/gnomonView/gnomonMplView.h>
#include <gnomonVisualization/gnomonView/gnomonQmlView.h>
#include <qtmetamacros.h>

class gnomonAbstractAlgorithmCommand;

class GNOMONWORKSPACE_EXPORT gnomonAlgorithmWorkspace : public gnomonAbstractWorkspace
{
    Q_OBJECT

public:
             gnomonAlgorithmWorkspace(QObject *parent = nullptr);
    virtual ~gnomonAlgorithmWorkspace(void);

public:
    Q_PROPERTY(QString algoName READ algoName WRITE setAlgoName NOTIFY algorithmChanged);
    Q_PROPERTY(QStringList algorithms READ algorithms NOTIFY algorithmsLoaded);
    Q_PROPERTY(QVariantList algorithmsData READ algorithmsData NOTIFY algorithmsLoaded);
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged);
    Q_PROPERTY(gnomonVtkViewList* sources READ sources CONSTANT);
    Q_PROPERTY(gnomonVtkViewList* targets READ targets CONSTANT);
    Q_PROPERTY(gnomonVtkView* source READ source CONSTANT); //for ease of use
    Q_PROPERTY(gnomonVtkView* target READ target CONSTANT); //for ease of use
    Q_PROPERTY(gnomonMplView* figure READ figure CONSTANT);
    Q_PROPERTY(gnomonQmlView* textView READ textView CONSTANT);
    Q_PROPERTY(int progress READ progress NOTIFY progressChanged);
    Q_PROPERTY(QJSValue parameters READ parameters NOTIFY parametersChanged)

signals:
    void algorithmsLoaded(void);
    void algorithmChanged(const QString& algorithm);
    void parametersChanged(void);
    void currentIndexChanged(void);
    void progressChanged(int progress);

public slots:
    virtual void run(bool no_async=false);
    virtual void setInputs(void);
    virtual void viewOutputs(void);
    virtual void saveState(void);
    virtual void restoreState(void);
    virtual void export_outputs(void) override;

public slots:
    virtual void pause(void);
    virtual void resume(void);
    virtual void stop(void);

public:
    QString algoName(void) const;
    QStringList algorithms(void) const;
    QVariantList algorithmsData(void) const;
    void setAlgoName(const QString &);
    int currentIndex(void) const;
    void setCurrentIndex(int);
    int progress(void);

public:
    gnomonVtkViewList *sources(void) const;
    virtual gnomonVtkViewList *targets(void) const;

    //for old compatibility
    gnomonVtkView *source(void) const { return (*this->sources())[0]; };
    gnomonVtkView *target(void) const {return (*this->targets())[0]; };

    gnomonMplView *figure(void) const;
    gnomonQmlView *textView(void) const;

    QJSValue parameters(void);
    QJsonObject serialize(void) override;
    void unSerialize(const QJsonObject&) override;

protected:
    void addInputView(const QVector<QString>& accepted_forms = {}, QStringList nodePortNames = {});
    void addOutputView(const QVector<QString>& accepted_forms = {}, QStringList nodePortNames = {});

protected:
    class gnomonAlgorithmWorkspacePrivate *d = nullptr;
};

//
// gnomonAlgorithmWorkspace.h ends here
