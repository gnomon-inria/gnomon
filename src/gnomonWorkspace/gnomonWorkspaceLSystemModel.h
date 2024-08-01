#pragma once

#include <gnomonWorkspaceExport>

#include "gnomonAbstractWorkspace.h"

#include <QJSValue>

class gnomonVtkView;
class gnomonQmlView;

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
    Q_PROPERTY(gnomonVtkView* view READ view CONSTANT);
    Q_PROPERTY(gnomonQmlView* textView READ textView CONSTANT);
    Q_PROPERTY(QString message READ message NOTIFY messageChanged)
    Q_PROPERTY(QJSValue parameters READ parameters NOTIFY parametersChanged)
    Q_PROPERTY(QString fileName READ fileName WRITE setFileName NOTIFY fileChanged);
    Q_PROPERTY(QStringList missingTextures READ missingTextures NOTIFY missingTexturesChanged);

signals:
    void textChanged(const QString&);
    void derivationLengthChanged(int);
    void animationStepChanged(int);
    void messageChanged();

    void modelsLoaded(void);
    void modelChanged(const QString& model);
    void fileChanged(const QString& file_name);
    void parametersChanged(void);
    void currentIndexChanged(void);
    void requestOpenFile(const QString& path);
    void missingTexturesChanged(void);

    // TODO: factorize in a code editor workspace class
    void codeEditorReady(void);

public:
    QString message(void) const;

    QString text(void);
    Q_INVOKABLE void setText(const QString& text);
    Q_INVOKABLE void copyTextureFiles(const QStringList& files);
    Q_INVOKABLE void importFile(const QString& file_name, const QString& path);

    int derivationLength(void);
    void setDerivationLength(int l);

    int animationStep(void);
    void setAnimationStep(int s);

public:
    Q_INVOKABLE QUrl defaultReadPath();

public slots:
    void read(const QString& file_url, bool read_only=false, bool restoring=false);
    void save(const QString& file_url) const;
    void close(const QString& file_name);

    void setDefaultLSystem(void);

public slots:
    void step(void);
    void animate(void);
    void run(void);
    void reset(void);
    void stop(void);
    void setAnimationTime(const QString& time);
    void restore(void);

    void setInitialState(void);
    void viewState(void);
    void viewNewStep(void);
    virtual void export_outputs(void) override;

public:
    QString modelName(void) const;
    QStringList models(void) const;
    QString fileName(void) const;
    void setModelName(const QString &);
    void setFileName(const QString &);
    int currentIndex(void) const;
    void setCurrentIndex(int);
    bool backup(void);
    QStringList missingTextures(void) const;

    gnomonVtkView *view(void) const;
    gnomonQmlView *textView(void) const;

    QJSValue parameters(void);
    QStringList parameterNames(void);
    void setParameter(const QString& parameter, const QVariant& value);

    QJsonObject serialize() override;
    void deserialize(const QJsonObject &state) override;
public slots:
    void saveState();
    void restoreState();

protected:
    class gnomonWorkspaceLSystemModelPrivate *d = nullptr;
};
