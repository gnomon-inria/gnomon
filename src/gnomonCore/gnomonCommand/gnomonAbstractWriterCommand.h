#pragma once

#include <gnomonCoreExport>

#include "gnomonAbstractAlgorithmCommand.h"

class gnomonAbstractDynamicForm;

class GNOMONCORE_EXPORT  gnomonAbstractWriterCommand : public gnomonAbstractAlgorithmCommand
{

public:
    virtual const QString& path() {return this->m_path;}
    virtual void setPath(const QString & path) { m_path = path; };
    virtual void setForm(std::shared_ptr<gnomonAbstractDynamicForm> form) = 0;

public:
    virtual QStringList extensions(void);

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

protected:
    QString m_path = "";
};


//
// gnomonAbstractWriterCommand.h ends here
