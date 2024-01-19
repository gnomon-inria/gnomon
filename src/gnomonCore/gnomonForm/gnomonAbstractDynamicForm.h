#pragma once

#include <gnomonCoreExport.h>

#include <memory>
#include <QtCore>
#include <dtkCore>

#include "gnomonAbstractForm"
#include "gnomonDynamicFormMetadata"

class GNOMONCORE_EXPORT gnomonAbstractDynamicForm
{
public:
     gnomonAbstractDynamicForm(void) : p_metadata(new gnomonDynamicFormMetadata), tmpdir(new QTemporaryDir) {
         storage_dir.cd(tmpdir->path());
     };

    gnomonAbstractDynamicForm(const gnomonAbstractDynamicForm &o) : p_metadata(new gnomonDynamicFormMetadata), tmpdir(new QTemporaryDir) {
        *p_metadata = *o.p_metadata;
        // always default to tmpdir in constructor as two objects can't share the same objects
        storage_dir.cd(tmpdir->path());
    };
    virtual ~gnomonAbstractDynamicForm(void) {
        delete p_metadata;
        delete tmpdir;
    }


public:
    virtual std::shared_ptr<gnomonAbstractDynamicForm> clone(void) const = 0;

public:
/*
    std::shared_ptr<gnomonAbstractForm> at(double t) {
        qWarning() << Q_FUNC_INFO << "MAKING A CLONE";
        return std::shared_ptr<gnomonAbstractForm>(this->at_impl(t)->clone());
    };
    std::shared_ptr<gnomonAbstractForm> current(void) {
        qWarning() << Q_FUNC_INFO << "MAKING A CLONE";
        return std::shared_ptr<gnomonAbstractForm>(this->current_impl()->clone());
    };
*/    
    virtual void insert(double t, std::shared_ptr<gnomonAbstractForm> form) = 0;
    virtual QMap<QString,QString> metadataAtT(double t) const = 0;

    virtual bool containsId(uint id) const = 0;
    virtual bool containsTime(double t, double precision) const = 0;
    virtual bool containsForm(std::shared_ptr<gnomonAbstractForm> form) const = 0;

    virtual double time(void) const = 0;
    virtual QList<double> times(void) const = 0;
    virtual void compose(std::shared_ptr<gnomonAbstractDynamicForm> pForm) = 0;

    virtual void drop(double t) = 0;

    virtual QString formName(void) = 0; //TODO CRTP to be able to put that as virtual static
    virtual QString uuid(void) = 0;

    int thumbnailId(void) {
        return m_thumbnail_id;
    }

    void setThumbnailId(int id) {
        m_thumbnail_id = id;
    }

    virtual QJsonObject serialize(void) {
        QJsonObject json;
        json["metadata"] = p_metadata->serialize();
        if(!tmpdir) {
            // TODO: come back later as it probably shouldn't be an absolute path
            json["storage_dir"] = storage_dir.absolutePath();
        }
        return json;
    }

    virtual void deserialize(const QJsonObject & json) {
        p_metadata->deserialize(json);
        if(json.contains("storage_dir")) {
            setFormStorageDir(json["storage_dir"].toString());
        }
    }

    gnomonDynamicFormMetadata* metadata(void) {
        return p_metadata;
    }

public:
    void setFormStorageDir(const QString &path) {
        if(storage_dir != QDir(path)) {
            QDir new_dir(path);
            for(const auto &file_info : storage_dir.entryInfoList()) {
                if(file_info.isFile()) {
                    QFile::copy(file_info.filePath(), new_dir.filePath(file_info.fileName()));
                    QFile::remove(file_info.filePath());
                }
            }
            // we're not in the default tmpdir anymore, it can go away
            delete tmpdir;
            tmpdir = nullptr;

            storage_dir = new_dir;
        }
    }

    virtual void load() = 0;
    virtual void unload() = 0;
    virtual bool loaded() = 0;

    virtual std::shared_ptr<gnomonAbstractForm> atAsAbstract(double t) = 0;
    virtual std::shared_ptr<gnomonAbstractForm> currentAsAbstract(void) = 0;
    
protected:
    gnomonDynamicFormMetadata *p_metadata = nullptr;
    QDir storage_dir;
    int m_thumbnail_id = -1;

    QTemporaryDir* tmpdir = nullptr;
};
//
// gnomonAbstractDynamicForm.h ends here
