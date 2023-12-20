#include "gnomonTimeSeries.h"
#include "gnomonTime.h"

#include <QtGlobal>

#define MANIFEST "manifest.json"

template <typename T> gnomonTimeSeries<T>::gnomonTimeSeries(const gnomonTimeSeries<T>& o) : gnomonAbstractDynamicForm(o)
{
    m_current_time = o.m_current_time;
    m_current_time_id = o.m_current_time_id;
    m_uuid = QUuid::createUuid().toString(QUuid::WithoutBraces);
    form_id_counter = o.form_id_counter;


    QFile::copy(
            o.storage_dir.filePath(MANIFEST),
            storage_dir.filePath(MANIFEST)
    );

    for (const auto& id : o.m_forms.keys()) {
        m_times[id] = o.m_times[id];
        m_storage_info[id] = o.m_storage_info[id];

        QFile::copy(
                o.storage_dir.filePath(o.m_storage_info[id].fileName),
                storage_dir.filePath(m_storage_info[id].fileName)
                );

        if(o.m_storage_info[id].loaded)
            m_forms[id] = std::make_shared<T>(*(o.m_forms[id].get()));

    }
}

template <typename T> gnomonTimeSeries<T>::gnomonTimeSeries(const QString& uuid) : gnomonAbstractDynamicForm()
{
    // QJsonObject serialization = GNOMON_SESSION->getForm(uuid);
    // this->deserialize(serialization);
    qDebug()<<"Not implemented";
}

template <typename T> std::shared_ptr<gnomonAbstractDynamicForm> gnomonTimeSeries<T>::clone(void) const
{
    qWarning() << Q_FUNC_INFO << "CLONE USED. check usage";

    auto o = std::make_shared<gnomonTimeSeries<T>>(*this);
    return std::dynamic_pointer_cast<gnomonAbstractDynamicForm>(o);
}


template <typename T> gnomonTimeSeries<T>& gnomonTimeSeries<T>::operator=(const gnomonTimeSeries<T>& o)
{
    if (this == &o)
        return *this;
    // TODO: define behavior
    m_forms = o.m_forms;
    m_times = o.m_times;
    m_storage_info = o.m_storage_info;
    m_current_time = o.m_current_time;
    *(p_metadata) = *(o.p_metadata);


    return (*this);
}

template <typename T> std::shared_ptr<T> gnomonTimeSeries<T>::at(double t)
{
    if(containsTime(t)) {
        m_current_time = closestT(t);
        m_current_time_id = idAtT(m_current_time);
        return this->current();
    } else {
        dtkWarn() << Q_FUNC_INFO << "Invalid time position : the form is not defined at this time:" << t;
        return nullptr;
    }
}

template <typename T> std::shared_ptr<gnomonAbstractForm> gnomonTimeSeries<T>::atAsAbstract(double t)
{
    return this->at(t);
}

template <typename T> std::shared_ptr<T> gnomonTimeSeries<T>::current(void) {
    if(!m_storage_info[m_current_time_id].loaded) {
        load(m_current_time_id);
    }
    return m_forms[m_current_time_id];
}

template <typename T> std::shared_ptr<gnomonAbstractForm> gnomonTimeSeries<T>::currentAsAbstract(void) {
    return this->current();
}

template <typename T> double gnomonTimeSeries<T>::time(void) const
{
    return m_current_time;
}

template <typename T> QList<double> gnomonTimeSeries<T>::times(void) const
{
    return m_times.values();
}

template <typename T> QMap<QString,QString> gnomonTimeSeries<T>::metadataAtT(double t) const
{
    if(containsTime(t))
        return m_forms[idAtT(t)]->metadata();
    else
        return QMap<QString, QString>();
}


template <typename T> bool gnomonTimeSeries<T>::containsId(uint id) const
{
    return m_times.contains(id);
}

template <typename T>
bool gnomonTimeSeries<T>::containsTime(double t, double precision) const
{
    for(auto it = m_times.keyValueBegin(); it!=m_times.keyValueEnd(); it++) {
        double distance = abs(t - it->second);
        if(distance <= precision) {
            return true;
        }
    }
    return false;
}

template <typename T>
bool gnomonTimeSeries<T>::containsForm(std::shared_ptr<gnomonAbstractForm> form) const
{
    // TODO: won't work as is for forms that are not loaded
    for(auto it = m_forms.keyValueBegin(); it!=m_forms.keyValueEnd(); it++) {
        if(it->second == form) {
            return true;
        }
    }
    return false;
}

template <typename T> 
void gnomonTimeSeries<T>::insert(double t, std::shared_ptr<gnomonAbstractForm> form)
{
    std::shared_ptr<T> form_casted = std::dynamic_pointer_cast<T>(form);
    if(!form_casted) {
        qWarning() << Q_FUNC_INFO << "Invalid form type : the form is not of the right type " << form->name() << " vs " << T::formName();
    }
    insert(t, std::dynamic_pointer_cast<T>(form));
}

