#pragma once

#include <gnomonCore/gnomonCommand/gnomonAbstractCommand>

#include <gnomonCore/gnomonForm/gnomonLString/gnomonLString>
#include <gnomonCore/gnomonModel/gnomonAbstractEvolutionModel>

class GNOMONCORE_EXPORT gnomonLStringEvolutionModelCommand : public QObject
{
Q_OBJECT

public:
     gnomonLStringEvolutionModelCommand(void);
    ~gnomonLStringEvolutionModelCommand(void) override;

public slots:
    virtual void  predo(void);
    virtual void postdo(void);
    virtual void   undo(void);
    virtual void   redo(void);

public:
    void setInitialState(std::shared_ptr<gnomonLStringSeries> lstring);
    std::shared_ptr<gnomonLStringSeries> initialState(void);

    std::shared_ptr<gnomonLStringSeries> state(void);

public:
    void setLSystem(const QString& code);

public:
    const QString& modelName(void) { return this->model_name; }
    virtual void setModelName(const QString& name);

    const QString& factoryName(void) { return this->factory_name; }

public:
    static bool isEmpty();
    inline static const QString groupName = "lStringEvolutionModel";
    static QStringList availablePlugins();

public:
    inline virtual dtkCoreParameters parameters() const { return this->model->parameters(); };
    inline virtual void setParameter(const QString& parameter, const QVariant& value) { this->model->setParameter(parameter, value); }
    inline virtual QMap<QString, QString> parameterGroups() const { return this->model->parameterGroups(); };

protected:
    class gnomonAbstractModel *model = nullptr;
    QString model_name = "";
    QString factory_name = "";

private:
    class gnomonLStringEvolutionModelCommandPrivate *d;
};
