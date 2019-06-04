#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QApplication>
#include "MainWindow.h"
#include "ColorHistogram.h"

void registerTypes();
void setContextProperties(QQmlApplicationEngine &, MainWindow *);

int main(int argc, char *argv[])
{
    QCoreApplication::setApplicationName(QStringLiteral("Abode Photoshop CS24"));
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QQuickStyle::setStyle(QStringLiteral("Material"));

    QApplication app(argc, argv);
    registerTypes();

    MainWindow mainWindow;

    QQmlApplicationEngine engine;
    engine.addImageProvider(QStringLiteral("imageProvider"), ImageHandler::instance());
    setContextProperties(engine, &mainWindow);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}

void registerTypes()
{
    qmlRegisterType<ColorHistogram>("viewmodels", 1, 0, "ColorHistogram");
}

void setContextProperties(QQmlApplicationEngine & engine, MainWindow * window_ptr)
{
    engine.rootContext()->setContextProperty("imageHandler",   ImageHandler::instance());
    engine.rootContext()->setContextProperty("imageCorrector", ImageCorrector::instance());
    engine.rootContext()->setContextProperty("mainWindow",     window_ptr);
}