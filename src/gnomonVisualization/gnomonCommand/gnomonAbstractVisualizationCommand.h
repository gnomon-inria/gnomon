#pragma once

#include <gnomonVisualizationExport>

#include "gnomonCore/gnomonCommand/gnomonAbstractCommand.h"
#include <gnomonVisualization/gnomonVisualizations/gnomonAbstractVisualization>

class gnomonAbstractDynamicForm;
class gnomonAbstractView;

class GNOMONCORE_EXPORT gnomonAbstractVisualizationCommand : public gnomonAbstractCommand
{
    Q_OBJECT

public:
    const QString& visualizationName(void)
    {
        return this->visu_name;
    }
    virtual void setVisualizationName(const QString &);

public:
    virtual void setForm(std::shared_ptr<gnomonAbstractDynamicForm> form) = 0;

public:
    virtual gnomonAbstractView *view(void);
    virtual void setView(gnomonAbstractView *view);

public:
    virtual std::shared_ptr<gnomonAbstractVisualization> visualization(void);
    virtual void setFormVisualization(const QString& visu_name, const QVariantMap &parameters);
    virtual void setVisualizationParameters(const QVariantMap &parameters);
    virtual QVariantMap visualizationParameters(void);

public:
    QMap<QString, std::shared_ptr<gnomonAbstractDynamicForm> > outputs() override {
        return {};
    }

    orderedMap outputTypes() override {
        return {};
    }

    void deserializeResults(QJsonObject &serialization) override {

    }

    QJsonObject serializeResults(void) override {
        return QJsonObject();
    }

public:
    dtkCoreParameters parameters() const override;
    void setParameter(const QString& parameter, const QVariant& value) override;
    QMap<QString, QString> parameterGroups() const override;

public:
    inline void  predo(void) override { }
    inline void   redo(void) override { }
    inline void postdo(void) override { }
    inline void   undo(void) override { }

    inline void pause(void) override { }
    inline void resume(void) override { }
    inline void stop(void) override { }
    inline int progress(void) override { return 0; }
    inline QString progressMessage(void) override { return ""; }

public:
    inline virtual QString documentation(void) override { return visu->documentation(); }
    inline virtual QString version(void) override { return visu->version(); };

public slots:
    virtual void update(void);
    virtual void clear(void) override;
    virtual void clear(bool clear_visu);

    virtual void setVisualization(std::shared_ptr<gnomonAbstractVisualization>);
    virtual inline void newVisualization(void) { };

    void connectVisualization(void);
    void disconnectVisualization(void);

public:
    void setVisible(bool visible);

signals:
    void visuParametersChanged(void);

protected:
    QString visu_name = "";
    gnomonAbstractView *_view = nullptr;
    std::shared_ptr<gnomonAbstractVisualization> visu = nullptr;
    bool visible = true;
};


//
// gnomonAbstractVisualizationCommand.h ends here