template <typename T> void gnomonTimeSeries<T>::insert(double t, std::shared_ptr<T> form)
{
    auto id = form_id_counter++;
    if (m_forms.size() == 0) {
        m_current_time = t;
        m_current_time_id = id;
    }
    m_forms.insert(id, form);
    m_times.insert(id, t);
    m_storage_info.insert(id,  {
            t, id, QString::number(id), true
    });
    save(t);
}

//void gnomonTimeSeries::insert(const gnomonTimeSeries& dynamic_form)
//{
//    for(auto it = dynamic_form.m_forms.begin(); it != dynamic_form.m_forms.end(); ++it) { // Iterates on the times
//        if(m_forms.contains(it.key())) { // The time already exists
//            for(auto jt = it->begin(); jt != it->end(); ++jt) { // Iterates on the forms of a given time
//                auto forms = m_forms[it.key()];
//                if(!forms.contains(jt.key())) { // The form doesn't exist at the given time
//                    forms.insert(jt.key(), jt.value()); // Insert the form at the given time
//                } else { // The form already exists at the given time
//                    dtkError() << "Invalid form : the form is already defined at this time";
//                }
//            }
//        } else { // The time doesn't exist yet
//            m_forms.insert(it.key(), it.value());  // Insert the time and the form
//        }
//    }
//}

template <typename T> void gnomonTimeSeries<T>::drop(const double t)
{
    Q_ASSERT_X(containsTime(t), "drop", "Invalid time position : the form is not defined at this time");
    uint id = idAtT(t);
    m_forms.remove(id);
    m_times.remove(id);

    QFile::remove(storage_dir.filePath(m_storage_info[id].fileName));
    m_storage_info.remove(id);
    updateManifest();

    if ((m_current_time = t) && (m_forms.size() > 0)) {
        m_current_time_id = m_forms.keys()[0];
        m_current_time_id = m_times[m_current_time_id];
    }
}

template <typename T> uint gnomonTimeSeries<T>::idAtT(double t) const
{
    double min_distance = INFINITY;
    uint min_index = 0;
    for(auto it = m_times.keyValueBegin(); it!=m_times.keyValueEnd(); it++) {
        double distance = abs(t - it->second);
        if(distance == 0.0) {
            return it->first;
        } else if(distance < min_distance) {
            min_distance = distance;
            min_index = it->first;
        }
    }
    return min_index;
}

template <typename T> double gnomonTimeSeries<T>::closestT(double t) const
{
    double min_distance = INFINITY;
    uint min_index = 0;
    for(auto it = m_times.keyValueBegin(); it!=m_times.keyValueEnd(); it++) {
        double distance = abs(t - it->second);
        if(distance == 0.0) {
            return t;
        } else if(distance < min_distance) {
            min_distance = distance;
            min_index = it->first;
        }
    }
    return m_times[min_index];
}

template <typename T> void gnomonTimeSeries<T>::save(uint id)
{
    if(m_storage_info.contains(id) && m_storage_info.value(id).loaded) {
        QJsonObject serialization = m_forms.value(id)->serialize();
        auto content = qCompress(QJsonDocument(serialization).toJson(), 5);
        QFile file(storage_dir.filePath(m_storage_info.value(id).fileName));
        if(file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            file.write(content);
            file.close();
        }
        updateManifest();
    }
}

template <typename T> void gnomonTimeSeries<T>::load(uint id)
{
    if(m_storage_info.contains(id) && !m_storage_info.value(id).loaded) {
        QString filePath = storage_dir.filePath(m_storage_info.value(id).fileName);
        QFile file(filePath);
        QJsonObject formSerialization;
        if(file.open(QIODevice::ReadOnly)) {
            auto content = qUncompress(file.readAll());
            file.close();
            formSerialization = QJsonDocument::fromJson(content).object();
            file.close();
        } else {
            qCritical() << Q_FUNC_INFO << "Could not open file " << filePath;
            throw std::runtime_error("Could not open file " + filePath.toStdString());
        }
        m_forms[id] = std::make_shared<T>(formSerialization);
        m_storage_info[id].loaded = true;
    }
}

template <typename T> void gnomonTimeSeries<T>::unload(uint id)
{

}

template <typename T> void gnomonTimeSeries<T>::load()
{
    readManifest();
    for(uint id: m_storage_info.keys()) {
        load(id);
    }
}

template <typename T> void gnomonTimeSeries<T>::unload()
{
    readManifest();
    for(uint id: m_storage_info.keys()) {
        save(id);
    }
    updateManifest();
    for(uint id: m_storage_info.keys()) {
        unload(id);
    }

}

