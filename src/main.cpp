#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QApplication>
#include "Editor.h"
#include "ImageProvider.h"

void setContextProperties(QQmlApplicationEngine &, QEditor *);

int main(int argc, char *argv[])
{
  QCoreApplication::setApplicationName(QStringLiteral("Image editor"));

  QApplication app(argc, argv);

  QImageProvider * imageProvider = new QImageProvider; // engine frees memory

  QQmlApplicationEngine engine;
  engine.addImageProvider(QStringLiteral("provider"), imageProvider);

  QEditor editor(imageProvider);

  setContextProperties(engine, &editor);
  engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

  return app.exec();
}

void setContextProperties(QQmlApplicationEngine & engine, QEditor * editor)
{
  engine.rootContext()->setContextProperty("editor",         editor);
  engine.rootContext()->setContextProperty("imageProvider",  engine.imageProvider("provider"));
}
