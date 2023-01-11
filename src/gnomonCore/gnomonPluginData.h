#pragma once

#include <QtCore>

class gnomonPluginData: public QObject {

    Q_OBJECT

    Q_PROPERTY(QString key READ getKey CONSTANT)
    Q_PROPERTY(QString name READ getName NOTIFY nameChanged)
    Q_PROPERTY(QString doc READ getDoc NOTIFY docChanged)
    Q_PROPERTY(QString package READ getPackage CONSTANT)

    signals:
    //void keyChanged();
    void nameChanged();
    void docChanged();
    //void packageChanged();

public:
    explicit gnomonPluginData(QObject *parent);
    gnomonPluginData(const gnomonPluginData& o);

    gnomonPluginData(const QString &key, const QString &name="", const QString &doc="",
                     const QString &package="");

    const QString &getKey() const;

    const QString &getName() const;

    const QString &getDoc() const;

    const QString &getPackage() const;

    gnomonPluginData& operator=(const gnomonPluginData &);

    bool operator==(const gnomonPluginData &rhs) const;

    bool operator!=(const gnomonPluginData &rhs) const;

private:
    QString key = "";
    QString name = "";
    QString doc = "";
    QString package = "";
};


template<typename factoryType> QList<gnomonPluginData*> gnomonPluginDataListFromFactory(factoryType factory) {
    // append only storage of plugin data by factory
    static QList<gnomonPluginData> storage;
    static QList<QString> storedKeys;

    // updating storage
    const auto & keys = factory.keys();
    for(const auto & key: keys) {
        if(!storedKeys.contains(key)) {
            storage.append({key, factory.name(key), factory.doc(key), ""});
            storedKeys.append(key);
        }
    }

    // --
    QList<gnomonPluginData*> out;
    for(gnomonPluginData & data: storage) {
        out.append(&data);
    }
    return out;
}
