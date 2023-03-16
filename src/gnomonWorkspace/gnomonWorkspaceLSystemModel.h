#pragma once

#include <gnomonWorkspaceExport>

#include "gnomonAbstractWorkspace.h"

#include <QJSValue>

class gnomonViewForm;


class GNOMONWORKSPACE_EXPORT gnomonWorkspaceLSystemModel : public gnomonAbstractWorkspace
{
    Q_OBJECT
    Q_CLASSINFO("description", "\
This workspace allows to compute a simulate a LString series using \
a L-System model using the L-Py language.\n\
\n")

public:
     gnomonWorkspaceLSystemModel(QObject *parent = nullptr);
    ~gnomonWorkspaceLSystemModel(void);

public:
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
    Q_PROPERTY(int derivationLength READ derivationLength WRITE setDerivationLength NOTIFY derivationLengthChanged)
    Q_PROPERTY(int animationStep READ animationStep WRITE setAnimationStep NOTIFY animationStepChanged)

    Q_PROPERTY(QString modelName READ modelName WRITE setModelName NOTIFY modelChanged);
    Q_PROPERTY(QStringList models READ models NOTIFY modelsLoaded);
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged);
    Q_PROPERTY(gnomonViewForm* view READ view CONSTANT);
    Q_PROPERTY(QString message READ message NOTIFY messageChanged)
    Q_PROPERTY(QJSValue parameters READ parameters NOTIFY parametersChanged)

signals:
    void textChanged(const QString&);
    void derivationLengthChanged(int);
    void animationStepChanged(int);
    void messageChanged();

    void modelsLoaded(void);
    void modelChanged(const QString& model);
    void parametersChanged(void);
    void currentIndexChanged(void);

public:
    QString message(void) const;

    QString text(void);
    void setText(const QString& text);

    int derivationLength(void);
    void setDerivationLength(int l);

    int animationStep(void);
    void setAnimationStep(int s);

public:
    Q_INVOKABLE QUrl defaultReadPath();

public slots:
    void read(const QString& file_url);
    void save(const QString& file_url) const;

public slots:
    void step(void);
    void animate(void);
    void run(void);
    void reset(void);
    void stop(void);
    void setAnimationTime(const QString& time);

    void setInitialState(void);
    void viewState(void);
    void viewNewStep(void);
    virtual void export_outputs(void) override;

public:
    QString modelName(void) const;
    QStringList models(void) const;
    void setModelName(const QString &);
    int currentIndex(void) const;
    void setCurrentIndex(int);

    gnomonViewForm *view(void) const;

    QJSValue parameters(void);

protected:
    class gnomonWorkspaceLSystemModelPrivate *d = nullptr;
};
