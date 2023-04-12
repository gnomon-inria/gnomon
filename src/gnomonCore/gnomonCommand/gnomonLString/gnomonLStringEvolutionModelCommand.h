#pragma once

#include <gnomonCore/gnomonCommand/gnomonAbstractEvolutionModelCommand>

#include <gnomonCore/gnomonForm/gnomonLString/gnomonLString>
#include <gnomonCore/gnomonModel/gnomonAbstractEvolutionModel>

enum class SimulationType {step = 1, run = 2, animate = 3};
class GNOMONCORE_EXPORT gnomonLStringEvolutionModelCommand : public gnomonAbstractEvolutionModelCommand
{
    Q_OBJECT

public:
     gnomonLStringEvolutionModelCommand(void);
    ~gnomonLStringEvolutionModelCommand(void) override;

public slots:
    virtual void  predo(void) override;
    virtual void postdo(void) override;
    virtual void   undo(void) override;
    // virtual void   redo() override;
    virtual inline void redo(void) override{
        this->redo(nullptr, nullptr);
    }
    virtual QFuture<int> redo(QMutex *mutex, QWaitCondition * synchro);

signals:
    void finished(void);
    void stepFinished(int);

public:
    void setAxiom(std::shared_ptr<gnomonLStringSeries> lstring);
    std::shared_ptr<gnomonLStringSeries> axiom(void);

    std::shared_ptr<gnomonLStringSeries> lString(void);

public:
    void setLSystem(const QString& code);
    const QString& lSystemCode(void) const;

    int derivationLength(void) const;
    void setDerivationLength(int);

    int animationStep(void) const;
    void setAnimationStep(int);
    void setAnimationTime(double t);

    SimulationType simulationType;

public:
    virtual void setModelName(const QString& name) override;

public:
    virtual QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > initialState() override;
    virtual gnomonAbstractCommand::orderedMap initialStateTypes() override;
    virtual void setInitialState(const QString& name, std::shared_ptr<gnomonAbstractDynamicForm> form) override;

    virtual QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > state() override;
    virtual gnomonAbstractCommand::orderedMap stateTypes() override;

public:
    static bool isEmpty();
    inline static const QString groupName = "lStringEvolutionModel";
    static QStringList availablePlugins();

private:
    class gnomonLStringEvolutionModelCommandPrivate *d;
};
