#pragma once

#include <gnomonCoreExport>

#include "gnomonAbstractCommand.h"

class gnomonAbstractDynamicForm;
class gnomonAbstractFormAlgorithm;

// TODO reemove ? is it used somewhere?
// NOTE: Good question !
class GNOMONCORE_EXPORT gnomonFormAlgorithmCommand : public gnomonAbstractCommand
{
public:
     gnomonFormAlgorithmCommand(void) = delete;
     gnomonFormAlgorithmCommand(const QString&);
    ~gnomonFormAlgorithmCommand(void) override;

public:
    void  predo(void) override;
    void postdo(void) override;
    void   undo(void) override;

public:
    void addInput(std::shared_ptr<gnomonAbstractDynamicForm> input);
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > inputs() override;

    void addOutput(std::shared_ptr<gnomonAbstractDynamicForm> output);
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs() override;

    void addParameter(const QString&, dtkCoreParameter *parameter);

public:
    dtkCoreParameters parameters() const override;

public:
    virtual void setAlgorithmName(const QString &name) override;

public:
    void setPythonCode(const QString& code);
    const QString& pythonCode(void) const;

    void setFormAlgorithm(gnomonAbstractFormAlgorithm *algo);
    gnomonAbstractFormAlgorithm *formAlgorithm(void);

    virtual orderedMap inputTypes() override;
    virtual void setInputForm(const QString& name, std::shared_ptr<gnomonAbstractDynamicForm> form) override;

    virtual orderedMap outputTypes() override;

    void deserializeResults(QJsonObject &serialization) override;

    QJsonObject serializeResults(void) override;

private:
    class gnomonFormAlgorithmCommandPrivate *d;
};
