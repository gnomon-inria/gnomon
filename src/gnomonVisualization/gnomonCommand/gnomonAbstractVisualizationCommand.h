#pragma once

#include <gnomonVisualizationExport>

#include "gnomonCore/gnomonCommand/gnomonAbstractCommand.h"

class gnomonAbstractDynamicForm;
class gnomonAbstractView;
class gnomonAbstractVisualization;

class GNOMONCORE_EXPORT gnomonAbstractVisualizationCommand : public gnomonAbstractCommand
{

public:
    virtual void setForm(std::shared_ptr<gnomonAbstractDynamicForm> form) = 0;

public:
    virtual gnomonAbstractView *view(void);
    virtual void setView(gnomonAbstractView *view);

public:
    virtual std::shared_ptr<gnomonAbstractVisualization> visualization(void);
    virtual void setFormVisualization(const QString& visu_name, const QVariantMap &parameters);
    virtual void setVisualizationParameters(const QVariantMap &parameters);

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
    inline void  predo(void) override { }
    inline void postdo(void) override { }
    inline void   undo(void) override { }
    void update(void);

public:
    void setVisible(bool visible);

protected:
    gnomonAbstractView *_view = nullptr;
    std::shared_ptr<gnomonAbstractVisualization> visu = nullptr;
    bool visible = true;
};


//
// gnomonAbstractVisualizationCommand.h ends here
