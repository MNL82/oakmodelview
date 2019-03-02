#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QFont>

int main(int argc, char *argv[])
{
    //qputenv("QT_DEBUG_PLUGINS", "1");
    //qputenv("QML_IMPORT_TRACE", "1");

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QFont font = app.font();
    font.setPixelSize(14);
    app.setFont(font);

    QQmlApplicationEngine engine;
    engine.addImportPath("../Lib/Plugins");
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
