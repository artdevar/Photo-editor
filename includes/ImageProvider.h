#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H

#include <QObject>
#include <QQuickImageProvider>
#include <QImage>
#include <QByteArray>
#include <memory>

class QImageProvider : public QQuickImageProvider
{
  Q_OBJECT
  Q_PROPERTY(const QVector<QRgb> & imagePixels READ getImagePixels CONSTANT)

signals:

  void imageOpened(int width, int height);
  void imageClosed();
  void imageEdited();
  void imageSaved(const QString & fileName);

protected:

  struct ImageInfo
  {
    QVector<QRgb> pixels;
    QString       name;
  };

public:

  QImageProvider();
  ~QImageProvider() override;

  QImage requestImage(const QString & id, QSize * size, const QSize & requestedSize) override;

  bool openImage(const QString & fileName);
  void closeImage();
  bool saveImage() const;
  bool saveImageAs(const QString & fileName) const;
  bool isImageOpened() const;

  void loadImageFromData(const QVector<QRgb> & rawData);
  void loadImageFromData(QVector<QRgb> && rawData);

  void mirrorImage(Qt::Orientation orientation);
  void invert();

  const QVector<QRgb> & getImagePixels() const;
  QSize getImageSize() const;

protected:

  void onImageEdited();

  void fillImageInfo(const QString & fileName);
  void updateImageInfo();
  void clearImageInfo();

protected:

  ImageInfo               m_ImageInfo;
  std::unique_ptr<QImage> m_Image;
};

#endif // IMAGEPROVIDER_H
