//
// Created by arthur on 19/09/23.
//

#pragma once


// Copyright (C) 2022 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only


//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QtCore/qobject.h>
#include <QtCore/qvariant.h>
#include <QtQml/qqml.h>
#include <QtQml/qqmlparserstatus.h>

QT_BEGIN_NAMESPACE

class gnomonSettingsPrivate;

class gnomonSettings : public QObject, public QQmlParserStatus {
Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)

    QML_NAMED_ELEMENT(Settings)

    friend class gnomonSettingsPrivate;

public:
    explicit gnomonSettings(QObject* parent = nullptr);

    ~gnomonSettings() override;

    Q_INVOKABLE QVariant value(const QString &key, const QVariant &defaultValue = {}) const;

    Q_INVOKABLE void setValue(const QString &key, const QVariant &value);

    Q_INVOKABLE void sync();


protected:
    void timerEvent(QTimerEvent *event) override;

    void classBegin() override;

    void componentComplete() override;

private:
    gnomonSettingsPrivate* d = nullptr;

};

QT_END_NAMESPACE
