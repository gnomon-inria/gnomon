#include <QtCore>
#include <QApplication>
#include <QStyleHints>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QtQml/qqmlextensionplugin.h>
#include <QLoggingCategory>

#include <xVis.hpp>

#include "gnomon.hpp"
#include "MaterialPreview.hpp"

Q_IMPORT_QML_PLUGIN(gnomonQuick_ControlsPlugin)
Q_IMPORT_QML_PLUGIN(gnomonQuick_MenusPlugin)
Q_IMPORT_QML_PLUGIN(gnomonQuick_MonacoPlugin)
Q_IMPORT_QML_PLUGIN(gnomonQuick_StylePlugin)
Q_IMPORT_QML_PLUGIN(gnomonQuick_WorkspacesPlugin)

int main(int argc, char *argv[])
{
    gnomon_rinit();

    QApplication app(argc, argv);
    app.setApplicationName("gnomon");
    app.setOrganizationName("inria");
    app.setOrganizationDomain("fr");

    QQuickStyle::setStyle("Basic");
    QGuiApplication::styleHints()->setUseHoverEffects(true);
    QLoggingCategory::setFilterRules(QStringLiteral("qt.qml.binding.removal.info=true"));

    QQmlApplicationEngine engine;

    //TODO for qt >= 6.5 !
    //engine.addImportPath("qrc:/qt/qml/");
    //engine.load(QUrl("qrc:/qt/qml/gnomon/main.qml"));
    engine.addImportPath("qrc:/");
    //engine.set_property("_title", QVariant::from(QString::from("gnomon")));

    engine.load(QUrl("qrc:/gnomon/main.qml"));
    //engine.load(QUrl("qrc:/gnomon/main_temp.qml"));

    workspaceImageProvider *imageProvider = new workspaceImageProvider;
    engine.addImageProvider("thumbnails", new gnomonImageProvider);
    engine.addImageProvider("workspaces", imageProvider);

    QObject *root = engine.rootObjects().first();
    QObject::connect(root, SIGNAL(getScreenshot(QString)), imageProvider, SLOT(makeScreenshot(QString)));

    gnomonInitLogServer();
    app.setWindowIcon(QIcon("qrc:/gnomon/assets/gnomon_logo.png"));

    // from x_quick
#if __APPLE__
    //xQuickWindowWrap(&engine);
#endif

    return app.exec();
}