template <typename T> bool gnomonTimeSeries<T>::loaded()
{
    readManifest();
    for(uint id: m_storage_info.keys()) {
        if(!m_storage_info[id].loaded) {
            return false;
        }
    }
    return true;
}

template <typename T> void gnomonTimeSeries<T>::updateManifest()
{
    QJsonObject manifest;
    manifest.insert("type", formName());

    QJsonObject files_info;
    for(auto it = m_storage_info.keyValueBegin(); it!=m_storage_info.keyValueEnd();it++) {
        QJsonObject file_info;
        file_info.insert("time", it->second.time);
        file_info.insert("fileName", it->second.fileName);

        files_info.insert(QString::number(it->first), file_info);
    }
    manifest.insert("forms", files_info);

    QFile file(storage_dir.filePath(MANIFEST));
    if(file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        file.write(QJsonDocument(manifest).toJson());
        file.close();
    }
}

template <typename T> void gnomonTimeSeries<T>::readManifest()
{
    QFile file(storage_dir.filePath(MANIFEST));
    if(file.open(QIODevice::ReadOnly)) {
        QJsonObject manifest;
        auto content = file.readAll();
        file.close();
        manifest = QJsonDocument::fromJson(content).object();

        QJsonObject file_info_map = manifest.value("forms").toObject();
        for(auto it = file_info_map.constBegin(); it != file_info_map.constEnd(); it++) {
            uint id = (uint) it.key().toInt();
            if(!containsId(id)) {
                QJsonObject file_info = it.value().toObject();
                formStorageInfo info;
                info.fileName = file_info["fileName"].toString();
                info.time = file_info["time"].toDouble();
                info.id = id;
                if(m_forms.contains(id)) {
                    info.loaded = true;
                    qWarning() << "Something wrong is happening, inconsistent indexing";
                } else {
                    info.loaded = false;
                }
                m_times[id] = info.time;
                m_storage_info[id] = info;
            }
        }
    }
}

template<typename T>
QJsonObject gnomonTimeSeries<T>::serialize(void) {
    QJsonObject out(gnomonAbstractDynamicForm::serialize());
    out["current_time"] = m_current_time;
    out["current_index"] = (int) m_current_time_id;
    out["uuid"] = m_uuid;
    out["form_type"] = formName();
    out["form_id_counter"] = (int) form_id_counter;
    if(tmpdir) {
        // must store everything in memory as the tmpdir won't survive
        QJsonObject forms;
        QJsonObject _times;
        for(auto& id: m_times.keys()) {
            bool unloaded = !m_storage_info[id].loaded;
            if(unloaded) {
                load(id);
            }
            forms[QString::number(id)] = m_forms[id]->serialize();
            _times[QString::number(id)] = m_times[id];
            if(unloaded) {
                unload(id);
            }
        }
        out["forms"] = forms;
        out["times"] = _times;
    } else {
        // stored in a permanent storage can restore form from the manifest and files
    }

    return out;
}

template<typename T>
void gnomonTimeSeries<T>::deserialize(const QJsonObject &serialization) {
    gnomonAbstractDynamicForm::deserialize(serialization);

    m_current_time = serialization["current_time"].toDouble();
    m_current_time_id = serialization["current_index"].toInt();
    m_uuid = serialization["uuid"].toString();
    form_id_counter = serialization["form_id_counter"].toInt();
    // emptying current forms map and deleting forms
    m_forms.clear();
    m_times.clear();
    m_storage_info.clear();
    if(serialization.contains("forms")) {
        QJsonObject forms = serialization["forms"].toObject();
        QJsonObject _times = serialization["forms"].toObject();
        for(auto& id: forms.keys()) {
            auto formSerialization = forms[id].toObject();
            m_forms[id.toInt()] = std::make_shared<T>(formSerialization);
            m_times[id.toInt()] = _times[id].toDouble();
            m_storage_info[id.toInt()] = {
                    _times[id].toDouble(), (uint)id.toInt(), id, true
            };
        }
    } else {
        readManifest();
        load();
    }

}

template<typename T>
void gnomonTimeSeries<T>::compose(std::shared_ptr<gnomonAbstractDynamicForm> pForm) {
    if(auto second_form = std::dynamic_pointer_cast<gnomonTimeSeries<T>>(pForm)) {
        auto times1 = this->times();
        auto times2 = pForm->times();
        double offset = times2.first() > times1.last() ? 0. : times1.last() - times2.first() + 1.;
        for(const auto &t: times2) {
            insert(t+offset, second_form->at(t));
        }
    }
}

// /////////////////////////////////////////////////////////////////
// Register to gnomonCore layer
// /////////////////////////////////////////////////////////////////

//namespace gnomonCore {
//    GNOMON_DEFINE_CONCEPT(gnomonTimeSeries, discreteDynamicForm, gnomonCore);
//}

//
// gnomonTimeSeries.tpp ends here
