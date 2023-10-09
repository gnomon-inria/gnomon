//
// This is a modified version of QQmlSettings from QtDeclaratives
// It implements similar functionalities but the actual storage is redirected to the session manager
//

#include "gnomonSettings.h"
#include "gnomonSessionManager.h"
#include <QtQml>
// Copyright (C) 2022 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only


#include <QtQml/qjsvalue.h>
#include <QtQml/qqmlfile.h>
#include <QtQml/qqmlinfo.h>

#include <QtCore/qcoreapplication.h>
#include <QtCore/qcoreevent.h>
#include <QtCore/qdebug.h>
#include <QtCore/qhash.h>
#include <QtCore/qloggingcategory.h>
#include <QtCore/qpointer.h>
#include <QtCore/qsettings.h>


/*!
    \qmltype Settings
//!    \instantiates gnomonSettings
    \inherits QtObject
    \inqmlmodule QtCore
    \since 6.5
    \brief Provides persistent platform-independent application settings.

    The Settings type provides persistent platform-independent application settings.

    Users normally expect an application to remember its settings (window sizes
    and positions, options, etc.) across sessions. The Settings type enables you
    to save and restore such application settings with the minimum of effort.

    Individual setting values are specified by declaring properties within a
    Settings element. Only value types recognized by QSettings are supported.
    The recommended approach is to use property aliases in order
    to get automatic property updates both ways. The following example shows
    how to use Settings to store and restore the geometry of a window.

    \qml
    import QtCore
    import QtQuick

    Window {
        id: window

        width: 800
        height: 600

        Settings {
            property alias x: window.x
            property alias y: window.y
            property alias width: window.width
            property alias height: window.height
        }
    }
    \endqml

    At first application startup, the window gets default dimensions specified
    as 800x600. Notice that no default position is specified - we let the window
    manager handle that. Later when the window geometry changes, new values will
    be automatically stored to the persistent settings. The second application
    run will get initial values from the persistent settings, bringing the window
    back to the previous position and size.

    A fully declarative syntax, achieved by using property aliases, comes at the
    cost of storing persistent settings whenever the values of aliased properties
    change. Normal properties can be used to gain more fine-grained control over
    storing the persistent settings. The following example illustrates how to save
    a setting on component destruction.

    \qml
    import QtCore
    import QtQuick

    Item {
        id: page

        state: settings.state

        states: [
            State {
                name: "active"
                // ...
            },
            State {
                name: "inactive"
                // ...
            }
        ]

        Settings {
            id: settings
            property string state: "active"
        }

        Component.onDestruction: {
            settings.state = page.state
        }
    }
    \endqml

    Notice how the default value is now specified in the persistent setting property,
    and the actual property is bound to the setting in order to get the initial value
    from the persistent settings.

    \section1 Application Identifiers

    Application specific settings are identified by providing application
    \l {QCoreApplication::applicationName}{name},
    \l {QCoreApplication::organizationName}{organization} and
    \l {QCoreApplication::organizationDomain}{domain}, or by specifying
    \l location.

    \code
    #include <QGuiApplication>
    #include <QQmlApplicationEngine>

    int main(int argc, char *argv[])
    {
        QGuiApplication app(argc, argv);
        app.setOrganizationName("Some Company");
        app.setOrganizationDomain("somecompany.com");
        app.setApplicationName("Amazing Application");

        QQmlApplicationEngine engine("main.qml");
        return app.exec();
    }
    \endcode

    These are typically specified in C++ in the beginning of \c main(),
    but can also be controlled in QML via the following properties:
    \list
        \li \l {Qt::application}{Qt.application.name},
        \li \l {Qt::application}{Qt.application.organization} and
        \li \l {Qt::application}{Qt.application.domain}.
    \endlist

    \section1 Categories

    Application settings may be divided into logical categories by specifying
    a category name via the \l category property. Using logical categories not
    only provides a cleaner settings structure, but also prevents possible
    conflicts between setting keys.

    If several categories are required, use several Settings objects, each with
    their own category:

    \qml
    Item {
        id: panel

        visible: true

        Settings {
            category: "OutputPanel"
            property alias visible: panel.visible
            // ...
        }

        Settings {
            category: "General"
            property alias fontSize: fontSizeSpinBox.value
            // ...
        }
    }
    \endqml

    Instead of ensuring that all settings in the application have unique names,
    the settings can be divided into unique categories that may then contain
    settings using the same names that are used in other categories - without
    a conflict.

    \section1 Settings singleton

    It's often useful to have settings available to every QML file as a
    singleton. For an example of this, see the
    \l {Qt Quick Controls - To Do List}{To Do List example}. Specifically,
    \l {https://code.qt.io/cgit/qt/qtdeclarative.git/tree/examples/quickcontrols/ios/todolist/AppSettings.qml}
    {AppSettings.qml} is the singleton, and in the
    \l {https://code.qt.io/cgit/qt/qtdeclarative.git/tree/examples/quickcontrols/ios/todolist/CMakeLists.txt}
    {CMakeLists.txt file},
    the \c QT_QML_SINGLETON_TYPE property is set to \c TRUE for that file via
    \c set_source_files_properties.

    \section1 Notes

    The current implementation is based on \l QSettings. This imposes certain
    limitations, such as missing change notifications. Writing a setting value
    using one instance of Settings does not update the value in another Settings
    instance, even if they are referring to the same setting in the same category.

    The information is stored in the system registry on Windows, and in XML
    preferences files on \macos. On other Unix systems, in the absence of a
    standard, INI text files are used. See \l QSettings documentation for
    more details.

    \sa QSettings
*/


