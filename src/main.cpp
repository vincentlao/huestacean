#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QFontDatabase>
#include <QQmlContext>
#include "huestacean.h"

#ifdef _WIN32
#include <Windows.h>
#endif

extern QQmlApplicationEngine* engine;
QQmlApplicationEngine* engine = nullptr;

static QObject *huestacean_singleton_provider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);

    Huestacean *huestacean = new Huestacean();
    return huestacean;
}

int main(int argc, char *argv[])
{
#ifdef _WIN32
    if (AttachConsole(ATTACH_PARENT_PROCESS)) {
        freopen("CONOUT$", "w", stdout);
        freopen("CONOUT$", "w", stderr);
    }
#endif

    //https://github.com/sqlitebrowser/sqlitebrowser/commit/73946400c32d1f7cfcd4672ab0ab3f563eb84f4e
    qputenv("QT_BEARER_POLL_TIMEOUT", QByteArray::number(INT_MAX));

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setOrganizationName("BradyBrenot");
    QCoreApplication::setOrganizationDomain("bradybrenot.com");
    QCoreApplication::setApplicationName("Huestacean");

    QGuiApplication app(argc, argv);

    qmlRegisterSingletonType<Huestacean>("Huestacean", 1, 0, "Huestacean", huestacean_singleton_provider);

    const QFont fixedFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    QQmlApplicationEngine theEngine;
    ::engine = &theEngine;
    theEngine.rootContext()->setContextProperty("fixedFont", fixedFont);
    theEngine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
    if (theEngine.rootObjects().isEmpty())
        return -1;

	theEngine.rootContext()->setContextProperty("mainWindow", theEngine.rootObjects().first());

    return app.exec();
}