Q_LOGGING_CATEGORY(lcQmlSettings, "qt.core.settings")

static constexpr const int settingsWriteDelay = 500;

class gnomonSettingsPrivate
{
    Q_DISABLE_COPY_MOVE(gnomonSettingsPrivate)

public:
    gnomonSettingsPrivate() = default;
    ~gnomonSettingsPrivate() = default;

    QJsonObject * instance() const;

    void init();
    void reset();

    void load(QJsonObject* storage);
    void store();

    void _q_propertyChanged();
    QVariant readProperty(const QMetaProperty &property) const;

    gnomonSettings *q = nullptr;
    int timerId = 0;
    bool initialized = false;
    QString uuid = "";
    QHash<const char *, QVariant> changedProperties = {};
};

QJsonObject * gnomonSettingsPrivate::instance() const
{
    auto storage = GNOMON_SESSION->getStorageForWorkspace(uuid);

    if (initialized)
        // in the original code but why so leaving it commented ¯\_(ツ)_/¯
        // why would calling load multiple times be necessary ?
        //q->d->load(storage); // not const

    return storage;
}

void gnomonSettingsPrivate::init()
{
    if (initialized)
        return;
    qDebug() << "Initializing settings for uuid: " << uuid;
    load(GNOMON_SESSION->getStorageForWorkspace(uuid));
    initialized = true;
}

void gnomonSettingsPrivate::reset()
{
    if (initialized && !changedProperties.isEmpty())
        store();
    //TODO: reset the storage
}

void gnomonSettingsPrivate::load(QJsonObject* storage)
{
    const QMetaObject *mo = q->metaObject();
    const int offset = mo->propertyOffset();
    const int count = mo->propertyCount();

    // don't save built-in properties if there aren't any qml properties
    if (offset == 1)
        return;

    for (int i = offset; i < count; ++i) {
        QMetaProperty property = mo->property(i);
        const QString propertyName = QString::fromUtf8(property.name());

        const QVariant previousValue = readProperty(property);
        const QVariant currentValue = storage->contains(propertyName) ? storage->value(propertyName) : previousValue;

        if (!currentValue.isNull() && (!previousValue.isValid()
                                       || (currentValue.canConvert(previousValue.metaType())
                                           && previousValue != currentValue))) {
            property.write(q, currentValue);
            qCDebug(lcQmlSettings) << "gnomonSettings: load" << property.name() << "setting:" << currentValue << "default:" << previousValue;
            qDebug() << "gnomonSettings: load" << property.name() << "setting:" << currentValue << "default:" << previousValue;
        }

        // ensure that a non-existent setting gets written
        // even if the property wouldn't change later
        if (!storage->contains(propertyName))
            _q_propertyChanged();

        // setup change notifications on first load
        if (!initialized && property.hasNotifySignal()) {
            static const int propertyChangedIndex = mo->indexOfSlot("_q_propertyChanged()");
            QMetaObject::connect(q, property.notifySignalIndex(), q, propertyChangedIndex);
        }
    }
}

void gnomonSettingsPrivate::store()
{
    QHash<const char *, QVariant>::const_iterator it = changedProperties.constBegin();
    while (it != changedProperties.constEnd()) {
        instance()->insert(QString::fromUtf8(it.key()), it.value().toJsonValue());
        qCDebug(lcQmlSettings) << "gnomonSettings: store" << it.key() << ":" << it.value();
        ++it;
    }
    changedProperties.clear();
    q->sync();
}

void gnomonSettingsPrivate::_q_propertyChanged()
{
    const QMetaObject *mo = q->metaObject();
    const int offset = mo->propertyOffset();
    const int count = mo->propertyCount();
    for (int i = offset; i < count; ++i) {
        const QMetaProperty &property = mo->property(i);
        const QVariant value = readProperty(property);
        changedProperties.insert(property.name(), value);
        qCDebug(lcQmlSettings) << "gnomonSettings: cache" << property.name() << ":" << value;
        qDebug() << "gnomonSettings: cache" << property.name() << ":" << value;
    }
    if (timerId != 0)
        q->killTimer(timerId);
    timerId = q->startTimer(settingsWriteDelay);
}

QVariant gnomonSettingsPrivate::readProperty(const QMetaProperty &property) const
{
    QVariant var = property.read(q);
    if (var.metaType() == QMetaType::fromType<QJSValue>()) {
        var = var.value<QJSValue>().toVariant();
    }
    return var;
}

gnomonSettings::gnomonSettings(QObject *parent)
        : QObject(parent), d(new gnomonSettingsPrivate)
{
    d->q = this;
}

gnomonSettings::~gnomonSettings()
{
    d->reset(); // flush pending changes
}

/*!
   \qmlmethod var Settings::value(string key, var defaultValue)

   Returns the value for setting \a key. If the setting doesn't exist,
   returns \a defaultValue.

   \sa QSettings::value
*/
QVariant gnomonSettings::value(const QString &key, const QVariant &defaultValue) const
{
    auto storage = d->instance();
    return storage->contains(key) ? storage->value(key).toVariant() : defaultValue;
}

/*!
   \qmlmethod Settings::setValue(string key, var value)

   Sets the value of setting \a key to \a value. If the key already exists,
   the previous value is overwritten.

   \sa QSettings::setValue
*/
void gnomonSettings::setValue(const QString &key, const QVariant &value)
{
    d->instance()->insert(key, value.toJsonValue());
    qCDebug(lcQmlSettings) << "gnomonSettings: setValue" << key << ":" << value;
}

/*!
   \qmlmethod Settings::sync()

    Writes any unsaved changes to permanent storage, and reloads any
    settings that have been changed in the meantime by another
    application.

    This function is called automatically from QSettings's destructor and
    by the event loop at regular intervals, so you normally don't need to
    call it yourself.

   \sa QSettings::sync
*/
void gnomonSettings::sync()
{
    GNOMON_SESSION->sync();
}

void gnomonSettings::classBegin()
{
}

void gnomonSettings::componentComplete()
{
    d->init();
}

void gnomonSettings::timerEvent(QTimerEvent *event)
{
    QObject::timerEvent(event);
    if (event->timerId() != d->timerId)
        return;
    killTimer(d->timerId);
    d->timerId = 0;
    d->store();
}

QString gnomonSettings::category() {
    return d->uuid;
}

void gnomonSettings::setCategory(const QString& value) {
    if(value != d->uuid) {
        d->uuid = value;
        emit categoryChanged(value);
    }
}

void gnomonSettings::_q_propertyChanged() {
    d->_q_propertyChanged();
}

